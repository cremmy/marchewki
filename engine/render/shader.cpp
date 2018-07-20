/*
 * shader.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "shader.h"

#include <cstring>
#include <GL/glu.h>

#include "../consts.h"
#include "../debug/log.h"
#include "../io/resource.h"
#include "../utils/stringparser.h"

using namespace Engine::Render;

const int MAX_INCLUDE_LEVEL=32;
const char PRAGMA_MARKER[]="wtsngine";
const char PRAGMA_INCLUDE[]="include";
const char PRAGMA_SHADER_VERTEX[]="vertex";
const char PRAGMA_SHADER_FRAGMENT[]="fragment";

bool Shader::load(const std::string& path)
	{
	const std::string pathV=path+SHADER_VERTEX_EXTENSION;
	const std::string pathF=path+SHADER_FRAGMENT_EXTENSION;

	GLuint err;

	clear();
	glGetError(); // Wyzeruj bledy OpenGL

	LOG_INFO("Shader.load: Wczytywanie \"%s\"", path.c_str());

	/***** Wczytanie i kompilacja shaderow *****/
	auto compile=[](const char* data, const int shadertype)->GLuint
		{
		const GLchar* source[1];
		GLint cresult;
		GLint cinfolength;

		source[0]=data;

		GLuint shader=glCreateShader(shadertype);
		glShaderSource(shader, 1, source, nullptr);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &cresult);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &cinfolength);

		/**** Wypisanie informacji o ewentualnych bledach ****/
		if(cresult!=GL_TRUE)
			{
			std::vector<char> info;
			info.resize(cinfolength);

			glGetShaderInfoLog(shader, cinfolength, NULL, info.data());

			LOG_ERROR("Shader.load: Nie udalo sie skompilowac shadera");

			char* ptr;
			ptr=strtok(info.data(), "\r\n");

			while(ptr)
				{
				LOG_ERROR("Shader.load: %s", ptr);

				ptr=strtok(nullptr, "\r\n");
				}

			glDeleteShader(shader);

			return 0u;
			}

		return shader;
		};

	GLuint shaderV;
	GLuint shaderF;

	std::stringstream ssvert;
	std::stringstream ssfrag;

	if(IO::Resource::find(path))
		{
		if(!load(path, ssvert, ssfrag))
			{
			//LOG_ERROR("Shader.load: Nie udalo sie wczytac kodu shadera \"%s\"", path.c_str());
			return false;
			}
		}
	else
		{
		if(!load(pathV, ssvert, ssfrag, true) ||
		   !load(pathF, ssvert, ssfrag, false))
			{
			//LOG_ERROR("Shader.load: Nie udalo sie wczytac kodu shadera \"%s\"/\"%s\"", pathV.c_str(), pathF.c_str());
			return false;
			}
		}

	shaderV=compile(ssvert.str().data(), GL_VERTEX_SHADER);
	shaderF=compile(ssfrag.str().data(), GL_FRAGMENT_SHADER);

	if(!shaderV || !shaderF)
		{
		return false;
		}

	/***** Linkowanie programu *****/
	program=glCreateProgram();

	if((err=glGetError())!=GL_NO_ERROR)
		{
		LOG_ERROR("Shader.load: Nie udalo sie utworzyc programu");
		LOG_ERROR("Shader.load: %s", gluErrorString(err));

		program=0u;

		return false;
		}

	glAttachShader(program, shaderV);
	glAttachShader(program, shaderF);
	glLinkProgram(program);
	glDetachShader(program, shaderV);
	glDetachShader(program, shaderF);
	glDeleteShader(shaderV);
	glDeleteShader(shaderF);

	if((err=glGetError())!=GL_NO_ERROR)
		{
		LOG_ERROR("Shader.load: Linkowanie programu nie powiodlo sie \"%s\", \"%s\"", pathV.c_str(), pathF.c_str());
		LOG_ERROR("Shader.load: %s", gluErrorString(err));

		program=0u;

		return false;
		}

	LOG_SUCCESS("Shader.load: Shader \"%s\" gotowy [GLid: %u]", path.c_str(), program);

	return true;
	}


bool Shader::load(const std::string& path, std::stringstream& ssvert, std::stringstream& ssfrag, bool vert, int level)
	{
	if(level>MAX_INCLUDE_LEVEL)
		return false;

	LOG_DEBUG("Shader.load: [%2d: %s] ", level, path.c_str());

	char *data;

	/**** Wczytywanie zasobow ****/
	data=Engine::IO::Resource::load(path);

	if(!data)
		{
		LOG_ERROR("Shader.load: Nie udalo sie wczytac \"%s\"", path.c_str());
		return false;
		}

	LOG_DEBUG("Shader.load: [%2d: %s] Wczytano kod shadera", level, path.c_str());

	/**** Szukanie includeów ****/
	Utils::StringParser spline(data, "\n\r");
	Utils::StringParser spwhite("", " \t");

	//for(auto line: spline)
	for(unsigned i=0u; i<spline.count(); ++i)
		{
		const std::string& line=spline[i];

		if(line[0]!='#')
			{
			if(vert)
				ssvert << line << "\n";
			else
				ssfrag << line << "\n";

			continue;
			}

		spwhite.set(line);

		if(spwhite[0]=="#pragma" && spwhite[1]==PRAGMA_MARKER)
			{
			if(spwhite[2]==PRAGMA_SHADER_VERTEX)
				{
				vert=true;
				}
			else if(spwhite[2]==PRAGMA_SHADER_FRAGMENT)
				{
				vert=false;
				}
			else if(spwhite[2]==PRAGMA_INCLUDE)
				{
				if(spwhite.count()<4)
					{
					LOG_ERROR("Shader.load: [%2d: %s] Nie podano sciezki do pliku (linia %u)", level, path.c_str(), i);
					return false;
					}

				const std::string incpath=path.substr(0, path.rfind('/')+1)+spwhite[3];

				LOG_DEBUG("Shader.load: [%2d: %s][include \"%s\"]", level, path.c_str(), incpath.c_str());

				if(!load(incpath, ssvert, ssfrag, vert, level+1))
					{
					return false;
					}
				}

			continue;
			}
		else
			{
			if(vert)
				ssvert << line << "\n";
			else
				ssfrag << line << "\n";
			}
		}

	return true;
	};


void Shader::clear()
	{
	LOG_DEBUG("Shader.clear: Czyszczenie shadera: [program: %u] [uniforms: %u]", program, uniforms.size());

	if(program!=0u)
		glDeleteProgram(program);
	uniforms.clear();
	}

GLint Shader::getUniform(const std::string& name) const
	{
	auto it=uniforms.find(name);

	if(it!=uniforms.end())
		return it->second;

	const GLint id=glGetUniformLocation(program, name.c_str());

	if(id>=0)
		{
		uniforms[name]=id;
		}

	return id;
	}

GLuint Shader::getUniformBlock(const std::string& name) const
	{
	auto it=uniforms.find(name);

	if(it!=uniforms.end())
		return it->second;

	const GLuint id=glGetUniformBlockIndex(program, name.c_str());

	if(id!=GL_INVALID_INDEX)
		{
		uniforms[name]=id;
		}

	return id;
	}

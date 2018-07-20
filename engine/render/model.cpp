/*
 * model.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: crm
 */

#include "model.h"

#include <cstring>
#include <GL/glu.h>

#include "../consts.h"

#include "../debug/log.h"
#include "../io/resource.h"
#include "../utils/stringparser.h"

using namespace Engine::Render;

Model::Model(): uboid(0u)
	{
	//
	}

Model::~Model()
	{
	//
	}


bool Model::load(const std::string& path)
	{
	clear();

	LOG_INFO("Model.load: Wczytywanie \"%s\"", path.c_str());

	struct Face
		{
		/*Face():
			av(0), an(0), at(0),
			bv(0), bn(0), bt(0),
			cv(0), cn(0), ct(0) {}
		Face(const Face& f)=default;*/

		unsigned av, at, an;
		unsigned bv, bt, bn;
		unsigned cv, ct, cn;
		};

	std::vector<Math::Vector> verts;
	std::vector<Math::Vector> uvs;
	std::vector<Math::Vector> normals;
	std::vector<Face> faces;

	verts.reserve(1024);
	uvs.reserve(1024);
	normals.reserve(1024);
	normals.reserve(256);

	auto loadMtl=[this](const std::string& path)->bool
		{
		LOG_INFO("Model.loadMtl: Wczytywanie materialow \"%s\"", path.c_str());

		char* data=IO::Resource::load(path);

		if(!data)
			{
			LOG_ERROR("Model.loadMtl: Nie udalo sie wczytac \"%s\"", path.c_str());
			return false;
			}

		Utils::StringParser pfile(data, "\n\r");
		Utils::StringParser pline("", " \t");

		bool mtlfound=false;

		for(unsigned i=0u; i<pfile.count(); ++i)
			{
			if(pfile[i].size()<=1u || pfile[i][0]=='#')
				continue;

			pline=pfile[i];

			if(!mtlfound)
				{
				if(pline[0]=="newmtl")
					{
					LOG_INFO("Model.loadMtl: Znaleziono material \"%s\"", pline.get().c_str());
					mtlfound=true;
					}
				else
					{
					continue;
					}
				}
			else if(pline[0]=="newmtl")
				{
				LOG_WARNING("Model.loadMtl: Znaleziono kolejny material \"%s\", przerywanie wczytywania", pline.get().c_str());
				break;
				}
			else if(pline[0]=="Ka")
				{
				if(pline.count()<4)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.ambient[0]=pline.toFloat(1);
				material.ambient[1]=pline.toFloat(2);
				material.ambient[2]=pline.toFloat(3);
				}
			else if(pline[0]=="Kd")
				{
				if(pline.count()<4)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.diffuse[0]=pline.toFloat(1);
				material.diffuse[1]=pline.toFloat(2);
				material.diffuse[2]=pline.toFloat(3);
				}
			else if(pline[0]=="Ks")
				{
				if(pline.count()<4)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.specular[0]=pline.toFloat(1);
				material.specular[1]=pline.toFloat(2);
				material.specular[2]=pline.toFloat(3);
				}
			else if(pline[0]=="Ns")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.specularexp=pline.toFloat(1);
				}
			else if(pline[0]=="d")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				material.transparency=pline.toFloat(1);
				}
			else if(pline[0]=="map_Kd")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}

				std::string tpath;

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					tpath=pline.get().substr(7);
					}
				else
					{
					tpath=path.substr(0, slpos+1)+pline.get().substr(7);
					}

				texDiffuse=TexturePtr(tpath);
				}
			else if(pline[0]=="map_Bump")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}


				std::string tpath;

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					tpath=pline.get().substr(9);
					}
				else
					{
					tpath=path.substr(0, slpos+1)+pline.get().substr(9);
					}

				texNormal=TexturePtr(tpath);
				}
			else if(pline[0]=="shader")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadMtl: Za malo parametrow [\"%s\":%u]", path.c_str(), i);
					LOG_WARNING("Model.loadMtl: \"%s\"", pline.get().c_str());
					continue;
					}


				std::string spath;

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					spath=pline[1];
					}
				else
					{
					spath=path.substr(0, slpos+1)+pline[1];
					}

				shader=ShaderPtr(spath);
				}
			}

		LOG_DEBUG("[ambient %.2f %.2f %.2f][diffuse %.2f %.2f %.2f][specular %.2f %.2f %.2f][specular exp %f][transparency %f]",
				material.ambient[0], material.ambient[1], material.ambient[2],
				material.diffuse[0], material.diffuse[1], material.diffuse[2],
				material.specular[0], material.specular[1], material.specular[2],
				material.specularexp,
				material.transparency
				);

		return true;
		};

	auto loadObj=[this, loadMtl, &verts, &uvs, &normals, &faces](const std::string& path, bool onlyMaterial)->bool
		{
		LOG_INFO("Model.loadObj: Wczytywanie modelu \"%s\"", path.c_str());

		char* data=IO::Resource::load(path);

		if(!data)
			{
			LOG_ERROR("Model.loadObj: Nie udalo sie wczytac \"%s\"", path.c_str());
			return false;
			}

		Utils::StringParser pfile(data, "\n\r");
		Utils::StringParser pline("", " \t");
		Utils::StringParser pface("", "/", Utils::StringParser::DelimiterMode::EACH);

		LOG_DEBUG("Model.loadObj: [lines: %u]", pfile.count());

		bool onceft=false;
		bool oncefn=false;

		for(unsigned i=0u; i<pfile.count(); ++i)
			{
			if(pfile[i].size()<=1u || pfile[i][0]=='#')
				continue;

			pline=pfile[i];

			// Material
			if(pline[0]=="mtllib")
				{
				if(pline.count()<2)
					{
					LOG_WARNING("Model.loadObj: Nie podano sciezki do materialu");
					continue;
					}

				std::string mpath;

				pline.remove(0);

				auto slpos=path.rfind('/');
				if(slpos==path.npos)
					{
					mpath=pline.get();
					}
				else
					{
					mpath=path.substr(0, slpos+1)+pline.get();
					}

				if(!loadMtl(mpath))
					{
					LOG_WARNING("Model.loadObj: Nie udalo sie wczytac materialu z pliku \"%s\"", mpath.c_str());
					//return false; // Brak tekstur nie bedzie krytycznym problemem
					continue;
					}

				if(onlyMaterial)
					{
					delete [] data;
					return true;
					}
				}
			// Vertex
			else if(pline[0]=="v" && !onlyMaterial)
				{
				if(pline.count()<4)
					{
					LOG_ERROR("Model.loadObj: Za malo wspolrzednych wierzcholka [\"%s\":%u]", path.c_str(), i);
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					return false;
					}

				verts.push_back(Math::Vector(pline.toFloat(1), pline.toFloat(2), pline.toFloat(3)));
				}
			// Texture
			else if(pline[0]=="vt" && !onlyMaterial)
				{
				if(pline.count()<3)
					{
					LOG_ERROR("Model.loadObj: Za malo wspolrzednych wierzcholka (tekstura) [\"%s\":%u]", path.c_str(), i);
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					return false;
					}

				uvs.push_back(Math::Vector(pline.toFloat(1), pline.toFloat(2)));
				}
			// Normal
			else if(pline[0]=="vn" && !onlyMaterial)
				{
				if(pline.count()<4)
					{
					LOG_ERROR("Model.loadObj: Za malo wspolrzednych wierzcholka (normalny) [\"%s\":%u]", path.c_str(), i);
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					return false;
					}

				normals.push_back(Math::Vector(pline.toFloat(1), pline.toFloat(2), pline.toFloat(3)));
				}
			// Face
			else if(pline[0]=="f" && !onlyMaterial)
				{
				if(pline.count()<4)
					{
					LOG_ERROR("Model.loadObj: Wymagane trzy wierzcholki do face'a [\"%s\":%u]", path.c_str(), i);
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					return false;
					}

				pface=pline[1];
				unsigned fav=pface.toInt(0)-1;
				unsigned fat=pface.toInt(1)-1;
				unsigned fan=pface.toInt(2)-1;

				pface=pline[2];
				unsigned fbv=pface.toInt(0)-1;
				unsigned fbt=pface.toInt(1)-1;
				unsigned fbn=pface.toInt(2)-1;

				pface=pline[3];
				unsigned fcv=pface.toInt(0)-1;
				unsigned fct=pface.toInt(1)-1;
				unsigned fcn=pface.toInt(2)-1;

				if(fav>=verts.size() || fbv>=verts.size() || fcv>=verts.size())
					{
					LOG_ERROR("Model.loadObj: Bledny lub niezdefiniowany indeks wierzcholka [\"%s\":%u]", path.c_str(), i);
					LOG_ERROR("Model.loadObj: \"%s\"", pline.get().c_str());
					LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
					return false;
					}

				if(fat>=uvs.size() || fbt>=uvs.size() || fct>=uvs.size())
					{
					if(!onceft)
						{
						onceft=true;
						LOG_WARNING("Model.loadObj: Bledne lub niezdefiniowane wspolrzedne tekstury [\"%s\":%u]", path.c_str(), i);
						LOG_WARNING("Model.loadObj: \"%s\"", pline.get().c_str());
						LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
						}
					}

				if(fan>=normals.size() || fbn>=normals.size() || fcn>=normals.size())
					{
					if(!oncefn)
						{
						oncefn=true;
						LOG_WARNING("Model.loadObj: Bledny lub niezdefiniowany indeks wektora normalnego [\"%s\":%u]", path.c_str(), i);
						LOG_WARNING("Model.loadObj: \"%s\"", pline.get().c_str());
						LOG_DEBUG("Model.loadObj: [verts %u][uvs %u][normals %u]", verts.size(), uvs.size(), normals.size());
						}
					}

				faces.push_back({fav, fat, fan, fbv, fbt, fbn, fcv, fct, fcn});
				}
			}


		LOG_DEBUG("Model.loadObj: Zwalnianie pamieci");
		delete [] data;

		return true;
		};

	auto loadModel=[this, &verts, &uvs, &normals, &faces]()->bool
		{
//		LOG_DEBUG("Model.loadModel: Usrednianie normali");
//		std::vector<Math::Vector> normalAvg;
//		std::vector<int> normalAvgCount;
//
//		normalAvg.resize(verts.size());
//		normalAvgCount.resize(verts.size());
//
//		// Zerowanie
//		for(unsigned i=0u; i<normalAvgCount.size(); ++i)
//			{
//			normalAvgCount[i]=0;
//			}
//
//		auto normAvg=[normals](int vid, int nid, std::vector<Math::Vector>& normalAvg, std::vector<int>& normalAvgCount)
//			{
//			normalAvg[vid]+=normals[nid];
//			normalAvgCount[vid]++;
//			};
//
//		// Usrednianie normali
//		for(unsigned i=0u; i<faces.size(); ++i)
//			{
//			const Face& f=faces[i];
//
//			normAvg(f.av, f.an, normalAvg, normalAvgCount);
//			normAvg(f.bv, f.bn, normalAvg, normalAvgCount);
//			normAvg(f.cv, f.cn, normalAvg, normalAvgCount);
//			}
//
//		for(unsigned i=0u; i<normalAvg.size(); ++i)
//			{
//			normalAvg[i]=Math::VectorNormalize(normalAvg[i]*1.0f/normalAvgCount[i]);
//			}
//
//		normalAvgCount.clear();

		LOG_DEBUG("Model.loadModel: Wgrywanie trojkatow [faces %u]", faces.size());
		// Wpisywanie modelu do VBO
		for(unsigned i=0u; i<faces.size(); ++i)
			{
			const Face& f=faces[i];

			Vertex a(verts[f.av].x, verts[f.av].y, verts[f.av].z,  0, 0,  0, 0, 0,  0, 0, 0);
			Vertex b(verts[f.bv].x, verts[f.bv].y, verts[f.bv].z,  0, 0,  0, 0, 0,  0, 0, 0);
			Vertex c(verts[f.cv].x, verts[f.cv].y, verts[f.cv].z,  0, 0,  0, 0, 0,  0, 0, 0);

			if(f.at<uvs.size() && f.bt<uvs.size() && f.ct<uvs.size())
				{
				a.tx=uvs[f.at].x;
				a.ty=uvs[f.at].y;
				b.tx=uvs[f.bt].x;
				b.ty=uvs[f.bt].y;
				c.tx=uvs[f.ct].x;
				c.ty=uvs[f.ct].y;
				}

//			a.nx=normalAvg[f.av].x;
//			a.ny=normalAvg[f.av].y;
//			a.nz=normalAvg[f.av].z;
//			b.nx=normalAvg[f.bv].x;
//			b.ny=normalAvg[f.bv].y;
//			b.nz=normalAvg[f.bv].z;
//			c.nx=normalAvg[f.cv].x;
//			c.ny=normalAvg[f.cv].y;
//			c.nz=normalAvg[f.cv].z;
			a.nx=normals[f.an].x;
			a.ny=normals[f.an].y;
			a.nz=normals[f.an].z;
			b.nx=normals[f.bn].x;
			b.ny=normals[f.bn].y;
			b.nz=normals[f.bn].z;
			c.nx=normals[f.cn].x;
			c.ny=normals[f.cn].y;
			c.nz=normals[f.cn].z;

			// Prostopadle do normali
			if(uvs.size()>0u)
				{
				const Math::Vector distab=verts[f.bv]-verts[f.av];
				const Math::Vector distac=verts[f.cv]-verts[f.av];
				const Math::Vector distUVab=uvs[f.bt]-uvs[f.at];
				const Math::Vector distUVac=uvs[f.ct]-uvs[f.at];
				const float R=1.0f/(distUVab.x*distUVac.y-distUVab.y*distUVac.x);
				const Math::Vector tangent=-(distab*distUVac.y-distac*distUVab.y)*R;

				a.ntx=tangent.x;
				a.nty=tangent.y;
				a.ntz=tangent.z;
				b.ntx=tangent.x;
				b.nty=tangent.y;
				b.ntz=tangent.z;
				c.ntx=tangent.x;
				c.nty=tangent.y;
				c.ntz=tangent.z;
				}

			// Wpisanie do VBO
			vbo.add(a);
			vbo.add(b);
			vbo.add(c);
			}

		return true;
		};

	std::string cachePath=(std::string)"cache/"+path;

	if(IO::Resource::find(cachePath) && vbo.load(cachePath))
		{
		LOG_SUCCESS("Model.load: Wczytano model z cache \"%s\"", cachePath.c_str());

		if(!loadObj(path, true))
			{
			LOG_ERROR("Model.load: Nie udalo sie wczytac materialu \"%s\"", path.c_str());
			return false;
			}
		}
	else
		{
		if(!loadObj(path, false))
			{
			LOG_ERROR("Model.load: Nie udalo sie wczytac modelu \"%s\"", path.c_str());
			return false;
			}

		LOG_DEBUG("Model.load: [verts %u][uvs %u][normals %u][faces %u]", verts.size(), uvs.size(), normals.size(), faces.size());

		if(!loadModel())
			{
			LOG_ERROR("Model.load: Nie udalo sie wczytac modelu \"%s\" (plik .obj wczytany poprawnie)", path.c_str());
			return false;
			}

		if(!vbo.save(cachePath))
			{
			LOG_WARNING("Model.load: Nie udalo sie dodac modelu do cache \"%s\"", cachePath.c_str());
			}
		}

	if(!vbo.finalize())
		{
		return false;
		}

	if(!texDiffuse)
		{
		shader=ShaderPtr("shader/default_model_no-texture.glsl");
		}

	if(!shader)
		{
		LOG_WARNING("Model.load: Nie zdefiniowano shadera, wczytywanie domyslnego");
		shader=ShaderPtr(RENDER_DEFAULT_SHADER_MODEL);

		if(!shader)
			{
			LOG_ERROR("Model.load: Nie udalo sie wczytac shadera modelu");
			return false;
			}
		}

	LOG_INFO("Model.load: Inicjalizacja bufora materialu...");

	//GLenum err;

	glGenBuffers(1, &uboid);

	if(!uboid)
		{
		LOG_ERROR("Model.load: Nie udalo sie zainicjowac UBO [GLid: %u]", uboid);
		LOG_ERROR("Model.load: Blad: %s", gluErrorString(glGetError()));
		return false;
		}

	glBindBuffer(GL_UNIFORM_BUFFER, uboid);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), &material, GL_DYNAMIC_DRAW);

	if(texDiffuse)
		shader.uniform("u_texture", texDiffuse);
	if(texNormal)
		shader.uniform("u_normal", texNormal);

	LOG_SUCCESS("Model.load: Wczytano model \"%s\"", path.c_str());

	return true;
	}

void Model::clear()
	{
	vbo.clear();
	shader=nullptr;
	texDiffuse=nullptr;
	texNormal=nullptr;
	}

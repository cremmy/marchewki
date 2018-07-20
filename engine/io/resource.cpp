/*
 * resource.cpp
 *
 *  Created on: 23-12-2011
 *      Author: crm
 */

#include "resource.h"

#include <cstring>
#include <fstream>
#include <map>
#include <sys/stat.h>
#include <vector>

#include "listdir.h"
#include "resourcefile.h"

#include "../debug/log.h"
#include "../utils/ini.h"

namespace Engine
	{
	namespace IO
		{
		namespace Resource
			{
			unsigned size=0u;

			std::vector<ResourceFile *> resfiles;			// Tu beda wszystkie resfile, znikad indziej nie kasowac
			std::map<std::string, ResourceFile *> mresfiles;	// Mapa sciezka do pliku -> resfile		// Do wyszukiwania z podana sciezka do resfile (sciezka/do/resfile.crs:plik/w/resfile.png)

			std::vector<ResourceFile *> vfs;
			std::map<std::string, ResourceFile *> mvfs;		// Mapa sciezka do pliku -> resfile		// Do wyszukiwania bez sciezki do resfile

			ResourceFile* findResourceFileWith(const std::string& path);

			bool findRes(const std::string& path);
			bool findRaw(const std::string& path);

			char* readRaw(const std::string& file);
			}

		namespace Resource
			{
			bool open(const std::string& path)
				{
				ResourceFile *r;

				if(mresfiles.find(path)!=mresfiles.end())
					return true;

				try
					{
					r=new ResourceFile();
					}
				catch(std::bad_alloc&)
					{
					LOG_ERROR("Resource.include: Nie udalo zajac pamieci na resfile (\"%s\")", path.c_str());
					return false;
					}

				if(!r->open(path))
					{
					LOG_ERROR("Resource.open: Nie udalo sie wczytac pliku \"%s\"", path.c_str());

					delete r;
					return false;
					}

				resfiles.push_back(r);
				mresfiles[path]=r;

				//LOG_SUCCESS("Resource.open: Wczytano \"%s\"", path.c_str());

				return true;
				}

			void clear()
				{
				LOG_INFO("Resource.clear: Czyszczenie, %u resfile", resfiles.size());
				for(std::vector<ResourceFile *>::iterator it=resfiles.begin(); it!=resfiles.end(); ++it)
					delete *it;

				resfiles.clear();
				mresfiles.clear();
				vfs.clear();
				mvfs.clear();
				LOG_SUCCESS("Resource.clear: Wyczyszczono");
				}
			}

		namespace Resource
			{
			bool include(const std::string& path)
				{
				if(!open(path))
					return false;

				ResourceFile *r=mresfiles[path];

				vfs.push_back(r);
				mvfs[path]=r;

				return true;
				}

			bool includeDir(const std::string& path)
				{
				std::vector<std::string> files;
				std::string file;

				if(listdir(path, files)<1)
					{
					LOG_ERROR("Resource.includeDir: Nie znaleziono folderu \"%s\" lub jest on pusty", path.c_str());
					return false;
					}

				for(unsigned int i=2; i<files.size(); ++i)
					{
					file=path+"/"+files[i];

					if(file.length()<3 || file.compare(file.length()-3, 3, "crs")!=0)
						continue;

					if(!include(file))
						continue;
					}

				return true;
				}

			bool includeList(const std::string& name)
				{
				Utils::Ini ini;

				if(!ini.open(name, "list"))
					{
					LOG_ERROR("Resource.includeList: Nie udalo sie wczytac pliku listy \"%s\"", name.c_str());
					return false;
					}

				unsigned i;
				char key[8];
				std::string file;

				i=0;

				while(true)
					{
					sprintf(key, "%u", i);
					ini.get(key, file);

					if(file.length()<2)
						break;

					/*for(unsigned pos=file.find('\r'); pos!=file.npos; pos=file.find('\r'))
						file.erase(pos, 1);
					for(unsigned pos=file.find('\n'); pos!=file.npos; pos=file.find('\n'))
						file.erase(pos, 1);*/

					if(!include(file))
						return false;

					i++;
					}

				LOG_INFO("Resource.includeList: Wczytano %d resfile", i);

				return true;
				}
			}

		namespace Resource
			{
			ResourceFile* findResourceFileWith(const std::string& path)
				{
				ResourceFile *r;

				unsigned colon=path.find(':');
				if(colon!=path.npos && colon>0u)
					{
					const std::string res=path.substr(0, colon);
					const std::string file=path.substr(colon+1, path.length()-colon-1);

					if(mresfiles.find(res)==mresfiles.end())
						if(!open(res))
							return NULL;

					r=mresfiles[res];

					if(r->find(file))
						return r;
					return NULL;
					}

				for(std::vector<ResourceFile *>::iterator it=vfs.begin(); it!=vfs.end(); ++it)
					{
					r=*it;

					if(r->find(path))
						return r;
					}

				return NULL;
				}

			bool findRes(const std::string& path)
				{
				return findResourceFileWith(path);
				}

			bool findRaw(const std::string& path)
				{
				struct stat buff;						// http://stackoverflow.com/a/12774387

				if(path.find(':', 0)!=path.npos)
					return false;

				return !stat(path.c_str(), &buff);		// Zwraca 0 w razie braku bledu - gdy plik istnieje
				}

			bool find(const std::string& path)
				{
				return findRes(path) || findRaw(path);
				}
			}

		namespace Resource
			{
			char* load(const std::string& path)
				{
				ResourceFile *r;

				size=0u;

				r=findResourceFileWith(path);
				if(r)
					{
					char *data;
					unsigned colon;

					colon=path.find(':');
					if(colon!=path.npos)
						data=r->load( path.substr(colon+1, path.length()-colon-1) );
					else
						data=r->load(path);

					size=r->getLastSize();
					return data;
					}

				if(findRaw(path))
					return readRaw(path);

				return NULL;
				}

			char* readRaw(const std::string& file)
				{
				std::fstream in;
				char *data;

				in.open(file.c_str(), std::ios::in|std::ios::binary);
				if(!in)
					{
					LOG_ERROR("Resource.readRaw: Nie udalo sie otworzyc pliku \"%s\"", file.c_str());
					return NULL;
					}

				in.seekg(0, std::ios::end);
				size=in.tellg();
				in.seekg(0, std::ios::beg);

				try
					{
					data=new char[size+1];
					}
				catch(std::bad_alloc&)
					{
					LOG_ERROR("Resource.readRaw: Nie udalo sie zajac miejsca na plik \"%s\"", file.c_str());
					return NULL;
					}

				in.read(data, size);
				data[size]='\0';

				return data;
				}

#ifndef RESOURCEFILE_NO_SDL_RWOPS
			SDL_RWops* getRW(const std::string& path)
				{
				ResourceFile *r=findResourceFileWith(path);

				if(r)
					{
					LOG_DEBUG("Resource.getRW: Resfile");

					unsigned colon;

					colon=path.find(':');
					if(colon!=path.npos)
						return r->getRW( path.substr(colon+1, path.length()-colon-1) );
					else
						return r->getRW(path);
					}

				LOG_DEBUG("Resource.getRW: File");
				return SDL_RWFromFile(path.c_str(), "r");
				}
#endif
			}


		namespace Resource
			{
			void symlinkAdd(const std::string& name, const std::string& target)
				{
				if(mresfiles.find(name)!=mresfiles.end())
					LOG_INFO("Resource.symlinkAdd: Zastepowanie \"%s\"", name.c_str());

				if(mresfiles.find(target)==mresfiles.end())
					{
					if(!open(target))
						{
						LOG_ERROR("Resource.symlinkAdd: Nie udalo sie wczytac \"%s\"", target.c_str());
						return;
						}
					}

				mresfiles[name]=mresfiles[target];

				LOG_DEBUG("Resource.symlinkAdd: %s -> %s", name.c_str(), target.c_str());
				}

			void symlinkDel(const std::string& name)
				{
				if(mresfiles.find(name)!=mresfiles.end())
					mresfiles.erase(name);
				}
			}


		unsigned Resource::getSize(const std::string& file)
			{
			size=0u;

			return 0u;
			}

		/*unsigned Resource::getSizeRaw(const std::string& file)
			{
			std::fstream in;

			in.open(file.c_str(), std::ios::in|std::ios::binary);
			if(!in)
				return 0u;

			in.seekg(0, std::ios::end);
			rsize=in.tellg();
			in.close();

			return rsize;
			}*/

		/*unsigned Resource::getSizeRes(const std::string& file)
			{
			for(std::map<std::string, ResourceFile *>::iterator it=mresfiles.begin(); it!=mresfiles.end(); ++it)
				{
				ResourceFile *res=it->second;
				if(res->find(file))
					{
					//printf("Znaleziono plik %s w resfile %s, wielkosc resfile: %u, pliku: %u\n", file.c_str(), res->getCurrFile(), res->getSize(), res->getSize(file));
					rsize=res->getSize();
					return res->getSize(file);
					}
				}
			return readRawSize(file);
			}*/


		unsigned Resource::getSize()
			{
			return size;
			}
		}
	}

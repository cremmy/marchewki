/*
 * resourcefile.cpp
 *
 *  Created on: 7 sty 2017
 *      Author: crm
 */

#include "resourcefile.h"

#include <cstring>
#include <zlib.h>

#include "../debug/log.h"
#include "../utils/stringparser.h"

using namespace Engine;
using namespace Engine::IO;

const char FILESTAMP[]="CRES";
const char VERSION=2;

bool ResourceFile::open(const std::string& res)
	{
	clear();

	in.open(res.c_str(), std::ios::in|std::ios::binary);

	if(!in)
		{
		LOG_ERROR("ResourceFile.open: Nie udalo sie otworzyc pliku \"%s\"", res.c_str());
		return false;
		}

	respath=res;

	/**** Sprawdzenie naglowka i wersji ****/
		{
		const unsigned IDENT_LENGTH=strlen(FILESTAMP);

		char ident[IDENT_LENGTH+1];
		char version;

		in.seekg(0, std::ios::end);
		rsize=in.tellg();
		in.seekg(0, std::ios::beg);

		if(rsize<IDENT_LENGTH+1u)	// + bajt na wersje
			{
			LOG_ERROR("ResourceFile.open: Problem z wczytaniem naglowka pliku \"%s\" - plik za maly", res.c_str());
			return false;
			}

		in.read(ident, IDENT_LENGTH);
		in.read(&version, 1);

		if(in.fail())
			{
			LOG_ERROR("ResourceFile.open: Problem z wczytaniem naglowka pliku \"%s\"", res.c_str());
			return false;
			}

		ident[IDENT_LENGTH]='\0';

		if(strcmp(ident, FILESTAMP)!=0)
			{
			LOG_ERROR("ResourceFile.open: Plik \"%s\" nie jest poprawnym plikiem zasobow", res.c_str());
			return false;
			}

		if(version!=VERSION)
			{
			LOG_ERROR("ResourceFile.open: Nieobslugiwana wersja pliku zasobow w \"%s\"", res.c_str());
			return false;
			}
		}

	/**** Wczytanie naglowka ****/
	unsigned hsize, count;
	in.read(reinterpret_cast<char*>(&hsize), sizeof(unsigned));
	in.read(reinterpret_cast<char*>(&count), sizeof(unsigned));
	header.reserve(count);

	Item item;
	unsigned nlen;
	std::vector<char> name;

	for(unsigned int i=0; i<count; i++)
		{
		in.read((char*)&item.start, sizeof(unsigned));
		in.read((char*)&item.size, sizeof(unsigned));
		in.read(&item.flags, sizeof(char));

		if(item.flags&COMPRESSED)
			in.read((char*)&item.ucsize, sizeof(unsigned));
		else
			item.ucsize=item.size;

		in.read(reinterpret_cast<char*>(&nlen), sizeof(unsigned));
		name.resize(nlen+1);
		in.read(name.data(), nlen);
		name[nlen]='\0';

		int fname;
		for(fname=nlen-1; fname>=0 && name[fname]!='/'; --fname)

		item.path=name.data();
		item.name=name.data()+fname+1;
		item.start+=hsize;

		//Log::debug("ResourceFile.open: \"%s\" (%s); start %u, size %u, ucsize %u, flags 0x%02X", item.path.c_str(), item.name.c_str(), item.start, item.size, item.ucsize, item.flags);

		if(in.fail())
			{
			LOG_ERROR("ResourceFile.open: Problem z wczytaniem listy plikow z pliku \"%s\"", res.c_str());
			return false;
			}

		header.push_back(new Item(item));
		treeAddItem(item.path, header.back());
		}

	size=0u;

	return true;
	}

void ResourceFile::treeAddItem(const std::string& path, Item* item)
	{
	Utils::StringParser pars(path, "/");
	TreeItem *leaf;
	TreeItem nleaf;

	leaf=&root;

	for(unsigned i=0u; i<pars.count()-1u; ++i)
		{
		TreeItem *tmp=treeFindDirectory(pars[i], leaf);
		if(tmp)
			leaf=tmp;
		else
			{
			nleaf.name=pars[i];
			nleaf.type=TreeItem::DIRECTORY;
			leaf->items.push_back(nleaf);
			leaf=&leaf->items.back();
			}
		}

	nleaf.name=pars[pars.count()-1];
	nleaf.type=TreeItem::FILE;
	nleaf.item=item;
	leaf->items.push_back(nleaf);
	}

ResourceFile::TreeItem* ResourceFile::treeFind(const std::string& name, TreeItem* leaf, TreeItem::Type type)
	{
	for(unsigned i=0; i<leaf->items.size(); ++i)
		{
		if(leaf->items[i].type!=type)
			continue;
		if(leaf->items[i].name.compare(name)==0)
			return &leaf->items[i];
		}

	return nullptr;
	}

void ResourceFile::clear()
	{
	if(in.is_open())
		in.close();

	//curr=std::string();
	size=0u;
	rsize=0u;

	for(std::vector<Item *>::iterator it=header.begin(); it!=header.end(); ++it)
		delete *it;
	header.clear();

	root.type=TreeItem::DIRECTORY;
	root.name=std::string();
	root.items.clear();
	}

char* ResourceFile::load(Item *entry)
	{
	char *data;
	char *cdata;

	try
		{
		data=new char[entry->size+1];
		}
	catch(const std::bad_alloc &)
		{
		return nullptr;
		}

	in.seekg(entry->start);
	in.read(data, entry->size);
	size=entry->size;

	if(entry->flags&COMPRESSED)
		{
		int ret;

		cdata=data;
		size=entry->ucsize;

		try
			{
			data=new char[entry->ucsize+1];
			}
		catch(const std::bad_alloc& e)
			{
			delete [] cdata;
			return nullptr;
			}

		ret=uncompress((Bytef*)data, (uLongf*)&entry->ucsize, (Bytef*)cdata, entry->size);
		delete [] cdata;

		switch(ret)
			{
			case Z_MEM_ERROR:
				LOG_ERROR("ResourceFile.load: Za malo pamieci do rozpakowania pliku \"%s\"", entry->name.c_str());
			break;

			case Z_BUF_ERROR:
				LOG_ERROR("ResourceFile.load: Bufor wyjsciowy jest za maly \"%s\"", entry->name.c_str());
			break;

			case Z_DATA_ERROR:
				LOG_ERROR("ResourceFile.load: Problem z rozpakowaniem danych wejsciowych \"%s\"", entry->name.c_str());
			break;

			default:
			case Z_OK:
				LOG_SUCCESS("ResourceFile.load: Rozpakowano plik \"%s\" (%uB -> %uB)", entry->name.c_str(), entry->size, entry->ucsize);
			break;
			}

		if(ret!=Z_OK)
			{
			delete [] data;
			return nullptr;
			}
		}

	data[size]='\0';

	//Log::debug("ResourceFile.load: Wczytano plik \"%s\", wielkosc: %u; skompresowany: %d (wielkosc po dekompresji: %u)", entry->name.c_str(), entry->size, entry->flags&COMPRESSED, entry->ucsize);

	return data;
	}

ResourceFile::Item* ResourceFile::findItem(const std::string& path)
	{
	/*for(unsigned i=0; i<header.size(); ++i)
		if(header[i]->path.compare(path)==0)
			return header[i];
	return nullptr;*/

	Utils::StringParser pars(path, "/");

	TreeItem *leaf=&root;

	for(unsigned i=0u; i<pars.count()-1u; ++i)
		{
		leaf=treeFindDirectory(pars[i], leaf);

		if(!leaf)
			return nullptr;
		}

	leaf=treeFindFile(pars[pars.count()-1], leaf);

	if(leaf)
		return leaf->item;
	return nullptr;
	}

char* ResourceFile::load(const std::string& res, const std::string& file)
	{
	if(!open(res))
		return nullptr;

	return load(file);
	}

char* ResourceFile::load(const std::string& path)
	{
	/*StringParser pars;

	pars.setDelimiter(":");
	pars.setText(path);

	if(pars.count()<2)
		return load(curr, path);
	return load(pars[0], pars[1]);*/

	Item *item;

	item=findItem(path);

	if(item)
		return load(item);
	return nullptr;
	}

char* ResourceFile::data(const std::string& res)
	{
	//rsize=0u;
	if(!open(res))
		return nullptr;

	char *data;

	try
		{
		data=new char[rsize];
		}
	catch(const std::bad_alloc &)
		{
		return nullptr;
		}

	std::fstream in;

	in.open(res.c_str(), in.in|in.binary);
	in.read(data, rsize);
	in.close();

	size=rsize;

	return data;
	}


bool ResourceFile::find(const std::string& path)
	{
	Utils::StringParser pars(path, "/");

	if(pars.count()>1u)
		{
		open(pars[0]);
		return findItem(pars[1]);
		}

	return findItem(path);
	}


unsigned ResourceFile::getSize(const std::string& path)
	{
	Utils::StringParser pars(path, "/");
	Item *item;

	if(pars.count()>1u)
		{
		open(pars[0]);
		item=findItem(pars[1]);
		}
	else
		item=findItem(path);

	if(item)
		return item->ucsize;
	return 0u;
	}


const std::string& ResourceFile::getFileName(unsigned int num)
	{
	return header[num]->name;
	}



/*****************************************************************************/
/**** SDL_RWops **************************************************************/
/*****************************************************************************/
#ifndef RESOURCEFILE_NO_SDL_RWOPS
struct ResRWopData
	{
	std::fstream in;
	unsigned offset;
	unsigned size;
	};

Sint64 RESF_RW_size(SDL_RWops* rw);
Sint64 RESF_RW_seek(SDL_RWops* rw, Sint64 offset, int whence);
size_t  RESF_RW_read(SDL_RWops* rw, void *ptr, size_t size, size_t maxnum);
int RESF_RW_close(SDL_RWops* rw);

SDL_RWops* ResourceFile::getRW(const std::string& path)
	{
	Item *item;

	item=findItem(path);

	if(!item)
		return nullptr;

	if(item->flags&Flags::COMPRESSED)
		{
		LOG_ERROR("ResourceFile.getRWops: Plik \"%s\" jest skompresowany");
		return nullptr;
		}

	SDL_RWops* rw=SDL_AllocRW();

	if(!rw)
		{
		LOG_ERROR("ResourceFile.getRWops: Nie udalo sie utworzyc RWopsow: %s", SDL_GetError());
		return nullptr;
		}

	rw->type=SDL_RWOPS_UNKNOWN;
	rw->size=RESF_RW_size;
	rw->seek=RESF_RW_seek;
	rw->read=RESF_RW_read;
	rw->write=nullptr;
	rw->close=RESF_RW_close;

	ResRWopData* rwdata=new ResRWopData();
	rwdata->in.open(this->respath.c_str(), std::ios::in|std::ios::binary);

	if(!rwdata->in.is_open())
		{
		delete rwdata;
		SDL_FreeRW(rw);

		return nullptr;
		}

	rwdata->in.seekg(item->start, std::ios::beg);

	rwdata->offset=item->start;
	rwdata->size=item->ucsize;

	rw->hidden.unknown.data1=rwdata;

	return rw;
	}

Sint64 RESF_RW_size(SDL_RWops* rw)
	{
	ResRWopData* rwdata=(ResRWopData*)rw->hidden.unknown.data1;

	//LOG_DEBUG("RESF_RW_size: %d", rwdata->size);

	return rwdata->size;
	}

Sint64 RESF_RW_seek(SDL_RWops* rw, Sint64 offset, int whence)
	{
	ResRWopData* rwdata=(ResRWopData*)rw->hidden.unknown.data1;

	switch(whence)
		{
		case RW_SEEK_CUR:
			rwdata->in.seekg(offset, std::ios::cur);
		break;

		case RW_SEEK_END:
			rwdata->in.seekg(rwdata->offset+rwdata->size+offset, std::ios::beg);
		break;

		default:
		case RW_SEEK_SET:
			rwdata->in.seekg(rwdata->offset+offset, std::ios::beg);
		break;
		}

	//LOG_DEBUG("RESF_RW_seek: offset %d dir %d; tell %d, %d", offset, whence, (int)rwdata->in.tellg(), (int)rwdata->in.tellg()-(int)rwdata->offset);

	return (Sint64)rwdata->in.tellg()-(Sint64)rwdata->offset;
	}

size_t  RESF_RW_read(SDL_RWops* rw, void *ptr, size_t size, size_t maxnum)
	{
	ResRWopData* rwdata=(ResRWopData*)rw->hidden.unknown.data1;

	//LOG_DEBUG("RESF_RW_read: %d %d, eof %d", size, maxnum, rwdata->in.eof());

	if(rwdata->in.eof())
		return 0;

	rwdata->in.read((char*)ptr, size*maxnum);

	//LOG_DEBUG("RESF_RW_read: %d %d, eof %d", size, maxnum, rwdata->in.eof());

	return rwdata->in.gcount()/size;
	}

int RESF_RW_close(SDL_RWops* rw)
	{
	ResRWopData* rwdata=(ResRWopData*)rw->hidden.unknown.data1;

	rwdata->in.close();

	delete rwdata;

	return 0;
	}

#endif

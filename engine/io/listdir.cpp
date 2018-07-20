#include "listdir.h"

#include <dirent.h>
#include <sys/stat.h>

int Engine::IO::listdir(const std::string& dir, std::vector<std::string> &files)
	{
	DIR *dp;
	struct dirent *dirp;

	if((dp=opendir(dir.c_str()))==NULL)
		{
		//printf("FAIL: ListDir: Nie udalo sie otworzyc folderu\n");
		return 0;
		}

	int i=0;
	while((dirp=readdir(dp))!=NULL)
		{
		files.push_back( std::string(dirp->d_name) );
		i++;
		}
	closedir(dp);
	return i;
	}

int Engine::IO::listdirw(const std::wstring& dir, std::vector<std::wstring> &files)
	{
	_WDIR *dp;
	struct _wdirent *dirp;

	if((dp=_wopendir(dir.c_str()))==NULL)
		return 0;

	int i=0;
	while((dirp=_wreaddir(dp))!=NULL)
		{
		files.push_back( std::wstring(dirp->d_name) );
		i++;
		}
	_wclosedir(dp);
	return i;
	}

unsigned Engine::IO::filemtime(const std::string& file)
	{
	struct stat s;
	if(stat(file.c_str(), &s))
		return 0;
	return s.st_mtime;
	}

unsigned Engine::IO::filemtimew(const std::wstring& file)
	{
	struct _stat s;
	if(_wstat(file.c_str(), &s))
		return 0;
	return s.st_mtime;
	}

unsigned Engine::IO::filectime(const std::string& file)
	{
	struct stat s;
	if(stat(file.c_str(), &s))
		return 0;
	return s.st_ctime;
	}

unsigned Engine::IO::filectimew(const std::wstring& file)
	{
	struct _stat s;
	if(_wstat(file.c_str(), &s))
		return 0;
	return s.st_ctime;
	}

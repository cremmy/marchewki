#pragma once

#include <string>
#include <vector>

namespace Engine
	{
	namespace IO
		{
		int listdir(const std::string& dir, std::vector<std::string> &files);		// Zwraca ilosc plikow w folderze
		int listdirw(const std::wstring& dir, std::vector<std::wstring> &files);
		unsigned filemtime(const std::string& file);
		unsigned filemtimew(const std::wstring& file);
		unsigned filectime(const std::string& file);
		unsigned filectimew(const std::wstring& file);
		}
	}

#ifndef MHFILESYSTEM_HPP
#define MHFILESYSTEM_HPP
#include "MhIoSystem.hpp"
namespace MH33 {
namespace Io {

class Filesystem : public System
{
public:
	Filesystem();
	Device* open(const char* path, Mode mode);
	bool exists(const char* path);
	char separator() const;
	void enumerate(const char* path, bool withPath, std::vector<std::string>& output);
	void enumerate(const char* path, bool withPath, FilenameCallback functor);
	void enumerate(const char* path, bool withPath, FilenameCallbackPP functor);
	void enumerate(const char* path, FilesystemCallback functor);
	void enumerate(const char* path, FilesystemCallbackPP functor);
	bool isDirectory(const char* path);
	bool isSymlink(const char* path);
	bool isFile(const char* path);
	bool mkdir(const char* dir);
	bool remove(const char* path);
};

}
}
#endif // MHFILESYSTEM_HPP

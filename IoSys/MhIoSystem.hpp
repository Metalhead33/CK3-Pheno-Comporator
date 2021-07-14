#ifndef MHIOSYSTEM_HPP
#define MHIOSYSTEM_HPP
#include <Io/MhIoDevice.hpp>
#include <functional>

namespace MH33 {
namespace Io {

class System {
public:
	typedef std::function<void(const char*)> FilenameCallback;
	typedef std::function<void(const std::string&)> FilenameCallbackPP;
	typedef std::function<void(System*, const char*, const char*)> FilesystemCallback; // First string is full path, second is just filename
	typedef std::function<void(System*, const std::string&, const std::string&)> FilesystemCallbackPP; // First string is full path, second is just filename
	virtual ~System() = default;
	// The actual functions to be implemented
	virtual Device* open(const char* path, Mode mode) = 0;
	virtual bool exists(const char* path) = 0;
	virtual char separator() const = 0;
	virtual void enumerate(const char* path, bool withPath, std::vector<std::string>& output) = 0;
	virtual void enumerate(const char* path, bool withPath, FilenameCallback functor) = 0;
	virtual void enumerate(const char* path, bool withPath, FilenameCallbackPP functor) = 0;
	virtual void enumerate(const char* path, FilesystemCallback functor) = 0;
	virtual void enumerate(const char* path, FilesystemCallbackPP functor) = 0;
	virtual bool isDirectory(const char* path) = 0;
	virtual bool isSymlink(const char* path) = 0;
	virtual bool isFile(const char* path) = 0;
	virtual bool mkdir(const char* dir) = 0;
	virtual bool remove(const char* path) = 0;

	// Convenience wrappers
	inline Device* open(const std::string& path, Mode mode) {
		return open(path.c_str(),mode);
	}
	inline bool exists(const std::string& path) {
		return exists(path.c_str());
	}
	inline void enumerate(const std::string& path, bool withPath, std::vector<std::string>& output) {
		enumerate(path.c_str(),withPath,output);
	}
	inline std::vector<std::string> enumerate(const char* path, bool withPath) {
		std::vector<std::string> tmp;
		enumerate(path,withPath,tmp);
		return tmp;
	}
	inline std::vector<std::string> enumerate(const std::string& path, bool withPath) {
		std::vector<std::string> tmp;
		enumerate(path,withPath,tmp);
		return tmp;
	}
	inline void enumerate(const std::string& path, bool withPath, FilenameCallback functor) {
		enumerate(path.c_str(),withPath,functor);
	}
	inline void enumerate(const std::string& path, bool withPath, FilenameCallbackPP functor) {
		enumerate(path.c_str(),withPath,functor);
	}
	inline void enumerate(const std::string& path, FilesystemCallback functor) {
		enumerate(path.c_str(),functor);
	}
	inline void enumerate(const std::string& path, FilesystemCallbackPP functor) {
		enumerate(path.c_str(),functor);
	}
	bool isDirectory(const std::string& path) {
		return isDirectory(path.c_str());
	}
	bool isSymlink(const std::string& path) {
		return isSymlink(path.c_str());
	}
	bool isFile(const std::string& path) {
		return isFile(path.c_str());
	}
	inline bool mkdir(const std::string& dir) {
		return mkdir(dir.c_str());
	}
	inline bool remove(const std::string& dir) {
		return remove(dir.c_str());
	}
};

}
}
#endif // MHIOSYSTEM_HPP

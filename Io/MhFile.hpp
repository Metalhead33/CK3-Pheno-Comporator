#ifndef MHFILEIO_HPP
#define MHFILEIO_HPP
#include "MhIoDevice.hpp"
#include <cstdio>
#include <string>

namespace MH33 {
namespace Io {

class File : public Device
{
private:
	FILE* fdev;
	Mode mode;
	File(const File& cpy) = delete; // No copy constructor
	File& operator=(const File& cpy) = delete; // No copy assignment operator
public:
	// Destructor
	~File();
	// Move constructor
	File(File&& mov);
	// Move assignment operator
	File& operator=(File&& mov);
	// Default constructor
	File();
	// Actual constructor
	File(const char* path, Mode mode);
	File(const std::string& path, Mode mode);
	// Actual imnplementation
	bool flush();
	bool seek(SeekOrigin whence, intptr_t offset);
	intptr_t tell();
	intptr_t size();
	size_t write(const void* data, size_t dataSize);
	size_t read(void* destination, size_t dataSize);
	Mode getMode() const;
};

}
}
#endif // MHFILEIO_HPP

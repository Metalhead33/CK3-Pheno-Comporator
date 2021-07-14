#ifndef MHSTDIN_HPP
#define MHSTDIN_HPP
#include "MhIoDevice.hpp"
#include <cstdio>

namespace MH33 {
namespace Io {

class Stdin : public Device
{
public:
	Stdin();
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
#endif // MHSTDIN_HPP

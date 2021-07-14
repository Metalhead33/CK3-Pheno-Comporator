#ifndef MHBUFFERWRAPPER_HPP
#define MHBUFFERWRAPPER_HPP
#include "MhIoDevice.hpp"

namespace MH33 {
namespace Io {

class BufferWrapper : public Device
{
private:
	Util::Buffer buffer;
	intptr_t cursor;
public:
	// Copy and move constructors and assignments - from other wrappers
	BufferWrapper(const BufferWrapper& cpy);
	BufferWrapper(BufferWrapper&& mov);
	BufferWrapper& operator=(const BufferWrapper& cpy);
	BufferWrapper& operator=(BufferWrapper&& mov);
	// Copy and move constructors - from other buffers
	BufferWrapper(const Util::Buffer& cpy);
	BufferWrapper(Util::Buffer&& mov);
	// Default constructor
	BufferWrapper();
	BufferWrapper(size_t sz);
	// Direct buffer access, in case we need it.
	Util::Buffer& getBuffer();
	const Util::Buffer& getBuffer() const;
	// Actual IO functions
	bool flush();
	bool seek(SeekOrigin whence, intptr_t offset);
	intptr_t tell();
	intptr_t size();
	size_t write(const void* data, size_t dataSize);
	size_t read(void* destination, size_t dataSize);
	Mode getMode() const;
	void yield(Util::Buffer& target);
};

}
}
#endif // MHBUFFERWRAPPER_HPP

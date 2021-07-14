#include "MhBufferWrapper.hpp"
namespace MH33 {
namespace Io {

const Util::Buffer &BufferWrapper::getBuffer() const
{
	return buffer;
}

bool BufferWrapper::flush()
{
	return true;
}

bool BufferWrapper::seek(SeekOrigin whence, intptr_t offset)
{
	switch (whence) {
	case SeekOrigin::CUR:
		cursor += offset;
		break;
	case SeekOrigin::END:
		cursor = buffer.size() - offset;
		break;
	case SeekOrigin::SET:
		cursor = offset;
		break;
	}
	if(cursor < 0) {
		cursor = 0;
		return false;
	} else
		if(cursor > buffer.size()) {
		cursor = buffer.size();
		return false;
	} else return true;
}

intptr_t BufferWrapper::tell()
{
	return cursor;
}

intptr_t BufferWrapper::size()
{
	return buffer.size();
}

size_t BufferWrapper::write(const void *data, size_t dataSize)
{
	if( buffer.capacity() < (cursor+dataSize) ) buffer.reserve(cursor+dataSize);
	for(size_t i = 0; i < dataSize; ++i) {
		if(cursor >= buffer.size()) buffer.push_back(static_cast<const std::byte*>(data)[i]);
		else buffer[cursor] = static_cast<const std::byte*>(data)[i];
		++cursor;
	}
	return dataSize;
}

size_t BufferWrapper::read(void *destination, size_t dataSize)
{
	size_t readData = 0;
	for(;readData < dataSize; ++readData) {
		if(cursor < buffer.size()) static_cast<std::byte*>(destination)[readData] = buffer[cursor];
		else break;
		++cursor;
	}
	return readData;
}

Mode BufferWrapper::getMode() const
{
	return Mode::READ_WRITE;
}

void BufferWrapper::yield(Util::Buffer &target)
{
	target = std::move(buffer);
}

Util::Buffer &BufferWrapper::getBuffer()
{
	return buffer;
}

BufferWrapper::BufferWrapper(const BufferWrapper &cpy)
	: buffer(cpy.buffer), cursor(cpy.cursor)
{

}

BufferWrapper::BufferWrapper(BufferWrapper &&mov)
	: buffer(std::move(mov.buffer)), cursor(mov.cursor)
{

}

BufferWrapper &BufferWrapper::operator=(const BufferWrapper &cpy)
{
	this->buffer = cpy.buffer;
	this->cursor = cpy.cursor;
	return *this;
}

BufferWrapper &BufferWrapper::operator=(BufferWrapper &&mov)
{
	this->buffer = std::move(mov.buffer);
	this->cursor = mov.cursor;
	return *this;
}

BufferWrapper::BufferWrapper(const Util::Buffer &cpy)
	: buffer(cpy), cursor(0)
{

}

BufferWrapper::BufferWrapper(Util::Buffer &&mov)
	: buffer(std::move(mov)), cursor(0)
{

}

BufferWrapper::BufferWrapper() : buffer(0), cursor(0)
{

}

BufferWrapper::BufferWrapper(size_t sz) : buffer(sz), cursor(0)
{

}
}
}

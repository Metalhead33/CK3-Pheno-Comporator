#include "MhStderr.hpp"
namespace MH33 {
namespace Io {

Stderr::Stderr()
{

}

bool Stderr::flush()
{
	return fflush(stderr) == 0;
}

bool Stderr::seek(SeekOrigin whence, intptr_t offset)
{
	(void)whence;
	(void)offset;
	return false;
}

intptr_t Stderr::tell()
{
	return -1;
}

intptr_t Stderr::size()
{
	return -1;
}

size_t Stderr::write(const void *data, size_t dataSize)
{
	return fwrite(data,1,dataSize,stderr);
}

size_t Stderr::read(void *destination, size_t dataSize)
{
	(void)destination;
	(void)dataSize;
	return 0; // Write-only stream
}

Mode Stderr::getMode() const
{
	return Mode::APPEND;
}

}
}

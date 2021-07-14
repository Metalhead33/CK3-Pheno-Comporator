#include "MhStdout.hpp"
namespace MH33 {
namespace Io {

Stdout::Stdout()
{

}


bool Stdout::flush()
{
	return fflush(stdout) == 0;
}

bool Stdout::seek(SeekOrigin whence, intptr_t offset)
{
	(void)whence;
	(void)offset;
	return false;
}

intptr_t Stdout::tell()
{
	return -1;
}

intptr_t Stdout::size()
{
	return -1;
}

size_t Stdout::write(const void *data, size_t dataSize)
{
	return fwrite(data,1,dataSize,stdout);
}

size_t Stdout::read(void *destination, size_t dataSize)
{
	(void)destination;
	(void)dataSize;
	return 0; // Write-only stream
}

Mode Stdout::getMode() const
{
	return Mode::APPEND;
}

}
}

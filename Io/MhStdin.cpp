#include "MhStdin.hpp"
namespace MH33 {
namespace Io {

Stdin::Stdin()
{

}

bool Stdin::flush()
{
	return fflush(stdin) == 0;
}

bool Stdin::seek(SeekOrigin whence, intptr_t offset)
{
	(void)whence;
	(void)offset;
	return false;
}

intptr_t Stdin::tell()
{
	return -1;
}

intptr_t Stdin::size()
{
	return -1;
}

size_t Stdin::write(const void *data, size_t dataSize)
{
	(void)data;
	(void)dataSize;
	return 0; // rEAD-only stream
}

size_t Stdin::read(void *destination, size_t dataSize)
{
	return fread(destination,1,dataSize,stdin);
}

Mode Stdin::getMode() const
{
	return Mode::READ;
}

}
}

#include "MhRNG.hpp"
#include "MhNormDenorm.hpp"
namespace MH33 {

namespace Util {
uint32_t RNG::getSeed() const
{
	return seed;
}

void RNG::setSeed(uint32_t value)
{
	seed = value;
}

RNG::RNG(uint32_t seed) : seed(seed)
{

}

uint32_t RNG::generate()
{
	seed += 0xe120fc15;
	uint64_t tmp;
	tmp = uint64_t(seed) * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = uint64_t(m1) * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

float RNG::generateFloat()
{
	return Util::fnormalize(generate());
}

double RNG::generateDouble()
{
	return Util::dnormalize(generate());
}

uint32_t RNG::generate(uint32_t max)
{
	return generate() % max;
}

float RNG::generateFloat(float max)
{
	return generateFloat() * max;
}

double RNG::generateDouble(double max)
{
	return generateDouble() * max;
}

uint32_t RNG::generate(uint32_t min, uint32_t max)
{
	const uint32_t range = max - min;
	return generate(range) + min;
}

float RNG::generateFloat(float min, float max)
{
	const float range = max - min;
	return generateFloat(range) + min;
}

double RNG::generateDouble(double min, double max)
{
	const double range = max - min;
	return generateDouble(range) + min;
}
}
}

#include "Noise.hpp"
#include "glm/glm.hpp"
#include <algorithm>
#include <cmath>
#include <random>

Noise::Noise(unsigned long seed, float frequency, float amplitude): _frequency(frequency), _amplitude(amplitude)
{
	_permutationTable.resize(PERMUTATION_TABLE_SIZE);
	for (int i = 0; i < PERMUTATION_TABLE_SIZE; ++i)
		_permutationTable[i] = i;
	std::shuffle(_permutationTable.begin(), _permutationTable.end(), std::default_random_engine(seed));
	_permutationTable.insert(_permutationTable.end(), _permutationTable.begin(), _permutationTable.end());
}

static glm::vec2 getConstantVector(int permutationValue)
{
	switch (permutationValue % 8)
	{
		case 0: return {1.0, 0.0};
		case 1: return {-1.0, 0.0};
		case 2: return {0.0, 1.0};
		case 3: return {0.0, -1.0};
		case 4: return {1.0, 1.0};
		case 5: return {-1.0, -1.0};
		case 6: return {1.0, -1.0};
		default: return {-1.0, 1.0};
	}
}

static float lerp(float t, float a1, float a2)
{
	return a1 + t * (a2 - a1);
}

static float fade(float t)
{
	return ((6 * t - 15) * t + 10) * t * t * t;
}

float Noise::_computeNoise(float x, float y) const
{
	const uint32_t X = static_cast<uint32_t>(std::floor(x)) & 255;
	const uint32_t Y = static_cast<uint32_t>(std::floor(y)) & 255;

	const float xf = x - std::floor(x);
	const float yf = y - std::floor(y);

	const glm::vec2 topRight(xf - 1.0, yf - 1.0);
	const glm::vec2 topLeft(xf, yf - 1.0);
	const glm::vec2 bottomRight(xf - 1.0, yf);
	const glm::vec2 bottomLeft(xf, yf);

	const int valueTopRight = _permutationTable[_permutationTable[X + 1] + Y + 1];
	const int valueTopLeft = _permutationTable[_permutationTable[X] + Y + 1];
	const int valueBottomRight = _permutationTable[_permutationTable[X + 1] + Y];
	const int valueBottomLeft = _permutationTable[_permutationTable[X] + Y];

	const float dotTopRight = glm::dot(topRight, getConstantVector(valueTopRight));
	const float dotTopLeft = glm::dot(topLeft, getConstantVector(valueTopLeft));
	const float dotBottomRight = glm::dot(bottomRight, getConstantVector(valueBottomRight));
	const float dotBottomLeft = glm::dot(bottomLeft, getConstantVector(valueBottomLeft));

	const float u = fade(xf);
	const float v = fade(yf);
	return (lerp(u, 
		lerp(v, dotBottomLeft, dotTopLeft), 
		lerp(v, dotBottomRight, dotTopRight)));
}

float Noise::getNoise(float x, float y, int octaveCount) const
{
	float result = 0;
	float frequency = _frequency;
	float amplitude = _amplitude;

	for (int octave = 0; octave < octaveCount; ++octave)
	{
		float n = amplitude * _computeNoise(x * frequency, y * frequency);
		result += n;
		frequency *= 2.0;
		amplitude *= 0.5;
	}
	return result;
}

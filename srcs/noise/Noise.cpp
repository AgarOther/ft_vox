#include "Noise.hpp"
#include "glm/glm.hpp"
#include <algorithm>
#include <cmath>
#include <random>

Noise::Noise(unsigned long seed, double frequency, double amplitude): _frequency(frequency), _amplitude(amplitude)
{
	_permutationTable.resize(PERMUTATION_TABLE_SIZE);
	for (int i = 0; i < PERMUTATION_TABLE_SIZE; ++i)
		_permutationTable.push_back(i);
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

static double lerp(double t, double a1, double a2)
{
	return a1 + t * (a2 - a1);
}

static double fade(double t)
{
	return ((6 * t - 15) * t + 10) * t * t * t;
}

double Noise::_computeNoise(double x, double y) const
{
	const int X = static_cast<int>(std::floor(x)) & 255;
	const int Y = static_cast<int>(std::floor(y)) & 255;

	const double xf = x - std::floor(x);
	const double yf = y - std::floor(y);

	const glm::vec2 topRight(xf - 1.0, yf - 1.0);
	const glm::vec2 topLeft(xf, yf - 1.0);
	const glm::vec2 bottomRight(xf - 1.0, yf);
	const glm::vec2 bottomLeft(xf, yf);

	const int valueTopRight = _permutationTable[_permutationTable[X + 1] + Y + 1];
	const int valueTopLeft = _permutationTable[_permutationTable[X] + Y + 1];
	const int valueBottomRight = _permutationTable[_permutationTable[X + 1] + Y];
	const int valueBottomLeft = _permutationTable[_permutationTable[X] + Y];

	const double dotTopRight = glm::dot(topRight, getConstantVector(valueTopRight));
	const double dotTopLeft = glm::dot(topLeft, getConstantVector(valueTopLeft));
	const double dotBottomRight = glm::dot(bottomRight, getConstantVector(valueBottomRight));
	const double dotBottomLeft = glm::dot(bottomLeft, getConstantVector(valueBottomLeft));

	const double u = fade(xf);
	const double v = fade(yf);
	return (lerp(u, 
		lerp(v, dotBottomLeft, dotTopLeft), 
		lerp(v, dotBottomRight, dotTopRight)));
}

double Noise::getNoise(double x, double y, int octaveCount) const
{
	double result = 0;
	double frequency = _frequency;
	double amplitude = _amplitude;

	for (int octave = 0; octave < octaveCount; ++octave)
	{
		double n = amplitude * _computeNoise(x * frequency, y * frequency);
		result += n;
		frequency *= 2.0;
		amplitude *= 0.5;
	}
	return result;
}

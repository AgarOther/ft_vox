#pragma once

#include <vector>

#define PERMUTATION_TABLE_SIZE 256
#define NOISIFY(x) std::round((((x + 1.0) / 2.0)))

class Noise
{
	public:
		Noise(unsigned long seed, float frequency, float amplitude);

		float				getNoise(float x, float y, int octaveCount) const;
	private:
		std::vector<int>	_permutationTable;
		float				_frequency;
		float				_amplitude;

		float				_computeNoise(float x, float z) const;
};

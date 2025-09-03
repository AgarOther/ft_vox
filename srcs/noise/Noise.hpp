#pragma once

#include <vector>

#define PERMUTATION_TABLE_SIZE 256
#define NOISIFY(x) std::round((((x + 1.0) / 2.0)))

class Noise
{
	public:
		Noise(unsigned long seed, double frequency, double amplitude);

		double				getNoise(double x, double y, int octaveCount) const;
	private:
		std::vector<int>	_permutationTable;
		double				_frequency;
		double				_amplitude;

		double				_computeNoise(double x, double z) const;
};

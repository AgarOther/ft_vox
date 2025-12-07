#pragma once

#include "ChunkWorker.hpp"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class PriorityWorker: public ChunkWorker
{
	public:
		PriorityWorker(Environment environment): ChunkWorker(environment) {}
		~PriorityWorker() {}

		bool	queue(const std::vector<Chunk * > & chunkQueue) override;
	private:
		void	_process() override;
};

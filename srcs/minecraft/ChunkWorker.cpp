#include "ChunkWorker.hpp"
#include "colors.hpp"
#include "utils.hpp"
#include <mutex>
#include <iostream>

uint8_t ChunkWorker::_count = 0;

ChunkWorker::ChunkWorker(Environment environment): _active(false), _working(false), _environment(environment)
{
	_id = ChunkWorker::_count++;
}

bool ChunkWorker::queue(const std::vector<Chunk * > & chunkQueue)
{
	const std::lock_guard<std::mutex> lg(_queueMutex);

	if (chunkQueue.empty())
		return false;

	_chunkQueue.reserve(chunkQueue.size());
	for (Chunk * chunk : chunkQueue)
	{
		if (chunk && chunk->getState() <= DIRTY)
			_chunkQueue.push_back(chunk);
	}
	return true;
}

void ChunkWorker::_process()
{
	const std::lock_guard<std::mutex> lg(_queueMutex);

	if (_chunkQueue.empty())
		return;

	_working = true;
	for (Chunk * chunk : _chunkQueue)
	{		
		if (chunk->getState() == IDLE)
			chunk->generateBlocks(_environment);
		else
			chunk->generateMesh();
	}
	_chunkQueue.clear();
	_working = false;
}

void ChunkWorker::_loop()
{
	while (_active)
	{
		_process();
		USLEEP(100);
	}
}

void ChunkWorker::start()
{
	std::cout << GREEN << "[CHUNK] Started ChunkWorker #" << (int)_id << " thread!" << RESET << std::endl;
	_active = true;
	_thread = std::thread(&ChunkWorker::_loop, this);
}

void ChunkWorker::stop()
{
	_active = false;
	_thread.join();
	std::cout << YELLOW << "[CHUNK] Stopped ChunkWorker #" << (int)_id << " thread!" << RESET << std::endl;
}

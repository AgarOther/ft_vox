#include "ChunkWorker.hpp"
#include "colors.hpp"
#include <mutex>
#include <unistd.h>
#include <iostream>

bool ChunkWorker::queue(std::vector<Chunk * > & chunkQueue)
{
	std::lock_guard<std::mutex> lg(_queueMutex);
	if (chunkQueue.empty())
		return false;

	for (Chunk * chunk : chunkQueue)
	{
		if (chunk->getState() == IDLE)
			_chunkQueue.push_back(chunk);
	}
	chunkQueue.clear();
	chunkQueue.shrink_to_fit();
	return true;
}

void ChunkWorker::_process()
{
	std::lock_guard<std::mutex> lg(_queueMutex);
	if (_chunkQueue.empty())
		return;

	_working = true;
	for (Chunk * chunk : _chunkQueue)
	{
		chunk->generateBlocks();
		chunk->generateMesh();
	}
	_chunkQueue.clear();
	_chunkQueue.shrink_to_fit();
}

void ChunkWorker::_loop()
{
	while (_active)
	{
		_process();
		usleep(100);
		_working = false;
	}
}

void ChunkWorker::start()
{
	_active = true;
	_thread = std::thread(&ChunkWorker::_loop, this);
	std::cout << GREEN << "[CHUNK] Started ChunkWorker thread!" << RESET << std::endl;
}

void ChunkWorker::stop()
{
	_active = false;
	_thread.join();
	std::cout << YELLOW << "[CHUNK] Stopped ChunkWorker thread!" << RESET << std::endl;
}

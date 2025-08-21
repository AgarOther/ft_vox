#include "ChunkWorker.hpp"
#include "colors.hpp"
#include <mutex>
#include <unistd.h>
#include <iostream>

bool ChunkWorker::queue(const std::vector<Chunk * > & chunkQueue)
{
	std::lock_guard<std::mutex> lg(_queueMutex);

	if (chunkQueue.empty())
		return false;

	_chunkQueue.reserve(chunkQueue.size());
	for (Chunk * chunk : chunkQueue)
		_chunkQueue.push_back(chunk);
	return true;
}

void ChunkWorker::_process()
{
	std::lock_guard<std::mutex> lg(_queueMutex);

	if (_chunkQueue.empty())
		return;

	_working = true;
	std::cout << "Processing " << _chunkQueue.size() << " chunks." << std::endl;
	for (Chunk * chunk : _chunkQueue)
	{
		chunk->generateBlocks();
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
		usleep(100);
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

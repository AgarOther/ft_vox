#include "ChunkWorker.hpp"
#include "colors.hpp"
#include <mutex>
#include <unistd.h>
#include <iostream>

std::mutex g_CHUNK_INFO;

uint8_t ChunkWorker::_count = 0;

ChunkWorker::ChunkWorker(): _active(false), _working(false)
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
		_chunkQueue.push_back(chunk);
	g_CHUNK_INFO.lock();
	std::cout << "[Worker #" << (int)_id << "] " << "Queued " << chunkQueue.size() << " chunks. Now has: " << _chunkQueue.size() << " chunks." << std::endl;
	g_CHUNK_INFO.unlock();
	return true;
}

void ChunkWorker::_process()
{
	const std::lock_guard<std::mutex> lg(_queueMutex);

	if (_chunkQueue.empty())
		return;

	g_CHUNK_INFO.lock();
	std::cout << "[Worker #" << (int)_id << "] " << "Processing " << _chunkQueue.size() << " chunks." << std::endl;
	_working = true;
	for (Chunk * chunk : _chunkQueue)
	{
		chunk->generateBlocks();
		chunk->generateMesh();
	}
	std::cout << "[Worker #" << (int)_id << "] " << "Cleared" << std::endl;
	g_CHUNK_INFO.unlock();
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

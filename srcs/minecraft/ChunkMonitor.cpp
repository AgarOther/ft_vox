#include "ChunkMonitor.hpp"
#include "colors.hpp"
#include "errors.hpp"
#include "utils.hpp"
#include <mutex>
#include <unistd.h>
#include <iostream>

ChunkMonitor::ChunkMonitor() : _active(false)
{
	const int threadCount = std::thread::hardware_concurrency() - 2; // minus main & this one
	if (threadCount <= 0)
		handleExit(FAILURE_THREAD);
	for (int i = 0; i < threadCount; ++i)
		_workers.push_back(new ChunkWorker);
	_start();
}

void ChunkMonitor::queue(std::vector<Chunk * > & chunkQueue)
{
	if (chunkQueue.empty())
		return;

	const std::lock_guard<std::mutex> lg(_queueMutex);

	_chunkQueue.reserve(chunkQueue.size());
	for (Chunk * chunk : chunkQueue)
	{
		if (chunk && chunk->getState() <= GENERATED)
			_chunkQueue.push_back(chunk);
	}
}

void ChunkMonitor::_process()
{
	const std::lock_guard<std::mutex> lg(_queueMutex);
	uint8_t	chunksToQueue;

	if (_chunkQueue.empty())
		return;
	for (ChunkWorker * worker : _workers)
	{
		chunksToQueue = std::min(_chunkQueue.size(), static_cast<size_t>(CHUNKS_PER_THREAD));
		if (!worker->isWorking() && chunksToQueue > 0)
		{
			if (worker->queue({_chunkQueue.begin(), _chunkQueue.begin() + chunksToQueue}))
				_chunkQueue.erase(_chunkQueue.begin(), _chunkQueue.begin() + chunksToQueue);
		}
	}
}

void ChunkMonitor::_loop()
{
	while (_active)
	{
		_process();
		usleep(100);
	}
}

void ChunkMonitor::_start()
{
	std::cout << GREEN << "[CHUNK] Started ChunkMonitor thread!" << RESET << std::endl;
	_active = true;
	for (ChunkWorker * worker : _workers)
		worker->start();
	_thread = std::thread(&ChunkMonitor::_loop, this);
}

void ChunkMonitor::stop()
{
	_active = false;
	_thread.join();
	for (ChunkWorker * worker : _workers)
	{
		worker->stop();
		delete worker;
	}
	std::cout << YELLOW << "[CHUNK] Stopped ChunkMonitor thread!" << RESET << std::endl;
}

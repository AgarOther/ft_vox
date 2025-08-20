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
	std::lock_guard<std::mutex> lg(_queueMutex);
	
	if (chunkQueue.empty())
		return;
	for (Chunk * chunk : chunkQueue)
		_chunkQueue.push_back(chunk);
}

void ChunkMonitor::_process()
{
	std::lock_guard<std::mutex> lg(_queueMutex);

	if (_chunkQueue.empty())
		return;
	for (ChunkWorker * worker : _workers)
	{
		if (!worker->isActive())
		{
			if (worker->queue(_chunkQueue))
				_chunkQueue.erase(_chunkQueue.begin(), _chunkQueue.begin() + CHUNKS_PER_THREAD);
		}
	}

	if (_chunkQueue.empty())
		_chunkQueue.shrink_to_fit();
}

void ChunkMonitor::_loop()
{
	std::lock_guard<std::mutex> lg(_queueMutex);
	if (_chunkQueue.empty())
		return;
	while (_active)
	{
		_process();
		usleep(100);
	}
}

void ChunkMonitor::_start()
{
	_active = true;
	_thread = std::thread(&ChunkMonitor::_loop, this);
	std::cout << GREEN << "[CHUNK] Started ChunkMonitor thread!" << RESET << std::endl;
	for (ChunkWorker * worker : _workers)
		worker->start();
}

void ChunkMonitor::stop()
{
	for (ChunkWorker * worker : _workers)
		worker->stop();
	_active = false;
	_thread.join();
	std::cout << YELLOW << "[CHUNK] Stopped ChunkMonitor thread!" << RESET << std::endl;
}

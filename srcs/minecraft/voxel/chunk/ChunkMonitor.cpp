#include "ChunkMonitor.hpp"
#include "ChunkWorker.hpp"
#include "colors.hpp"
#include "errors.hpp"
#include "utils.hpp"
#include <mutex>
#include <iostream>

void ChunkMonitor::start()
{
	#ifdef DEBUG
	std::cout << GREEN << "[CHUNK] Started ChunkMonitor thread!" << RESET << std::endl;
	#endif

	if (_workers.empty())
	{
		const int threadCount = (std::thread::hardware_concurrency() - 2); // minus main & this one
		if (threadCount - 2 <= 0) // minus at least a worker and a priority
			handleExit(FAILURE_THREAD);
		for (int i = 0; i < threadCount - 1; ++i)
			_workers.push_back(new ChunkWorker(_environment));
	}
	_priorityWorker = new PriorityWorker(_environment);

	_active = true;

	for (ChunkWorker * worker : _workers)
		worker->start();
	_priorityWorker->start();

	_thread = std::thread(&ChunkMonitor::_loop, this);
}

void ChunkMonitor::queue(std::vector<Chunk * > & chunkQueue)
{
	if (chunkQueue.empty())
		return;

	const std::lock_guard<std::mutex> lg(_queueMutex);

	_chunkQueue.reserve(_chunkQueue.size() + chunkQueue.size());
	for (Chunk * chunk : chunkQueue)
	{
		if (!chunk)
			continue;
		_chunkQueue.push_back(chunk);
	}
	_chunkQueue.shrink_to_fit();
}

void ChunkMonitor::queuePriority(std::vector<Chunk * > & chunkQueue)
{
	if (chunkQueue.empty())
		return;

	const std::lock_guard<std::mutex> lg(_queueMutex);

	for (Chunk * chunk : chunkQueue)
		chunk->addToPriorityMeshing();
	_priorityWorker->queue(chunkQueue);
}

bool ChunkMonitor::areWorkersWorking() const
{
	for (ChunkWorker * worker : _workers)
	{
		if (worker && worker->isWorking())
			return true;
	}
	return false;
}

void ChunkMonitor::_process()
{
	const std::lock_guard<std::mutex> lg(_queueMutex);

	if (_chunkQueue.empty()|| areWorkersWorking())
		return;

	uint8_t	chunksToQueue;

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
		USLEEP(100);
	}
}

void ChunkMonitor::stop()
{
	_active = false;
	if (_thread.joinable())
		_thread.join();
	for (ChunkWorker * worker : _workers)
	{
		worker->stop();
		delete worker;
	}
	_workers.clear();
	#ifdef DEBUG
	std::cout << YELLOW << "[CHUNK] Stopped ChunkMonitor thread!" << RESET << std::endl;
	#endif
}

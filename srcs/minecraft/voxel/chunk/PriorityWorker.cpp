#include "PriorityWorker.hpp"

bool PriorityWorker::queue(const std::vector<Chunk * > & chunkQueue)
{
	const std::lock_guard<std::mutex> lg(_queueMutex);

	if (chunkQueue.empty())
		return false;

	_chunkQueue.reserve(_chunkQueue.size() + chunkQueue.size());
	for (Chunk * chunk : chunkQueue)
	{
		if (chunk && chunk->hasPriority())
			_chunkQueue.push_back(chunk);
	}
	_chunkQueue.shrink_to_fit();
	return true;
}

void PriorityWorker::_process()
{
	const std::lock_guard<std::mutex> lg(_queueMutex);

	if (_chunkQueue.empty())
		return;

	_working = true;

	for (Chunk * chunk : _chunkQueue)
		chunk->generateMesh();

	_chunkQueue.clear();
	_working = false;
}

#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include "Chunk.hpp"
#include "PriorityWorker.hpp"

#define CHUNKS_PER_THREAD 2

class ChunkMonitor
{
	public:
		ChunkMonitor(Environment environment): _environment(environment), _active(false) {}
		
		void						queue(std::vector<Chunk * > & chunkQueue);
		void						queuePriority(std::vector<Chunk * > & chunkQueue);
		void						start();
		void						stop();
		bool						areWorkersWorking() const;
	private:
		void						_loop();
		void						_process();

		Environment					_environment;
		std::thread					_thread;
		std::atomic_bool			_active;
		std::mutex					_queueMutex;
		std::vector<Chunk * >		_chunkQueue;
		std::vector<ChunkWorker * >	_workers;
		PriorityWorker *			_priorityWorker;
};

#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include "Chunk.hpp"
#include "ChunkWorker.hpp"

#define CHUNKS_PER_THREAD 2

class ChunkMonitor
{
	public:
		ChunkMonitor();
		
		void						queue(std::vector<Chunk * > & chunkQueue);
		void						stop();
	private:
		void						_start();
		void						_loop();
		void						_process();

		std::thread					_thread;
		std::atomic_bool			_active;
		std::mutex					_queueMutex;
		std::vector<Chunk * >		_chunkQueue;
		std::vector<ChunkWorker * >	_workers;
};

#pragma once

#include "Chunk.hpp"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class ChunkWorker
{
	public:
		ChunkWorker(Environment environment);

		void					start();
		bool					queue(const std::vector<Chunk * > & chunkQueue);
		bool					isWorking() { return _working; }
		void					stop();
	private:
		void					_loop();
		void					_process();

		static uint8_t			_count;
		uint8_t					_id;
		std::thread				_thread;
		std::mutex				_queueMutex;
		std::atomic_bool		_active;
		std::atomic_bool		_working;
		std::vector<Chunk * >	_chunkQueue;
		Environment				_environment;
};

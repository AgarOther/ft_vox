#pragma once

#include "Chunk.hpp"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class ChunkWorker
{
	public:
		ChunkWorker(): _active(false), _working(false) {};

		void					start();
		bool					queue(std::vector<Chunk * > & chunkQueue);
		std::atomic_bool &		isActive() { return _active; }
		void					stop();
	private:
		void					_loop();
		void					_process();

		std::thread				_thread;
		std::mutex				_queueMutex;
		std::atomic_bool		_active;
		std::atomic_bool		_working;
		std::vector<Chunk * >	_chunkQueue;
};

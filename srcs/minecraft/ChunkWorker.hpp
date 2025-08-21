#pragma once

#include "Chunk.hpp"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

extern std::mutex g_CHUNK_INFO;

class ChunkWorker
{
	public:
		ChunkWorker();

		void					start();
		bool					queue(const std::vector<Chunk * > & chunkQueue);
		bool					isActive() { return _active; }
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
};

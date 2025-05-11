#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <deque>
#include <functional>

#include "Structures.h"

class ThreadPool
{
public:
	ThreadPool() = default;
	ThreadPool(MIndices::ParallelThreads threadOpts);
	~ThreadPool();

	void EnqueTask(std::function<void()> task);
private:
	std::vector<std::thread> threads;
	std::deque<std::function<void()>> tasks;
	std::condition_variable condition;
	std::mutex mutex;
	std::atomic_bool stop = false;
	const int32_t maxQueueSize = 10648;

	void StartThreads();
};

	
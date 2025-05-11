#include "ThreadPool.h"

ThreadPool::ThreadPool(MIndices::ParallelThreads threadOpts)
{
	if (threadOpts != MIndices::ParallelThreads::off)
	{
		uint32_t numThreads = MIndices::threadMap[static_cast<uint32_t>(threadOpts)];
		for (uint32_t i = 0; i < numThreads; ++i)
		{
			threads.emplace_back([this]()
				{
					StartThreads();
				});
		}
	}
	else
	{
		throw std::runtime_error("Failed to intialize Thread Pool, invalid number of threads passed.");
	}

}

ThreadPool::~ThreadPool()
{
	stop = true;
	condition.notify_all();
	for (auto& thread : threads)
	{
		thread.join();
	}
}

void ThreadPool::EnqueTask(std::function<void()> task)
{
	if (tasks.size() >= maxQueueSize)
	{
		throw std::runtime_error("Tasks queue already at maximum size.");
	}

	std::unique_lock<std::mutex> lock(mutex);
	tasks.push_back(std::move(task));
	condition.notify_one();
}

void ThreadPool::StartThreads()
{
	while (true)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(mutex);
			condition.wait(lock, [this]()
				{
					return stop || !tasks.empty();
				});

			if (stop && tasks.empty())
			{
				return;
			}

			task = std::move(tasks.front());
			tasks.pop_front();
		}
		task();
	}
}

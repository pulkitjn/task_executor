#pragma once

#include <vector>
#include <map>

#include "task_params.h"
#include "task_queue.h"

class Executor
{
    private:
        size_t m_numThreads;
        std::vector<std::thread> m_workerThreads;
        std::vector<TaskQueue> m_taskQueues;
        uint64_t m_currentTaskId;
        std::map<uint64_t, std::shared_ptr<TaskParams>> m_tpMap;
        bool bStop;

        bool IsStopped() {
            return bStop;
        }

        void SetStopped(bool val) {
            bStop = val;
        }

    public:
        Executor (int numThreads);

        // Post on queueIndexTh taskQueue and task that you want to execute
        // Each task queue is assoicated with one of the threads
        // Posting to same queue will lead to synchronous execution
        void Post(uint64_t queueIndex, std::function<void()>&& task);

        // Call this to stop the executor. This will
        // stop all of taskQueues but wont interrupt
        // any function if it is executing at the 
        // time of the stop call, rather it will wait for
        // it to finish
        void Stop();

        // Prints out the execution details of each of the tasks
        void PrintExecutionTimes();

        ~ Executor();
};

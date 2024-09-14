//executor.cpp
#include "executor.h"

Executor::Executor (int numThreads): bStop(false), m_numThreads(numThreads), m_currentTaskId(0)
{
    // keep the queues ready for threads
    for (int i=0; i<numThreads; i++) {
        m_taskQueues.emplace_back(TaskQueue());
    }

    // create threads and let them start spinning with corresponding 
    // taskQueues waiting for tasks to get posted to them
    for (int i=0; i<numThreads; i++) {
        m_workerThreads.emplace_back(std::thread([this, i]{m_taskQueues[i].Run();} ));
    }
}

Executor::~Executor() {
    if (!IsStopped()) {
        Stop();
    }
    m_tpMap.clear();
}

// Post on queueIndexTh taskQueue and task that you want to execute
// Each task queue is assoicated with one of the threads
// Posting to same queue will lead to synchronous execution
void Executor::Post(uint64_t queueIndex, std::function<void()>&& task) 
{
    if (IsStopped()) {
        return;
    }

    if (queueIndex >= m_numThreads) {
        return;
    }
    auto& tp = m_tpMap[m_currentTaskId];
    tp = std::make_shared<TaskParams>();
    m_currentTaskId++;
    m_taskQueues[queueIndex].Post(std::move(task), tp);
}

// Call this to stop the executor. This will
// stop all of taskQueues but wont interrupt
// any function if it is executing at the 
// time of the stop call, rather it will wait for
// it to finish
void Executor::Stop() {
    if (IsStopped()) {
        return;
    }
    for (int i=0; i<m_numThreads; i++) {
        m_taskQueues[i].Stop();
        m_workerThreads[i].join();
    }
    m_workerThreads.clear();
    m_taskQueues.clear();
    m_numThreads = 0;
    SetStopped(true);
}

// Prints out the execution details of each of the tasks
void Executor::PrintExecutionTimes() {
    for (const auto& it: m_tpMap) {
        std::cout << "task_id=" << it.first << ' ' << (*it.second.get()) << "\n\n";
    }
}
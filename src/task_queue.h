#pragma once

#include <queue>
#include <thread>

#include "task_params.h"

class TaskQueue
{
    private:
        std::queue<std::pair<std::function<void()>, std::shared_ptr<TaskParams>>> m_queue;
        std::condition_variable m_cv;
        std::mutex m_queueMtx;
        bool bStop;

        bool IsStopped() {
            return bStop;
        }

        void SetStopped(bool val) {
            bStop = val;
        }

    public:
        TaskQueue() : bStop(true) {}

        TaskQueue (const TaskQueue&& other) {
            assert(other.bStop == true);
            SetStopped(true);
        }

        // Post a task to queue. Notify the thread
        // for it to start spinning again
        void Post(std::function<void()>&& task, std::shared_ptr<TaskParams> tp);

        // Worker function for the created thread
        // Start queue spinning waiting for tasks
        // if no tasks available the thread will be
        // put to sleep not consuming any processing 
        // power. When the task gets posted the thread 
        // will be notified. 
        void Run();

        // Stop the queue from executing further functions
        // and clear the queue. This function should be called
        // from the main executor thread
        void Stop();
};
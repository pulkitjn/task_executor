#include "task_queue.h"

void TaskQueue::Post(std::function<void()>&& task, std::shared_ptr<TaskParams> tp) {
    if (IsStopped()) {
        return;
    }
    std::unique_lock<std::mutex> lck(m_queueMtx);
    m_queue.push({std::move(task), tp});
    m_cv.notify_one();
}

void TaskQueue::Run() {
    SetStopped(false);
    std::unique_lock<std::mutex> lck(m_queueMtx);
    while(true) {
        m_cv.wait(lck, [this]{ return !m_queue.empty() || IsStopped() ;});
        if (IsStopped()) {
            return;
        }
        auto task = std::move(m_queue.front().first);
        auto tpPtr = m_queue.front().second;
        m_queue.pop();
        tpPtr->SetPopPoint();
        task();
        tpPtr->SetExecutionPoint();
    }
}

void TaskQueue::Stop() {
    std::unique_lock<std::mutex> lck(m_queueMtx);
    SetStopped(true);
    m_cv.notify_one();
    while (!m_queue.empty()) {
        m_queue.pop();
    }
}
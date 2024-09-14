#include "task_params.h"
#include "iostream"

void TaskParams::SetPopPoint() {
    popPoint = std::chrono::high_resolution_clock::now();
}

void TaskParams::SetExecutionPoint() {
    bExecuted = true;
    executionPoint = std::chrono::high_resolution_clock::now();
}

std::chrono::milliseconds TaskParams::GetTimeSpentInQueue() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(popPoint - pushPoint);
}

std::chrono::milliseconds TaskParams::GetTimeSpentInExecution() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(executionPoint - popPoint);
}

std::chrono::milliseconds TaskParams::GetTotalTimeSpent() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(executionPoint - pushPoint);
}

bool TaskParams::IsExecuted() const {
    return bExecuted;
}

std::ostream& operator<< (std::ostream& ostr , const TaskParams& tp) {
    ostr << std::boolalpha << "executed=" << tp.IsExecuted();
    if (!tp.IsExecuted()) {
        return ostr;
    }
    ostr << " time_spent_in_queue=" << tp.GetTimeSpentInQueue().count() << "ms";
    ostr << " time_spent_in_execution=" << tp.GetTimeSpentInExecution().count() << "ms";
    ostr << " total_time_spent=" << tp.GetTotalTimeSpent().count() << "ms";
    return ostr;
}

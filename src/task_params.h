#pragma once

#include <chrono>
#include <iostream>

class TaskParams
{
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> CurrentTime;
    private:
        CurrentTime pushPoint;
        CurrentTime popPoint;
        CurrentTime executionPoint;
        bool bExecuted;

    public:

        TaskParams() : pushPoint(std::chrono::high_resolution_clock::now()), bExecuted(false) {}

        void SetPopPoint();

        void SetExecutionPoint();

        std::chrono::milliseconds GetTimeSpentInQueue() const ;

        std::chrono::milliseconds GetTimeSpentInExecution() const ;

        std::chrono::milliseconds GetTotalTimeSpent() const ;

        bool IsExecuted() const ;

        friend std::ostream& operator<< (std::ostream& ostr , const TaskParams& tp);
};

std::ostream& operator<< (std::ostream& ostr , const TaskParams& tp);
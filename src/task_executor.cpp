#include <thread>
#include <iostream>

#include "executor.h"
#include "task_params.h"

int main() {
    // making use of actual parallelism
    // what ratio of available cores you want to use
    // technically one should check the system specs
    // and decide what percentage of cores you want to use for your process
    // It is just guesstimate really cause OS will determine if these threads would
    // be on different cores or not. But it generally works out
    uint64_t totalCores = std::thread::hardware_concurrency();
    uint64_t factor = 3;
    uint64_t coresToBeUsed = std::thread::hardware_concurrency()/factor;
    std::cout << "total_cores=" << totalCores << " cores_to_be_used=" << coresToBeUsed << "\n\n";
    Executor e(coresToBeUsed);
    TaskParams tp;
    tp.SetPopPoint();
    uint64_t taskCnt = 100;

    for (int i=0; i<taskCnt; i++) {
        e.Post(i%coresToBeUsed, [i]{
            //std::cout << "Executing task i=" << i <<" thread_id= " << std::this_thread::get_id() << "\n\n";
            // Simulate task
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        });
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(50*taskCnt/coresToBeUsed));
    e.Stop();
    tp.SetExecutionPoint();
    e.PrintExecutionTimes();

    std::cout << tp << " " << "normal_execution_time=" << 50*taskCnt << "ms";
}
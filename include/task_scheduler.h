#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <Arduino.h>
#include <vector>

struct Task {
    void (*function)();
    unsigned long interval;
    unsigned long lastRun;
    
    Task(void (*f)(), unsigned long i) : function(f), interval(i), lastRun(0) {}
};

class TaskScheduler {
public:
    void addTask(void (*function)(), unsigned long interval) {
        tasks.push_back(Task(function, interval));
    }

    void run() {
        unsigned long currentMillis = millis();
        for (Task& task : tasks) {
            if (currentMillis - task.lastRun >= task.interval) {
                task.function();
                task.lastRun = currentMillis;
            }
        }
    }

private:
    std::vector<Task> tasks;
};

#endif
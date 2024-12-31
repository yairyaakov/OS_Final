#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H
#include <iostream>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <string>

struct PTask {
    MST mst;               
    std::mutex taskMutex;
    std::condition_variable cv;
    bool isCompleted = false;
    std::string result;     
    int u, v;               


    void taskCompleted() {
        std::lock_guard<std::mutex> lock(taskMutex);
        isCompleted = true;
        cv.notify_one();
    }

    PTask(MST mst, int u = 0, int v = 0) : mst(mst), u(u), v(v) {}
};

class ActiveObject {
public:
    using ProcessFunc = std::function<void(PTask&)>;

    // Constructor takes a function that will process tasks
    ActiveObject(int id, ProcessFunc processFunc) : id(id), processFunc(processFunc) {
        worker = std::thread([this]() {
            while (true) {
                std::function<PTask&()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] { return !taskQueue.empty() || stop; });
                    if (stop && taskQueue.empty()) {
                        return;
                    }
                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                }
                this->processFunc(task());  

                if (next) next->postTask(task); 
                else task().taskCompleted();
            }
        });
    }

    ~ActiveObject() {
        stopWorker();
    }

    // Post a task to the task queue
    void postTask(std::function<PTask&()> task) {

        {
            std::lock_guard<std::mutex> lock(mtx);
            taskQueue.push(std::move(task));
            cv.notify_one();
        }
        
    }

    // Set the next ActiveObject in the pipeline
    void setNext(ActiveObject* nextActiveObject) {
        next = nextActiveObject;
    }

    // Stop the worker thread
    void stopWorker() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
            cv.notify_one();
        }
        
        if (worker.joinable()) {
            worker.join();
        }
    }

private:
    std::queue<std::function<PTask&()>> taskQueue;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread worker;
    bool stop = false;
    int id;
    ProcessFunc processFunc;
    ActiveObject* next = nullptr;  
};



#endif //ACTIVEOBJECT_H

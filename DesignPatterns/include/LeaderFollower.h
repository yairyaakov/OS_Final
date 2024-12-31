#ifndef LEADERFOLLOWER_H
#define LEADERFOLLOWER_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <thread>
#include <vector>
#include <future> // For std::promise and std::future
#include <iostream>

class LeaderFollower {
public:
    // Constructor that spawns a number of worker threads
    explicit LeaderFollower(int numWorkers);

    // Method to add a task to the queue
    void addTask(std::function<void()> task);

    // Method to ensure graceful shutdown
    void finish();

private:
    // Worker thread function
    void WorkerThread(int id);

    std::mutex mtx; // Mutex for protecting shared resources
    std::condition_variable cv; // Condition variable for synchronization
    std::queue<std::function<void()>> tasks; // Task queue
    bool done = false; // Flag for termination
    std::vector<std::thread> workers; // Worker threads
};

#endif // LEADERFOLLOWER_H

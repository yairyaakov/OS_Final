#include "../include/LeaderFollower.h"

LeaderFollower::LeaderFollower(int numWorkers) {
    for (int i = 1; i <= numWorkers; i++) {
        workers.emplace_back(&LeaderFollower::WorkerThread, this, i);
    }
}

void LeaderFollower::addTask(std::function<void()> task) {
    
    std::lock_guard<std::mutex> lock(mtx);
    tasks.push(std::move(task));
    
    cv.notify_one();
}


void LeaderFollower::finish() { {
        std::lock_guard<std::mutex> lock(mtx);
        done = true; 
        cv.notify_all(); 
    }
    
    // Wait for all threads to finish
    for (auto &t: workers) {
        if (t.joinable()) {
            t.join(); 
        }
    }
}

void LeaderFollower::WorkerThread(int id) {
    while (true) {
        std::function<void()> task;  
        {
            std::unique_lock<std::mutex> lock(mtx);

            // Wait until we have a task or are shutting down
            cv.wait(lock, [this] { return !tasks.empty() || done; });

            // If no tasks and we're done, exit the loop
            if (tasks.empty() && done)
                break;

            // Take the task from the queue
            task = std::move(tasks.front());
            tasks.pop();

            // Promote a new follower to become a leader
            cv.notify_one();
        }

        // Execute the task
        task();
    }
}


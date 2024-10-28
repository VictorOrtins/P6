#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <chrono>
#include <semaphore.h>

const int NUM_PRODUCERS = 4;
const int NUM_CONSUMERS = 4;

std::queue<std::string> buffer;
std::mutex bufferMutex;
sem_t emptySlots, fullSlots;

void producer(int id, const std::vector<std::string>& messages) {
    for (const auto& message : messages) {
        for (const auto& word : message) {
            sem_wait(&emptySlots); // Wait for empty slots in the buffer
            {
                std::lock_guard<std::mutex> lock(bufferMutex);
                buffer.push(std::string(1, word));
            }
            sem_post(&fullSlots); // Signal that buffer has a new item
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulating delay
        }
    }
}

void consumer(int id) {
    while (true) {
        sem_wait(&fullSlots); // Wait for items in the buffer
        std::string word;
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            word = buffer.front();
            buffer.pop();
        }
        sem_post(&emptySlots); // Signal that there's an empty slot in the buffer
        std::cout << "Consumer " << id << " read: " << word << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulating processing time
    }
}

int main() {
    sem_init(&emptySlots, 0, buffer.size()); // Initialize emptySlots semaphore with buffer size
    sem_init(&fullSlots, 0, 0); // Initialize fullSlots semaphore with 0

    std::vector<std::string> messages = {"Message1", "Message2", "Message3", "Message4"};
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.push_back(std::thread(producer, i, messages));
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.push_back(std::thread(consumer, i));
    }

    // Join producer threads
    for (auto& producerThread : producers) {
        producerThread.join();
    }

    // Join consumer threads
    for (auto& consumerThread : consumers) {
        consumerThread.join();
    }

    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);

    return 0;
}

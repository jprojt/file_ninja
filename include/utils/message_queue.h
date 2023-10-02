#pragma once

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>

enum MessageQueueReceiveStatus {
    SUCCESS,
    TIMEOUT,
    CLOSED,
};

template<typename T>
class MessageQueue {
public:
    MessageQueue<T>() = default;

    MessageQueueReceiveStatus Receive(std::chrono::milliseconds wait_time, std::unique_ptr<T>& out_item);
    MessageQueueReceiveStatus Receive(std::unique_ptr<T>& out_item);
    void Send(const T&& item);
    void Close();
    bool IsClosed();
    void Reset();
    

private:
    std::condition_variable condition_;
    std::mutex mutex_;
    std::deque<T> deque_;
    bool closed_ = false;

    static constexpr std::chrono::milliseconds kInfiniteDuration{-1};
};



//implementation:
template<typename T>
MessageQueueReceiveStatus MessageQueue<T>::Receive(std::chrono::milliseconds wait_time, std::unique_ptr<T>& out_item) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto is_empty_delegate = [&] { return !deque_.empty() || closed_;};
    if (wait_time == kInfiniteDuration) {
        condition_.wait(lock, is_empty_delegate);
    } else {
        if (!condition_.wait_for(lock, wait_time, is_empty_delegate)) {
            return closed_ ? MessageQueueReceiveStatus::CLOSED : MessageQueueReceiveStatus::TIMEOUT;
        }
    }

    if (closed_ && deque_.empty()) {
        return MessageQueueReceiveStatus::CLOSED;
    }

    out_item = std::make_unique<T>(std::move(deque_.front()));
    deque_.pop_front();
    return MessageQueueReceiveStatus::SUCCESS;
}

template<typename T>
MessageQueueReceiveStatus MessageQueue<T>::Receive(std::unique_ptr<T>& out_item) {
    return this->Receive(kInfiniteDuration, out_item);
}


template<typename T>
void MessageQueue<T>::Send(const T&& item) {
    std::unique_lock<std::mutex> lock(mutex_);
    deque_.emplace_back(std::move(item));
    lock.unlock();
    condition_.notify_one();
}

template<typename T>
void MessageQueue<T>::Close() {
    closed_ = true;
    condition_.notify_all();
}

template<typename T>
bool MessageQueue<T>::IsClosed() {
    return closed_;
}

template<typename T>
void MessageQueue<T>::Reset() {
    std::unique_lock<std::mutex> lock(mutex_);
    deque_.clear();
    closed_ = false;
    lock.unlock();
    condition_.notify_all();
}
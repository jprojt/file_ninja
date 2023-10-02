#include "fs/walker.h"
#include "utils/message_queue.h"
#include <chrono>
#include <exception>
#include <iostream>
#include <stack>
#include <thread>
#include <utility>
#include <filesystem>

Walker::Walker() {

}

Walker::~Walker() {
    if (thread_.joinable()) {
        thread_.join();
    }
}


bool Walker::Walk() {
    std::stack<std::filesystem::path> pending_dirs;
    pending_dirs.push(path_);
    std::filesystem::path current_dir;
    while (!pending_dirs.empty()) {
        current_dir = pending_dirs.top();
        pending_dirs.pop();
        dir_count_++;
        try {
            for (const auto& item: std::filesystem::directory_iterator(current_dir)) {
                if (stop_) {
                    break;
                }
                try {
                    if (item.is_directory() && !item.is_symlink()) {
#if __linux__
                        if (item.path() == "/proc") {
                            continue;
                        }
#endif
                        if (item.path() == "/dev") {
                            continue;
                        }
                        pending_dirs.emplace(item.path());
                    } else if (item.is_regular_file() && !item.is_symlink()) {
                        file_count_++;
                            if (!stop_ && handler_(item)) {
                                message_queue_.Send(std::move(item));
                            }
                    }
                } catch (const std::filesystem::filesystem_error& ex) {
                }
            }
        } catch (const std::filesystem::filesystem_error& fs_err) {
        }
        if (stop_) {
            break;
        }
    } 
    message_queue_.Close();
    running_ = false;
    stop_ = false;
    return false;
}


bool Walker::StartWalking(const std::filesystem::path& path, WalkerDelegate delegate) {
    if (running_) {
        return false;
    }
    file_count_ = 0;
    dir_count_ = 0;
    path_ = path;
    handler_ = delegate;
    message_queue_.Reset();
    running_ = true;
    stop_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
    thread_ = std::thread(&Walker::Walk, this);
    return true;
}


MessageQueue<std::filesystem::directory_entry>* Walker::GetQueue() {
    return &message_queue_;
}

long Walker::FileCount() const {
    return file_count_;
}

long Walker::DirCount() const {
    return dir_count_;
}

bool Walker::IsRunning() const {
    return running_;
}

void Walker::Stop() {
    if (!running_) {
        return;
    }
    stop_ = true;
}
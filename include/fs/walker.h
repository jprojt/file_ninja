#pragma once 


#include "utils/message_queue.h"
#include <algorithm>
#include <filesystem>
#include <functional>
#include <thread>
class Walker {
public:
    typedef std::function<bool(const std::filesystem::directory_entry& entry)> WalkerDelegate;

    Walker();
    ~Walker();
    Walker(const Walker& other) = delete;
    Walker operator=(const Walker& other) = delete;
    Walker& operator=(Walker&& other) = delete;
    Walker(Walker&& other) = delete;


    //Start the file system walk operation thread
    bool StartWalking(const std::filesystem::path& path, WalkerDelegate delegate);
    //number of files checked
    long FileCount() const;
    //number of dirs entered
    long DirCount() const;
    //currently walking
    bool IsRunning() const;
    
    void Stop();

    MessageQueue<std::filesystem::directory_entry>* GetQueue();
    

private:
    std::filesystem::path path_;
    std::thread thread_;
    bool running_{false};
    bool stop_{false};
    long file_count_{0};
    long dir_count_{0};
    WalkerDelegate handler_;
    MessageQueue<std::filesystem::directory_entry> message_queue_;
    bool Walk();
};
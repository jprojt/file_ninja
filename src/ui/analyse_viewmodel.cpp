#include "analyse_viewmodel.h"
#include "utils/formatter.h"
#include "utils/message_queue.h"
#include "waffle_listmodel.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <future>
#include <iostream>
#include <memory>
#include <thread>


AnalyseViewmodel* AnalyseViewmodel::GetInstance() {
    static AnalyseViewmodel* instance = new AnalyseViewmodel();
    return instance;
}

AnalyseViewmodel::~AnalyseViewmodel() {
    walker_.Stop();
    walker_.GetQueue()->Reset();
}

bool AnalyseViewmodel::isRunning() const {
    return walker_.IsRunning();
}

void AnalyseViewmodel::stop() {
    walker_.Stop();
    walker_.GetQueue()->Reset();
}

void AnalyseViewmodel::start() {
    //dummy data for now
    if (walker_.IsRunning()) {
        return;
    }

    waffle_listmodel_.Clear();

    std::string start_path("/");

    bool started = walker_.StartWalking(start_path, [&](const std::filesystem::directory_entry& entry) {
        try {
            waffle_listmodel_.Increment(std::string(entry.path().extension()), entry.file_size());
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << ex.what() << "\n";
        }
        return false;
    });

    if (!started) {
        return;
    } 

    future_ = std::async(std::launch::async, &AnalyseViewmodel::ReceiveFiles, this);
    
    emit isRunningChanged();

    setFirstRunComplete(true);
}

WaffleListModel* AnalyseViewmodel::waffleListModel() {
    return &waffle_listmodel_;
}


bool AnalyseViewmodel::firstRunComplete() const {
    return first_run_complete_;
}

void AnalyseViewmodel::setFirstRunComplete(bool complete) {
    if (first_run_complete_ == complete) {
        return;
    }
    first_run_complete_ = complete;
    emit firstRunCompleteChanged();
}

void AnalyseViewmodel::ReceiveFiles() {
    std::unique_ptr<std::filesystem::directory_entry> next;
    //bit cheeky, use wait_for with 30ms wait time and use it as a ui update timer
    while (walker_.GetQueue()->Receive(std::chrono::milliseconds(30), next) != MessageQueueReceiveStatus::CLOSED) {
        emit filesScannedChanged();
        emit dirsScannedChanged();
    }

    waffle_listmodel_.ForceUpdate();
    emit isRunningChanged();
}

long AnalyseViewmodel::filesScanned() const {
    return walker_.FileCount();
}

long AnalyseViewmodel::dirsScanned() const {
    return walker_.DirCount();
}

QString AnalyseViewmodel::bytesToString(long bytes) const {
    return formatter::FormatBytes(bytes).c_str();
}
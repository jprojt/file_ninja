#include "search_viewmodel.h"
#include <QObject>
#include <QString>
#include <QtCore/qobjectdefs.h>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <regex>
#include "fs/walker.h"
#include "search_listmodel.h"
#include "utils/message_queue.h"
#include "utils/rate_limit_delegate.h"
#include "utils/string_util.h"



SearchViewmodel* SearchViewmodel::GetInstance() {
    static SearchViewmodel* instance = new SearchViewmodel();
    return instance;
}

SearchViewmodel::~SearchViewmodel() {
    walker_.Stop();
    walker_.GetQueue()->Reset();
}

SearchViewmodel::SearchViewmodel(QObject* parent): QObject(parent) {
    const char* home_dir = std::getenv("HOME");
    setSearchPath(home_dir);
    scan_count_limiter_.SetDelegate([&]{
        setFilesScanned(walker_.FileCount());
        setDirsScanned(walker_.DirCount());
    });
}

QString SearchViewmodel::searchTerm() const {
    return search_term_;
}

void SearchViewmodel::setSearchTerm(const QString& search_term) {
    if (search_term != search_term_){
        search_term_ = search_term;
        emit searchTermChanged();
        SetRegex();
    }
}

bool SearchViewmodel::isSearchTermValid() const {
    return is_search_term_valid_;
}

void SearchViewmodel::setIsSearchTermValid(bool valid) {
    if (is_search_term_valid_ == valid) {
        return;
    }
    is_search_term_valid_ = valid;
    emit isSearchTermValidChanged();
}

bool SearchViewmodel::isSearching() const {
    return walker_.IsRunning();
}

bool SearchViewmodel::firstSearchComplete() const {
    return first_search_complete_;
}

bool SearchViewmodel::SetRegex() {
    if (isSearching()) {
        return true;    
    }
    std::regex_constants::syntax_option_type regex_flags = case_sensitive_ ? std::regex_constants::basic : std::regex_constants::icase;

    try {
        if (use_regex_) {
        regex_ = std::basic_regex(search_term_.toStdString(), regex_flags);
    } else {
        regex_ = std::basic_regex(string_util::EscapeRegexString(search_term_.toStdString()), regex_flags);
    }
    } catch (const std::regex_error& err) {
        std::cerr << err.what() << " " << err.code() << "\n";
        setIsSearchTermValid(false);
        return false;
    }
    setIsSearchTermValid(true);
    return true;
}

void SearchViewmodel::startSearch() {
    if (!SetRegex()) {
        return;
    }

    if (!first_search_complete_) {
        first_search_complete_ = true;
        emit firstSearchCompleteChanged();
    }

    if (isSearching()) {
        return;
    }

    scan_count_limiter_.ForceUpdate();
    list_model_.Clear();
    
    bool started = walker_.StartWalking(search_path_.toStdString(), [&] (const std::filesystem::directory_entry& entry) {
        scan_count_limiter_.Update();
        return IsMatch(entry);
    });

    if (started) {
        receieve_future_ = std::async(std::launch::async, &SearchViewmodel::ReceiveFiles, this);
    }
    emit isSearchingChanged();
}

void SearchViewmodel::stop() {
    walker_.Stop();
}

bool SearchViewmodel::IsMatch(const std::filesystem::directory_entry& entry) {
    std::smatch smatch;
    std::string searchable_name(entry.path().filename());
    return std::regex_search(searchable_name, smatch, regex_);
}

long SearchViewmodel::filesScanned() const {
    return files_scanned_;
}

void SearchViewmodel::setFilesScanned(long files_scanned) {
    if (files_scanned == files_scanned_) {
        return;
    }
    files_scanned_ = files_scanned;
    emit filesScannedChanged();
}

long SearchViewmodel::dirsScanned() const {
    return dirs_scanned_;
}

void SearchViewmodel::setDirsScanned(long dirs_scanned) {
    if (dirs_scanned == dirs_scanned_) {
        return;
    }
    dirs_scanned_ = dirs_scanned;
    emit filesScannedChanged();
}

SearchListModel* SearchViewmodel::listModel() {
    return &list_model_;
}

void SearchViewmodel::ReceiveFiles() {
    std::unique_ptr<std::filesystem::directory_entry> entry;
    while (walker_.GetQueue()->Receive(entry) != MessageQueueReceiveStatus::CLOSED) {
        if (list_model_.rowCount() >= result_limit_) {
            walker_.Stop();
            continue;
        }
        try {
            list_model_.Add(std::move(*entry));
        } catch (const std::exception& ex) {
            continue;
        }
    }

    scan_count_limiter_.ForceUpdate();

    emit isSearchingChanged();
}

QString SearchViewmodel::searchPath() const {
    return search_path_;
}

void SearchViewmodel::setSearchPath(const QString& search_path) {
    if (search_path != search_path_) {
        search_path_ = search_path;
        emit searchPathChanged();
    }
}

long SearchViewmodel::resultLimit() const {
    return result_limit_;
}

void SearchViewmodel::setResultLimit(long limit) {
    if (limit == result_limit_) {
        return;
    }
    result_limit_ = limit;
    emit resultLimitChanged();
}

void SearchViewmodel::showSettings() {
    emit requestedDisplaySettings();
}

bool SearchViewmodel::useRegex() const {
    return use_regex_;
}

void SearchViewmodel::setUseRegex(bool use_regex) {
    if (use_regex_ == use_regex) {
        return;
    }
    use_regex_ = use_regex;
    emit useRegexChanged();
    SetRegex();
}

bool SearchViewmodel::caseSensitive() const {
    return case_sensitive_;
}

void SearchViewmodel::setCaseSensitive(bool case_sensitive) {
    if (case_sensitive == case_sensitive_) {
        return;
    }
    case_sensitive_ = case_sensitive;
    emit caseSensitiveChanged();
}
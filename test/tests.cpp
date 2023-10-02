#include "fs/walker.h"
#include "utils/message_queue.h"
#include <algorithm>
#include <filesystem>
#include <future>
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


class WalkerTests: public ::testing::Test {
public:
    WalkerTests() {
        path = std::filesystem::current_path()
            .remove_filename()
            .parent_path()
            .parent_path()
            .append("test_data")
            .append("fake_fs");
    }
    std::filesystem::path path;
};

TEST_F(WalkerTests, TestWalker) { 
    Walker walker;
    std::vector<std::filesystem::directory_entry> entries;
    std::map<std::string, bool> lookup;

    std::unique_ptr<std::filesystem::directory_entry> entry;

    walker.StartWalking(this->path, [](const std::filesystem::directory_entry& entry) {
        return true;
    });

    while (walker.GetQueue()->Receive(entry) != CLOSED) {
        lookup.insert_or_assign(entry->path().filename(), true);
        entries.emplace_back(std::move(*entry));
        std::cout << "got file: " << entries.back().path().string() << "\n";
    }

    for (auto& expected_names: {"testfile1.txt", "testfile2.123"}) {
        auto it = lookup.find(expected_names);
        EXPECT_FALSE(it == lookup.end());
    }

    EXPECT_EQ(walker.FileCount(), 2);
}

TEST_F(WalkerTests, TestExtSearch) {
    std::map<std::string, long> extension_map;
    Walker walker;
    walker.StartWalking(this->path, [&](const std::filesystem::directory_entry& entry) {
        return true;
    });
    std::unique_ptr<std::filesystem::directory_entry> next;
    while (walker.GetQueue()->Receive(next) != MessageQueueReceiveStatus::CLOSED) {
        auto it = extension_map.find(next->path().extension());
        if (it == extension_map.end()) {
            extension_map.insert_or_assign(next->path().extension(), std::filesystem::file_size(next->path()));
        } else {
            it->second += std::filesystem::file_size(next->path());
        }
    }

    EXPECT_EQ(walker.FileCount(), 2);
    
    long sum_size = std::accumulate(extension_map.begin(), extension_map.end(), 0L, [](long acc, const std::pair<std::string, long>& p) {
        return acc + p.second;
    });

    EXPECT_EQ(sum_size, 8);
    EXPECT_EQ(extension_map.size(), 2);
}

TEST_F(WalkerTests, TestRegexSearch) {
    std::basic_regex test_search(R"(.*test.*)");
    std::basic_regex file1_search(R"(.*file1.*)");
    std::smatch match_result;    
    int file1_count = 0;
    Walker walker;
    walker.StartWalking(this->path, [&](const std::filesystem::directory_entry& entry) {
        std::string name(entry.path().filename());
        bool test_match = std::regex_search(name, match_result, test_search);
        bool file1_match = std::regex_search(name, match_result, file1_search);
        if (file1_match) {
            file1_count++;
            std::cout << name << "\n";
        }
        return test_match || file1_match;
    });
    std::unique_ptr<std::filesystem::directory_entry> next;
    std::vector<std::filesystem::directory_entry> entries;
    while (walker.GetQueue()->Receive(next) != MessageQueueReceiveStatus::CLOSED) {
        entries.emplace_back(std::move(*next));
    }
    EXPECT_EQ(entries.size(), 2);
    EXPECT_EQ(file1_count, 1);
}


class MessageQueueTests: public ::testing::Test {

};

TEST_F(MessageQueueTests, TestMessageQueue) {
    MessageQueue<std::string> queue;
    std::vector<std::string> results;
    auto receiver = std::async(std::launch::async, [&] {
        std::unique_ptr<std::string> item;
        while (queue.Receive(item) != MessageQueueReceiveStatus::CLOSED) {
            results.emplace_back(std::move(*item));
        }
    });

    for (int i = 0; i < 10; i++) {
        queue.Send(std::move("Item " + std::to_string(i)));
    }
    queue.Close();

    receiver.wait();

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(results[i], "Item " + std::to_string(i));
    }
}
#pragma once

#include "fs/walker.h"
#include "search_listmodel.h"
#include "utils/rate_limit_delegate.h"
#include <QObject>
#include <QString>
#include <chrono>
#include <filesystem>
#include <future>
#include <memory>
#include <ratio>
#include <regex>

class SearchViewmodel: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged);
    Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged);
    Q_PROPERTY(bool isSearchTermValid READ isSearchTermValid NOTIFY isSearchTermValidChanged);
    Q_PROPERTY(bool firstSearchComplete READ firstSearchComplete NOTIFY firstSearchCompleteChanged);
    Q_PROPERTY(long filesScanned READ filesScanned NOTIFY filesScannedChanged);
    Q_PROPERTY(long dirsScanned READ dirsScanned NOTIFY dirsScannedChanged);
    Q_PROPERTY(SearchListModel* listModel READ listModel NOTIFY listModelChanged);

    //options
    Q_PROPERTY(QString searchPath READ searchPath WRITE setSearchPath NOTIFY searchPathChanged);
    Q_PROPERTY(long resultLimit READ resultLimit WRITE setResultLimit NOTIFY resultLimitChanged);
    Q_PROPERTY(bool useRegex READ useRegex WRITE setUseRegex NOTIFY useRegexChanged);
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY caseSensitiveChanged);
    
public:
    static SearchViewmodel* GetInstance();
    ~SearchViewmodel();

    QString searchTerm() const;
    void setSearchTerm(const QString& search_term);
    bool isSearchTermValid() const;
    bool isSearching() const;
    bool firstSearchComplete() const;
    long filesScanned() const;
    long dirsScanned() const;
    QString searchPath() const;
    void setSearchPath(const QString&  search_path);
    long resultLimit() const;
    void setResultLimit(long limit);
    bool useRegex() const;
    void setUseRegex(bool use_regex);
    bool caseSensitive() const;
    void setCaseSensitive(bool case_sensitive);
    SearchListModel* listModel();

signals:
    void searchTermChanged();
    void isSearchTermValidChanged();
    void isSearchingChanged();
    void firstSearchCompleteChanged();
    void filesScannedChanged();
    void dirsScannedChanged();
    void listModelChanged();
    // options
    void searchPathChanged();
    void resultLimitChanged();
    void useRegexChanged();
    void caseSensitiveChanged();

    void requestedDisplaySettings();

public slots:
    void startSearch();
    void stop();
    void showSettings();

private:
    explicit SearchViewmodel(QObject* parent = nullptr);
    void setIsSearching(bool searching);
    void setIsSearchTermValid(bool valid);
    void setFilesScanned(long files_scanned);
    void setDirsScanned(long dirs_scanned);

    long files_scanned_{0};
    long dirs_scanned_{0};

    QString search_term_{};
    QString search_path_{};
    bool is_search_term_valid_{true};
    long result_limit_{1000};
    bool use_regex_{false};
    bool case_sensitive_{false};

    Walker walker_;
    std::future<void> receieve_future_;

    SearchListModel list_model_;

    bool first_search_complete_{false};

    std::basic_regex<char> regex_;

    void ReceiveFiles();
    bool IsMatch(const std::filesystem::directory_entry& entry);
    bool SetRegex();

    RateLimitDelegate scan_count_limiter_{std::chrono::milliseconds(20)};
};




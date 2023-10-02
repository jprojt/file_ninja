#pragma once 
#include "fs/walker.h"
#include "waffle_listmodel.h"
#include <QObject>
#include <QtCore/qobjectdefs.h>
#include <future>


class AnalyseViewmodel: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged);
    Q_PROPERTY(bool firstRunComplete READ firstRunComplete NOTIFY firstRunCompleteChanged);
    Q_PROPERTY(long filesScanned READ filesScanned NOTIFY filesScannedChanged);
    Q_PROPERTY(long dirsScanned READ dirsScanned NOTIFY dirsScannedChanged);
    Q_PROPERTY(WaffleListModel* waffleListModel READ waffleListModel NOTIFY waffleListModelChanged);

public:
    static AnalyseViewmodel* GetInstance();
    ~AnalyseViewmodel();
    bool isRunning() const;
    bool firstRunComplete() const;
    long filesScanned() const;
    long dirsScanned() const;
    WaffleListModel* waffleListModel();

signals:
    void isRunningChanged();
    void waffleListModelChanged();
    void firstRunCompleteChanged();
    void filesScannedChanged();
    void dirsScannedChanged();

public slots:
    void start();
    void stop();
    QString bytesToString(long bytes) const;

private:
    explicit AnalyseViewmodel(QObject* parent = nullptr): QObject(parent) {}

    bool first_run_complete_{false};
    void setFirstRunComplete(bool complete);

    //waffle model
    WaffleListModel waffle_listmodel_;

    Walker walker_;

    std::future<void> future_;
    void ReceiveFiles();
};
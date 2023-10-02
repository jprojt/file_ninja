#pragma once 

#include <QObject>
#include <QVariant>
#include <QAbstractTableModel>
#include <QtCore/qnamespace.h>
#include <filesystem>


struct SearchListModelItem {
    std::string name;
    std::string path;
    long size;
};

class SearchListModel: public QAbstractTableModel {
    Q_OBJECT
    Q_ENUMS(Roles);

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        SizeRole,
    };

    explicit SearchListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void Clear();

    void Add(const std::filesystem::directory_entry& entry);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<std::filesystem::directory_entry> data_list_;
};
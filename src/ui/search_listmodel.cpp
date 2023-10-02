#include "search_listmodel.h"
#include "utils/formatter.h"
#include <QtCore/qconfig.h>
#include <iostream>


SearchListModel::SearchListModel(QObject* parent): QAbstractTableModel(parent) {}

int SearchListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return data_list_.size();
}

int SearchListModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant SearchListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= data_list_.size()) {
        return QVariant();
    }

    const auto& item = data_list_.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case 0:
                    return QString(item.path().filename().c_str());
                case 1:
                    return QString(item.path().parent_path().c_str());
                case 2:
                    return QString(formatter::FormatBytes(item.file_size()).c_str());
                default:
                    return QVariant();                
            }
        case NameRole:        
            return QString(item.path().filename().c_str());
        case PathRole:
            return QString(item.path().parent_path().c_str());
        case SizeRole:
            return QString(formatter::FormatBytes(item.file_size()).c_str());
        default:
            return QVariant();
    }
}

void SearchListModel::Clear() {
    beginResetModel();
    data_list_.clear();
    endResetModel();
}

void SearchListModel::Add(const std::filesystem::directory_entry& entry) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
#if (QT_VERSION_MAJOR == 6) 
    data_list_.emplace_back(std::move(entry));
#else
    data_list_.push_back(std::move(entry));
#endif

    endInsertRows();
}

QHash<int, QByteArray> SearchListModel::roleNames() const {
    return {
        {Qt::DisplayRole, "display"},
        {NameRole, "name"},
        {PathRole, "path"},
        {SizeRole, "size"},
    };
}
#include "waffle_listmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qglobal.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <mutex>
#include <vector>


WaffleListModel::WaffleListModel(QObject* parent) : 
        QAbstractListModel(parent),
        calculate_limiter_(std::chrono::milliseconds(30)) 
{
    Resize();
    calculate_limiter_.SetDelegate([&] {
        Calculate();
    });
}

int WaffleListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);

    return data_list_.size();
}

int WaffleListModel::size() const {
    return size_;
}

void WaffleListModel::setSize(int size) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (size == size_) {
        return;
    }
    int size_sq = size*size;
    int row_diff = size_sq - size_squared_;
    if (row_diff > 0) {
        beginInsertRows(QModelIndex(), size_squared_, size_sq-1);
    } else if (row_diff < 0) {
        beginRemoveRows(QModelIndex(), size_squared_+row_diff, size_squared_-1);
    }
    size_ = size;
    size_squared_ = size_sq;
    Resize();
    if (row_diff > 0) {
        endInsertRows();
    } else if (row_diff < 0) {
        endRemoveRows();
    }
    emit sizeChanged();
    lock.unlock();
}

void WaffleListModel::Resize() {
    data_list_.clear();
    for (int i = 0; i < size_squared_; i++) {
        data_list_.emplace_back(&other_datum_);
    }
    Calculate();
}

void WaffleListModel::Calculate() {
    std::vector<WaffleDatum*> temp_items;
    temp_items.reserve(data_.size()+1);

    for (auto& datum_entry : data_) {
        datum_entry.second.percentage = datum_entry.second.value / running_total_;
        temp_items.push_back(&datum_entry.second);
    }

    std::sort(temp_items.begin(), temp_items.end(), [](const WaffleDatum* a, const WaffleDatum* b) {
        return a->value > b->value;
    });

    
    const int grid_count = size_squared_;
    other_datum_.percentage = 0.0;
    other_datum_.value = 0.0;
    other_datum_.count = 0;
    int current_index = 0;
    bool only_others = false;
    int group_id = 0;

    for (auto* sorted_item : temp_items) {
        if (!only_others) {
            int number_slots = static_cast<int>(std::round(sorted_item->percentage*grid_count));
            if (number_slots > 0) {
                sorted_item->group_id = group_id++;
                for (int i = 0; i < number_slots; i++) {
                    data_list_[i+current_index] = sorted_item;
                }
                current_index += number_slots;
                continue;
            }
            only_others = true;    
        }

        other_datum_.percentage += sorted_item->percentage;
        other_datum_.value += sorted_item->value;
        other_datum_.count += sorted_item->count;
    }

    for (int i = current_index; i < data_list_.size(); i++) {
        data_list_[i] = &other_datum_;
    }

    emit dataChanged(index(0, 0), index(data_list_.size()-1, 0), {NameRole, PercentRole, ValueRole, GroupIdRole, IdRole, CountRole});
}

QVariant WaffleListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() < 0 || index.row() > data_list_.size()) {
        return QVariant();
    }

    const auto& item = data_list_.at(index.row());
    Roles real_role = static_cast<Roles>(role);
    switch (role) {
        case IdRole:
            return item->id;
        case NameRole:
            return item->name.c_str();
        case PercentRole:
            return item->percentage;
        case ValueRole:
            return item->value;
        case CountRole:
            return qlonglong(item->count);
        case GroupIdRole:
            return item->group_id;
        default:
            return QVariant();
    }
}

void WaffleListModel::Clear() {
    std::unique_lock<std::mutex> lock(mutex_);
    data_.clear();
    other_datum_.percentage = 0.0;
    other_datum_.value = 0;
    running_total_ = 0;
    last_group_id_ = 0;
    last_id_ = 0;
    Calculate();
    lock.unlock();
}

void WaffleListModel::Increment(std::string&& name, double value) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(name);
    WaffleDatum* item;
    if (it == data_.end()) {
        data_.emplace(name, std::move(WaffleDatum{std::move(name), ++last_id_, 0.0, ++last_group_id_}));
    } else {
        item = &it->second;
        item->value += value;
        item->count++;
    }

    //calculate
    running_total_ += value;
    calculate_limiter_.Update();
}

QHash<int, QByteArray> WaffleListModel::roleNames() const {
    return {
        {Qt::DisplayRole, "display"},
        {NameRole, "name"},
        {PercentRole, "percentage"},
        {ValueRole, "value"},
        {CountRole, "count"},
        {GroupIdRole, "groupId"},
        {IdRole, "id"},
    };
}

void WaffleListModel::ForceUpdate() {
    calculate_limiter_.ForceUpdate();
}
#pragma once 
#include "utils/rate_limit_delegate.h"
#include <QObject>
#include <QAbstractListModel>
#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qvariant.h>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


struct WaffleDatum {
    std::string name;
    float percentage{0.0};
    double value;
    int group_id;
    int id;
    long count{1};

    WaffleDatum(const std::string& name, int id, float value = 0.0, int group_id = 0)
        :   name(name), 
            value(value),
            group_id(group_id),
            id(id) {};
    WaffleDatum& operator=(const WaffleDatum& other) = delete;
    WaffleDatum(const WaffleDatum& other) = delete;
    WaffleDatum(WaffleDatum&&  other) {
        if (this == &other) {
            return;
        }
        this->name = std::move(other.name);
        this->group_id = other.group_id;
        this->percentage = other.percentage;
        this->value = other.value;
        this->id = other.id;
        this->count = other.count;
    }
    WaffleDatum& operator=(WaffleDatum&&  other) {
        if (this == &other) {
            return *this;
        }
        this->name = std::move(other.name);
        this->group_id = other.group_id;
        this->percentage = other.percentage;
        this->value = other.value;
        this->id = other.id;
        this->count = other.count;
        return *this;
    }
};

class WaffleListModel: public QAbstractListModel {
    Q_OBJECT;
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged);
    Q_ENUMS(Roles);

public:
    enum Roles {
        NameRole = Qt::UserRole+1,
        PercentRole,
        ValueRole,
        CountRole,
        GroupIdRole,
        IdRole,
    };

    explicit WaffleListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void Clear();

    void Increment(std::string&& name, double value);

    int size() const;
    void setSize(int size);
    void ForceUpdate();

signals:
    void sizeChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::unordered_map<std::string, WaffleDatum> data_;
    std::vector<WaffleDatum*> data_list_;
    WaffleDatum other_datum_{"Other", -1, 0.0, -1};

    int size_{30};
    int size_squared_{size_*size_};
    double running_total_{0};
    std::mutex mutex_;

    void Resize();
    void Calculate();

    bool rate_limit_change_event_{false};
    RateLimitDelegate calculate_limiter_;

    int last_group_id_{0};
    int last_id_{0};
};
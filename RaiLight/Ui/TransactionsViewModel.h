/*#pragma once

#include <QAbstractListModel>

class TransactionsViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
        int row, int column, const QModelIndex &parent) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    Qt::ItemFlags flags(const QModelIndex &index) const override;
};*/
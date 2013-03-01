#include "projectlistmodel.h"


ProjectListModel::ProjectListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ProjectListModel::rowCount(const QModelIndex &) const
{
    return fileCount;
}

QVariant ProjectListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}



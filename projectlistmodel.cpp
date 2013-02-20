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

bool ProjectListModel::canFetchMore(const QModelIndex &) const
{
    return false;
}

void ProjectListModel::fetchMore(const QModelIndex &)
{
    int remainder = fileCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(QModelIndex(), fileCount, fileCount+itemsToFetch-1);

    fileCount += itemsToFetch;

    endInsertRows();

    emit numberPopulated(itemsToFetch);
}

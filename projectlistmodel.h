#ifndef PROJECTLISTMODEL_H
#define PROJECTLISTMODEL_H

#include <QAbstractListModel>

class ProjectListModel : public QAbstractListModel{


    Q_OBJECT

    public:
        ProjectListModel(QObject *parent = 0);

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    signals:
        void numberPopulated(int number);

    public slots:

    protected:
        bool canFetchMore(const QModelIndex &parent) const;
        void fetchMore(const QModelIndex &parent);

    private:
        int fileCount;
};

#endif // PROJECTLISTMODEL_H

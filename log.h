#ifndef LOG_H
#define LOG_H

#include "db/QDjangoModel.h"
#include <QDateTime>

class Log : public QDjangoModel{

    Q_OBJECT

    //Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(QString row READ row WRITE setRow)
    Q_PROPERTY(QDateTime row_time READ row_time WRITE setRow_time)
    Q_PROPERTY(qint32 row_level READ row_level WRITE setRow_level)

    Q_CLASSINFO("__meta__", "db_table=log")

    //Q_CLASSINFO("id", "primary_key=true")
    //Q_CLASSINFO("row", "max_length=200")

    public:
    Log(QObject *parent=0)
        : QDjangoModel(parent)
    {

    }

    QString row() const{ return m_row; }
    void setRow(const QString &row){ m_row = row; }

    QDateTime row_time(){ return m_row_time; }
    void setRow_time(const QDateTime &row_time){ m_row_time = row_time; }

    qint16 row_level(){ return m_row_level; }
    void setRow_level(const qint16 &row_level){ m_row_level = row_level; }

    private:
        QString m_row;
        QDateTime m_row_time;
        qint16 m_row_level;
};

#endif // LOG_H

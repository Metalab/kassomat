#ifndef QSLOGDESTSQL_H
#define QSLOGDESTSQL_H

#include "QsLog/QsLogDest.h"
#include "model/log.h"
#include <QDateTime>


class QsLogDestSQL : public QsLogging::Destination{
public:

    QsLogDestSQL(){
        //m_log = Log();
    }

    void write(const QString& message, QsLogging::Level level) {
        Log row;
        row.setRow_time( QDateTime::currentDateTime() );
        row.setRow_level( level );
        row.setRow( message );
        row.save();
    }

    virtual bool isValid(){ // returns whether the destination was created correctly
        //dont know how to do that yet,...
        return true;
    }

private:

};



#endif // QSLOGDESTSQL_H

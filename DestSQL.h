#ifndef DESTSQL_H
#define DESTSQL_H

#include "log/QsLogDest.h"
#include "log.h"
#include <QDateTime>


class DestSQL : public QsLogging::Destination{
public:

    DestSQL(){
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



#endif // DESTSQL_H

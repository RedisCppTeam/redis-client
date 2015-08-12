#include "Command.h"
#include "CRedisClient.h"

string CRedisClient::bgrewriteaof()
{
    Command cmd( "BGREWRITEAOF" );
    string str;
    _getStatus(cmd,str);
    return str;
}

string CRedisClient::bgsave()
{
    Command cmd( "BGSAVE" );
    string str;
    _getStatus(cmd,str);
    return str;
}
string  CRedisClient::clientGetname()
{
    Command cmd( "CLIENT" );
    cmd<<"GETNAME";
    string str;
    _getString(cmd,str);
    return str;

}
bool CRedisClient::clientKill(const string& ip,const uint32_t port)
{
    Command cmd( "CLIENT" );
    cmd<<"KILL";
    cmd<<ip<<port;
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}
bool CRedisClient::clientList(VecString& clients)
{
    Command cmd( "CLIENT" );
    cmd<<"LIST";
    return   _getArry(cmd,clients);

}
bool CRedisClient::clientSetname (const string& connectionName)
{
    Command cmd( "CLIENT" );
    cmd<<"SETNAME";
    cmd<<connectionName;

    string status;

    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}
bool CRedisClient::configGet(const string& parameter,VecString& reply)
{
    Command cmd( "CONFIG" );
    cmd<<"GET";
    cmd<<parameter;
    return   _getArry(cmd,reply);
}
void CRedisClient::configResetstat()
{
    Command cmd( "CONFIG" );
    cmd<<"RESETSTAT";
    string status;
    _getStatus( cmd, status );
}
bool CRedisClient::configRewrite()
{
    Command cmd( "CONFIG" );
    cmd<<"REWRITE";
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );
}
bool CRedisClient::configSet(const string& parameter,const string& value)
{
    Command cmd( "CONFIG" );
    cmd<<"SET";
    cmd<<parameter<<value;
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );
}
uint64_t CRedisClient::dbsize()
{
    Command cmd( "DBSIZE" );
    int64_t num;
    _getInt(cmd,num);
    return num;

}
string CRedisClient::debugObject(const string& key)
{
    Command cmd( "DEBUG" );
    cmd<<"OBJECT";
    cmd<<key;
    string str;
    _getString(cmd,str);
    return str;
}
void CRedisClient::debugSegfault()
{
    Command cmd( "DEBUG" );
    cmd<<"SEGFAULT";
    _sendCommand(cmd);

}
void CRedisClient::flushall()
{
    Command cmd( "FLUSHALL" );
    string status;
    _getStatus( cmd, status );
}
void CRedisClient::flushdb()
{
    Command cmd( "FLUSHDB" );
    string status;
    _getStatus( cmd, status );
}

bool CRedisClient::info(VecString& reply)
{
    Command cmd( "INFO" );
    return _getArry(cmd,reply);

}
uint64_t CRedisClient::lastsave()
{
    Command cmd( "LASTSAVE" );
    int64_t num;
    _getInt(cmd,num);
    return num;

}

bool CRedisClient::save()
{
    Command cmd( "SAVE" );
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}

string CRedisClient::shutdown()
{
    Command cmd( "SHUTDOWN" );
    string status;
    _getStatus( cmd, status );
    return status;
}

void CRedisClient::slaveof(const string& host,const string& port)
{
    Command cmd( "SLAVEOF" );
    cmd<<host<<port;
    string status;
    _getStatus( cmd, status );
}

bool CRedisClient::slowlog(const CRedisClient::VecString &subcommand, CResult &reply)
{
    Command cmd( "SLOWLOG" );
    VecString::const_iterator it = subcommand.begin();
    VecString::const_iterator  end=subcommand.end();
    for ( ; it !=end; ++it )
    {
        cmd << *it;
    }
    return _getArry(cmd,reply);
}


void CRedisClient::time(string& currentseconds,string& microseconds)
{
    CResult result;
    Command cmd( "TIME" );
    _getArry(cmd,result);
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    currentseconds=it->getString();
    ++it;
    microseconds=it->getString();
}

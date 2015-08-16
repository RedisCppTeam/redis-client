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
void CRedisClient::clientList(CResult& result)
{
    Command cmd( "CLIENT" );
    cmd<<"LIST";
    _socket.clearBuffer();
    _sendCommand(cmd);
    _getReply(result);

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
uint64_t CRedisClient::configGet(const string& parameter,VecString& reply)
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
    if ( "OK"!=status)
        throw ProtocolErr( "CONFIG RESETSTAT: data recved is not OK" );
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
    _socket.clearBuffer();
    _sendCommand(cmd);

}
void CRedisClient::flushall()
{
    Command cmd( "FLUSHALL" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ProtocolErr( "FLUSHALL: data recved is not OK" );
}
void CRedisClient::flushdb()
{
    Command cmd( "FLUSHDB" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ProtocolErr( "FLUSHDB: data recved is not OK" );
}

uint64_t CRedisClient::info(VecString& reply)
{
    Command cmd( "INFO" );
    _socket.clearBuffer();
    _sendCommand(cmd);
    string line;
    _socket.readLine(line);
    int64_t sumLen = _valueFromString<int64_t>( line.substr(1) );
    int64_t len=0;
    while(len<sumLen)
    {
        _socket.readLine(line);
        reply.push_back(line);
        len+=line.length()+2;
    }
    return len;
}




uint64_t CRedisClient::lastsave()
{
    Command cmd( "LASTSAVE" );
    int64_t num;
    _getInt(cmd,num);
    return num;

}

void CRedisClient::monitor(void* input,void* output,void (*p)(string& str,void* in,void* out))
{
    string reply;
    Command cmd("MONITOR");
    _socket.clearBuffer();
    _socket.setReceiveTimeout(0);
    _sendCommand(cmd);
    if ((input==NULL) &&(p==NULL)&&(output==NULL))
        while(1)
        {
            _socket.readLine(reply);
            reply=reply.substr(1);
            std::cout<<reply<<std::endl;
        }
    while(1)
    {
        _socket.readLine(reply);
        reply=reply.substr(1);
        p(reply,input,output);
    }

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
    if ( "OK"!=status)
        throw ProtocolErr( "SLAVEOF: data recved is not OK" );
}

void CRedisClient::slowlog(const CRedisClient::VecString &subcommand, CResult &reply)
{
    Command cmd( "SLOWLOG" );
    VecString::const_iterator it = subcommand.begin();
    VecString::const_iterator  end=subcommand.end();
    for ( ; it !=end; ++it )
    {
        cmd << *it;
    }
    _sendCommand(cmd);
    _getReply(reply);
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

#include "Command.h"
#include "CRedisClient.h"
#include "ExceptRedis.h"

namespace Taiji {
namespace Redis {



uint64_t CRedisClient::stringToVecString(string& str, CRedisClient::VecString& vec)
{
    uint64_t i=0;
    uint64_t len=str.length();
    uint64_t posStart=0;
    while(i<len)
    {
        if (str[i]=='\n')
        {
            vec.push_back(str.substr(posStart,i-posStart));
            posStart=i+1;
        }
        ++i;
    }
    return vec.size();
}

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
bool  CRedisClient::clientGetname(string& clientName)
{
    Command cmd( "CLIENT" );
    cmd<<"GETNAME";
    return _getString(cmd,clientName);

}

void CRedisClient::clientKill(const string& ip, const uint16_t port)
{
    Command cmd( "CLIENT" );
    cmd<<"KILL";
    std::stringstream ss;
    ss << ip<<":"<< port;
    cmd<<ss.str();
    string status;
     _getStatus(cmd,status);
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}
/*
void CRedisClient::clientList(CResult& result)
{
    Command cmd( "CLIENT" );
    cmd<<"LIST";
    _socket.clearBuffer();
    _sendCommand(cmd);
    _getReply(result);

}
*/

uint64_t CRedisClient::clientList(CRedisClient::VecString & reply)
{
    Command cmd( "CLIENT" );
    cmd<<"LIST";
    string str;
    _getString(cmd,str);
    return stringToVecString(str,reply);
}

void CRedisClient::clientSetname(const string& connectionName)
{
    Command cmd( "CLIENT" );
    cmd<<"SETNAME";
    cmd<<connectionName;

    string status;

    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}

uint64_t CRedisClient::configGet(const string& parameter,CRedisClient::VecString& reply)
{
    Command cmd( "CONFIG" );
    cmd<<"GET";
    cmd<<parameter;

    uint64_t num = 0;
    _getArry(cmd,reply, num );
    return num;
}

void CRedisClient::configResetstat()
{
    Command cmd( "CONFIG" );
    cmd<<"RESETSTAT";
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}

void CRedisClient::configRewrite()
{
    Command cmd( "CONFIG" );
    cmd<<"REWRITE";
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}

void CRedisClient::configSet(const string& parameter,const string& value)
{
    Command cmd( "CONFIG" );
    cmd<<"SET";
    cmd<<parameter<<value;
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
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
    _getStatus(cmd,str);
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
        throw ExceptProtocol( "FLUSHALL: data recved is not OK" );
}

void CRedisClient::flushdb()
{
    Command cmd( "FLUSHDB" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "FLUSHDB: data recved is not OK" );
}

void CRedisClient::info(CRedisClient::VecString& reply,const string& section)
{
    Command cmd( "INFO" );
    string str;
    if(""!=section)
        cmd << section;
    _getString(cmd,str);
    stringToVecString(str,reply);
}


uint64_t CRedisClient::lastsave()
{
    Command cmd( "LASTSAVE" );
    int64_t num;
    _getInt(cmd,num);
    return num;
}

void CRedisClient::monitorStart( void )
{
     Command cmd("MONITOR");
    _socket.clearBuffer();
    _sendCommand(cmd);
}
bool CRedisClient::monitorRead(std::string &value, uint64_t timeout )
{
    _socket.setReceiveTimeout( timeout );

    try{
        _socket.readLine( value );
        return true;
    }
    catch( ... )
    {
        sleep(1);
        return false;
    }
}

void CRedisClient::monitorStop( void )
{
    string info;
    shutdown( info );
    sleep(1);
    reconnect();
}


void CRedisClient::save()
{
    Command cmd( "SAVE" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "SAVE: data recved is not OK" );
}

bool CRedisClient::shutdown( string& status )
{
    Command cmd( "SHUTDOWN" );
    try
    {
        _getStatus( cmd, status );
        return false;
    }catch( ExceptRedis&e )
    {
        e.what();
        return true;
    }
}

void CRedisClient::slaveof(const string& host, const uint16_t port)
{
    Command cmd( "SLAVEOF" );
    cmd<<host<<port;
    string status;
    _getStatus( cmd, status );
    std::cout<<status<<std::endl;
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}

void CRedisClient::slowlog(const CRedisClient::VecString &subcommand, CResult &reply)
{
    Command cmd( "SLOWLOG" );
    _socket.clearBuffer();
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



}
}

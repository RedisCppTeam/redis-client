
#include "Command.h"
#include "CRedisClient.h"




void CRedisClient::sadd(const string &key, const CRedisClient::VecString &members, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "SADD" );
    cmd << key;
    VecString::const_iterator it = members.begin();
    VecString::const_iterator end = members.end();

    for ( ; it != end ; ++it )
    {
        cmd << *it;
    }

    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::sadd(const string &key, const CRedisClient::VecString &members)
{
    CResult result;
    sadd( key, members, result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "SADD: data recved is not interger" );
    }

    return result.getInt();
}

void CRedisClient::scard(const string &key, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "SCARD" );
    cmd << key;
    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::scard(const string &key)
{
    CResult result;
    scard( key, result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "SCARD: data recved is not interger");
    }
    return result.getInt();
}



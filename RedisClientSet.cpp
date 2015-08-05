
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
        throw ProtocolErr( "SADD: data recved is not intergerer" );
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
        throw ProtocolErr( "SCARD: data recved is not intergerer");
    }
    return result.getInt();
}



void CRedisClient::sdiff(const CRedisClient::VecString &keys, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "SDIFF" );
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::sdiff(const CRedisClient::VecString &keys, CRedisClient::VecString &values)
{
    CResult result;
    sdiff( keys, result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_ARRAY != type )
    {
        throw ProtocolErr( "SDIFF: data recved is not arry");
    }

    _getValueFromArry( result.getArry(), values );
    return values.size();
}

void CRedisClient::sdiffstore(const string &newKey, const CRedisClient::VecString &keys, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "SDIFFSTORE" );
    cmd << newKey;
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::sdiffstore(const string &newKey, const CRedisClient::VecString &keys )
{
    CResult result;
    sdiffstore( newKey,keys, result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "SDIFFSTORE: data recved is not integerer");
    }
    return result.getInt();
}

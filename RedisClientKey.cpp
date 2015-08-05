/**
 *
 * @file	RedisClientKey.cpp
 * @brief CRedisClient key 操作的代码实现。
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */

#include "Command.h"
#include "CRedisClient.h"


void CRedisClient::keys(const std::string &pattern, CResult &result )
{
    _socket.clearBuffer();

    Command cmd( "KEYS" );
    cmd << pattern;

    _sendCommand( cmd );

   _getReply( result );
}

int64_t CRedisClient::keys(const std::string &pattern, VecString &values )
{
    CResult result;
    keys( pattern, result );
    ReplyType type = result.getType();

    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }

    if ( type != REDIS_REPLY_ARRAY )
    {
        throw ProtocolErr( "KEYS: data recved is not arry");
    }

    _getValueFromArry( result.getArry(), values );
    return values.size();
}

void CRedisClient::del( CRedisClient::VecString &keys, CResult& result )
{
    _socket.clearBuffer();

    Command cmd( "DEL" );
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();

    for ( ; it != end; ++it )
    {
        cmd << *it ;
    }

    _sendCommand( cmd );
    _getReply( result );
}

int64_t CRedisClient::del(CRedisClient::VecString &keys )
{
    CResult result;
    del( keys, result );

    ReplyType type = result.getType();

    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }

    if ( type != REDIS_REPLY_INTEGERER )
    {
        throw ProtocolErr( "DEL: data recved is not integerer");
    }

    return result.getInt();
}







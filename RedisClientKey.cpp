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

int64_t CRedisClient::keys(const std::string &pattern, VecString &value )
{
    CResult result;
    keys( pattern, result );
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    ReplyType type = result.getType();

    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }

    if ( type != REDIS_REPLY_ARRAY )
    {
        throw ProtocolErr( "KEYS: data recved is not arry");
    }

    for ( ; it != result.getArry().end(); it++ )
    {
        value.push_back( result.getString() );
    }
    return value.size();
}

void CRedisClient::del( CRedisClient::VecString &keys, CResult& result )
{
    _socket.clearBuffer();

    Command cmd( "DEL" );
    VecString::const_iterator it = keys.begin();

    for ( ; it != keys.end(); it++ )
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







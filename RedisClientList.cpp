/**
 * @file	RedisClientList.cpp
 * @brief CRedisClient 的 list 方法。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 * 修订说明:初始版本
 */


#include "Command.h"
#include "CRedisClient.h"

void CRedisClient::lpush(const std::string &key, const VecString& value, CResult& result )
{
    _socket.clearBuffer();

    Command cmd("LPUSH");
    cmd << key;

    VecString::const_iterator it = value.begin();
    VecString::const_iterator end = value.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }
    _sendCommand( cmd );
    _getReply( result );
}


int64_t CRedisClient::lpush(const std::string &key, const VecString &value)
{
    CResult result;

    lpush( key, value, result );

    ReplyType type = result.getType();
    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }

    if ( type != REDIS_REPLY_INTEGERER )
    {
        throw ProtocolErr( "LPUSH: data recved is not integerer");
    }

    return result.getInt();
}



void CRedisClient::lpop( const std::string& key, CResult& result )
{
    _socket.clearBuffer();

    Command cmd( "LPOP" );
    cmd << key;

    _sendCommand( cmd );

    _getReply( result );
}

bool CRedisClient::lpop(const std::string &key, std::string &value)
{
    CResult result;
    lpop( key, result );

    ReplyType type = result.getType();
    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( result.getType() == REDIS_REPLY_STRING )
    {
        value = result.getString();
        return true;
    }else
    {
        throw ProtocolErr( "LPOP: data recved is not string" );
    }
}




/**
 * @file	RedisClientString.cpp
 * @brief CRedisClient 的 String 方法。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 * 修订说明:初始版本
 */

#include "Command.h"
#include "CRedisClient.h"

//-----------------------------string method--------------------------------------
void CRedisClient::set(const std::string &key, const std::string &value)
{
    _socket.clearBuffer();

    Command cmd( "SET" );
    cmd << key << value;
   _sendCommand( cmd );
    DEBUGOUT( "send" ,string( cmd ) );
    _replyOk();
}

void CRedisClient::get(const std::string &key, std::string &value)
{
    _socket.clearBuffer();

    Command cmd( "GET" );
    cmd << key ;
    _sendCommand( cmd );
    DEBUGOUT( "send", string( cmd ) );
    value = _replyBulk();
    return;
}



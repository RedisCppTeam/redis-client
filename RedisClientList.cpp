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


uint64_t CRedisClient::lpush(const std::string &key, const std::string &value)
{
    _socket.clearBuffer();

    Command cmd( "LPUSH" );
    cmd << key << value;
    _sendCommand( cmd );
    return _replyInt();
}

uint64_t CRedisClient::lpush(const std::string &key, const VecString& value)
{
    _socket.clearBuffer();

    Command cmd("LPUSH");
    VecString::const_iterator it = value.begin();
    cmd << key;
    for ( ; it !=value.end(); it++ )
    {
        cmd << *it;
    }
    _sendCommand( cmd );
    return _replyInt();
}





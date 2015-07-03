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


uint64_t CRedisClient::keys(const std::string &pattern, CRedisClient::VecString &keys )
{
    _socket.clearBuffer();

    Command cmd( "KEYS" );
    cmd << pattern;

    _sendCommand( cmd );

    return _replyMultiBulk( keys );
}


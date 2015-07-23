/**
 *
 * @file	RedisClientHash.cpp
 * @brief CRedisClient hash 操作的代码实现。
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */



#include "Command.h"
#include "CRedisClient.h"



//------------------------------hash method-----------------------------------

uint8_t CRedisClient::hset(const std::string &key, const std::string &field, const std::string &value)
{
    _socket.clearBuffer();

    Command cmd( "HSET" );
    cmd << key << field << value;

    _sendCommand( cmd );
    return _replyInt();
}

int8_t CRedisClient::hget(const std::string &key, const std::string &field,  CResult &value)
{
    _socket.clearBuffer();

    Command cmd( "HGET" );
    cmd << key << field ;

    _sendCommand( cmd );

    return _replyBulk( value );
}

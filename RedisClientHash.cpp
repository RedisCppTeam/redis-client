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

void CRedisClient::hset(const std::string &key, const std::string &field, const std::string &value, CResult &result)
{
    _socket.clearBuffer();

    Command cmd( "HSET" );
    cmd << key << field << value;

    _sendCommand( cmd );

    _getReply( result );
}

uint8_t CRedisClient::hset(const std::string &key, const std::string &field, const std::string &value)
{
    CResult result;

   hset( key, field, value, result );

   if ( result.getType() == REDIS_REPLY_ERROR )
   {
        throw ReplyErr( result.getErrorString() );
   }

   if ( result.getType() != REDIS_REPLY_INTEGERER )
   {
       throw ProtocolErr( "HSET: data recved is not integerer" );
   }
   return result.getInt();
}




void CRedisClient::hget(const std::string &key, const std::string &field,  CResult &result )
{
    _socket.clearBuffer();

    Command cmd( "HGET" );
    cmd << key << field ;

    _sendCommand( cmd );

     _getReply( result );
}

bool CRedisClient::hget( const std::string &key, const std::string &field, string &value )
{
    CResult result;
    hget( key, field, result );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }

    if ( result.getType() == REDIS_REPLY_NIL )
    {
            return false;
    }

    if ( result.getType() == REDIS_REPLY_STRING )
    {
        value = result.getString();
        return true;
    }else
    {
          throw ProtocolErr( "HSET: data recved is not string" );
    }
}



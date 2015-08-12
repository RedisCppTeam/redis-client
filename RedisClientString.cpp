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
void CRedisClient::_set(const string &key, const string &value, CResult &result, const string& suffix , long time,const string suffix2 )
{
    _socket.clearBuffer();

    Command cmd( "SET" );
    cmd << key << value;

    if ( suffix != "" )
    {
        cmd << suffix;
    }

    if ( time != 0 )
    {
        std::stringstream ss;
        ss << time ;
        cmd << ss.str();
    }
    if ( suffix2 != "" )
    {
        cmd << suffix2;
    }
    _sendCommand( cmd );
    _getReply( result );
}

void CRedisClient::_set(const std::string &key,const std::string &value)
{
    CResult result;
    _set( key, value, result );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( result.getType() == REDIS_REPLY_STATUS )
    {
        if ( result.getStatus() == "OK" )
        {
            return;
        }else
        {
            throw ProtocolErr( "SET: data recved is not OK" );
        }
    }else
    {
        throw ProtocolErr( "SET: data recved is not status" );
    }
}

bool CRedisClient::setEX(const std::string &key, const std::string &value, long time, SET_OPTION opt )
{
    string suffix;
    if ( DEFAULT == opt )
    {
        suffix.clear();
    }
    else if ( NX == opt )
    {
        suffix = "NX";
    }else if ( XX == opt )
    {
        suffix = "XX";
    }else
    {
        return false;
    }

    CResult result;
    _set( key, value, result,"EX", time,suffix );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }
    else if ( result.getType() == REDIS_REPLY_STATUS )
    {
       return true;
    }else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else
    {
        throw ProtocolErr( "SET: data recved is not status" );
    }
}

bool CRedisClient::setPX(const std::string &key, const std::string &value, long time, SET_OPTION opt)
{
    string suffix;
    if ( DEFAULT == opt )
    {
        suffix.clear();
    }
    else if ( NX == opt )
    {
        suffix = "NX";
    }else if ( XX == opt )
    {
        suffix = "XX";
    }else
    {
        return false;
    }

    CResult result;
    _set( key, value, result,"PX", time,suffix );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }
    else if ( result.getType() == REDIS_REPLY_STATUS )
    {
       return true;
    }else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else
    {
        throw ProtocolErr( "SET: data recved is not status" );
    }
}

bool CRedisClient::setNX(const std::string &key, const std::string &value)
{
    CResult result;
    _set( key, value, result,"NX" );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
       throw ReplyErr( result.getErrorString() );
    } else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( result.getType() == REDIS_REPLY_STATUS )
    {
        return true;
    }else
    {
        throw ProtocolErr( "SET: data recved is not status" );
    }
}

bool CRedisClient::setXX(const std::string &key, const std::string &value)
{
    CResult result;
    _set( key, value, result,"XX" );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
       throw ReplyErr( result.getErrorString() );
    } else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( result.getType() == REDIS_REPLY_STATUS )
    {
        return true;
    }else
    {
        throw ProtocolErr( "SET: data recved is not status" );
    }
}


void CRedisClient::get(const std::string &key, CResult& result )
{
    _socket.clearBuffer();

    Command cmd( "GET" );
    cmd << key ;
    _sendCommand( cmd );

    _getReply( result );
}



bool CRedisClient::get( const std::string &key, std::string &value )
{
    value.clear();
    CResult result;
    get( key, result );

    ReplyType type = result.getType();
    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( type == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( type == REDIS_REPLY_STRING )
    {
        value = result.getString();
        return true;
    }else
    {
        throw ProtocolErr( "GET: data recved is not string" );
    }
}







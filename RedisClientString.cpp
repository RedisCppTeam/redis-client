/**
 * @file	RedisClientString.cpp
 * @brief the String method of the CRedisClient
 * @author: 		yp
 * @date: 		June 14, 2015
 * Revision Description: initial version
 */

#include "Command.h"
#include "CRedisClient.h"

//-----------------------------string method--------------------------------------



uint64_t CRedisClient::append( const string& key, const string& value )
{
	Command cmd( "APPEND" );
	cmd << key << value;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


uint64_t CRedisClient::bitcount( const string& key, int64_t start, int64_t end )
{
	Command cmd( "BITCOUNT" );
	cmd << key << start << end;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


uint64_t CRedisClient::bitop( const string& operation, const string& destkey, VecString& keys )
{
	Command cmd( "BITOP" );
	cmd << operation << destkey;
	VecString::const_iterator it = keys.begin();
	for ( ; it != keys.end(); ++it )
	{
		cmd << *it;
	}
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


int64_t CRedisClient::decr( const string& key )
{
	Command cmd( "DECR" );
	cmd << key;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}




int64_t CRedisClient::decrby( const string& key, int64_t decrement )
{
	Command cmd( "DECRBY" );
	cmd << key << decrement;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


bool CRedisClient::get( const std::string &key, std::string &value )
{
    Command cmd( "GET" );
    cmd << key;
    return _getString( cmd, value );
}


uint8_t CRedisClient::getbit( const string& key, uint32_t offset )
{
	Command cmd( "GETBIT" );
	cmd << key << offset;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}



bool CRedisClient::getrange( const string& key, int64_t start, int64_t end, string &value )
{
	value.clear();
	Command cmd( "GETRANGE" );
	cmd << key << start << end;

	return _getString( cmd, value );
}


bool CRedisClient::getset(const string& key, const string &value, string &oldvalue )
{
	oldvalue.clear();
	Command cmd( "GETSET" );
	cmd << key << value;
	return _getString( cmd, oldvalue );
}


int64_t CRedisClient::incr( const string& key )
{
	Command cmd( "INCR" );
	cmd << key;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


int64_t CRedisClient::incrby( const string& key, int64_t increment )
{
	Command cmd( "INCRBY" );
	cmd << key << increment;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


bool CRedisClient::incrbyfloat( const string& key, float increment, float& value )
{
	Command cmd( "INCRBYFLOAT" );
	cmd << key << increment;

	string strVal;
	bool ret = _getString( cmd, strVal );
	value = _valueFromString<float>( strVal );

	return ret;
}


void CRedisClient::mget(VecString& keys, CResult& result )
{
	Command cmd( "MGET" );
	VecString::const_iterator it = keys.begin();
	for ( ; it != keys.end(); ++it )
	{
		cmd << *it;
	}

	_getArry( cmd, result );
}




void CRedisClient::mset( CRedisClient::TupleString &value )
{
	Command cmd( "MSET" );
	CRedisClient::TupleString::const_iterator it = value.begin();
	for ( ; it != value.end(); ++it )
	{
        cmd << std::get<0>(*it);
        cmd << std::get<1>(*it);
	}

	string status;
	_getStatus( cmd, status );
	if ( status != "OK" )
		throw ProtocolErr( "MSET: data recved is not OK" );
}




uint8_t CRedisClient::msetnx( CRedisClient::TupleString &value )
{
	Command cmd( "MSETNX" );
	CRedisClient::TupleString::const_iterator it = value.begin();
	for ( ; it != value.end(); ++it )
	{
        cmd << std::get<0>(*it);
        cmd << std::get<1>(*it);
	}

	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}








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

void CRedisClient::set(const std::string &key,const std::string &value)
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


uint8_t CRedisClient::setbit_( const string& key, uint32_t offset, const string& value )
{
	Command cmd( "SETBIT" );
	cmd << key << offset << value;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


uint64_t CRedisClient::setrange( const string& key, uint32_t offset, const string& value )
{
	Command cmd( "SETRANGE" );
	cmd << key << offset << value;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


uint64_t CRedisClient::strlen( const string& key )
{
	Command cmd( "STRLEN" );
	cmd << key;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}







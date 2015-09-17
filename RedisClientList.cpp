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

uint64_t CRedisClient::lpush( const string &key , const VecString &value )
{
	Command cmd("LPUSH");
	cmd << key;

	VecString::const_iterator it = value.begin();
	VecString::const_iterator end = value.end();
	for ( ; it != end ; ++it )
	{
		cmd << *it;
	}

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

uint64_t CRedisClient::rpush( const string &key , const VecString &value )
{
	Command cmd("RPUSH");
	cmd << key;

	VecString::const_iterator it = value.begin();
	VecString::const_iterator end = value.end();
	for ( ; it != end ; ++it )
	{
		cmd << *it;
	}

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

uint64_t CRedisClient::rpushx( const string& key , const string& value )
{
	Command cmd("RPUSHX");
	cmd << key;
	cmd << value;

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

uint64_t CRedisClient::lpushx( const string& key , const string& value )
{
	Command cmd("LPUSHX");
	cmd << key;
	cmd << value;

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::lpop( const string &key , std::string &value )
{
	Command cmd("LPOP");
	cmd << key;

	return _getString(cmd, value);
}

bool CRedisClient::rpop( const string &key , std::string &value )
{
	Command cmd("RPOP");
	cmd << key;

	return _getString(cmd, value);
}

bool CRedisClient::lindex( const string &key ,int64_t index , std::string &value )
{
	Command cmd("LINDEX");
	cmd << key;
	cmd << index;

	return _getString(cmd, value);
}

uint64_t CRedisClient::llen( const string& key )
{
	Command cmd("LLEN");
	cmd << key;

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

int64_t CRedisClient::linsert(const string& key , WHERE where , const string &pivot ,
        const string &value )
{
    string realWhere;
	Command cmd("LINSERT");
    if ( where == BEFORE )
    {
        realWhere = "before";
    }else
    {
        realWhere = "after";
    }
    cmd << key << realWhere << pivot << value;

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

uint64_t CRedisClient::lrem(const string &key  , int64_t count , const string &value )
{
	Command cmd("LREM");
	cmd << key << count << value;

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

uint64_t CRedisClient::lrem(const string &key  , const string &value )
{
    return ( lrem( key, 0, value ) );
}

void CRedisClient::ltrim( const string &key , int64_t start , int64_t stop )
{
	Command cmd("LTRIM");
	cmd << key << start << stop;

	string status;
    _getStatus(cmd, status);
}

void CRedisClient::lset( const string &key , uint64_t index , const string &value )
{
	Command cmd("LSET");
	cmd << key << index << value;

	string status;
    _getStatus(cmd, status);
}

bool CRedisClient::rpoplpush( const string &source , const string &dest , string &value )
{
	Command cmd("RPOPLPUSH");
	cmd << source << dest;

	return _getString(cmd, value);
}

uint64_t CRedisClient::lrange( const string &key , int64_t start , int64_t stop ,
		CRedisClient::VecString &value )
{
	Command cmd("LRANGE");
	cmd << key << start << stop;

    uint64_t num = 0;
    _getArry(cmd, value, num);
    return num;
}

bool CRedisClient::blpop( const CRedisClient::VecString &key , uint64_t &timeout ,
		CRedisClient::MapString &value )
{
	Command cmd("BLPOP");
	CRedisClient::VecString::const_iterator it = key.begin();
	CRedisClient::VecString::const_iterator end = key.end();
	for ( ; it != end ; it++ )
	{
		cmd << *it;
	}
	cmd << timeout;

    uint64_t num = 0;
    return ( _getArry(cmd, value,num) );
}

bool CRedisClient::brpop( const CRedisClient::VecString &key , uint64_t &timeout ,
		CRedisClient::MapString &value )
{
	Command cmd("BRPOP");
	CRedisClient::VecString::const_iterator it = key.begin();
	CRedisClient::VecString::const_iterator end = key.end();
	for ( ; it != end ; it++ )
	{
		cmd << *it;
	}
	cmd << timeout;

    uint64_t num = 0;
    return _getArry(cmd, value,num);
}

bool CRedisClient::brpoplpush(const string &source , const string &dest , uint64_t timeout ,
        string &value )
{
	value.clear();
	Command cmd("BRPOPLPUSH");
    cmd << source << dest << timeout;

    return (_getString( cmd, value ));
}


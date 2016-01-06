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
   Command cmd( "HSET" );
   cmd << key << field << value;
   int64_t num = 0;
   _getInt( cmd , num );
   return num;
}



bool CRedisClient::hget( const std::string &key, const std::string &field, string &value )
{
    Command cmd( "HGET" );
    cmd << key << field;
    return _getString( cmd , value );
}


uint64_t CRedisClient::hdel( const string &key, const CRedisClient::VecString &fields )
{
    Command cmd( "HDEL" );
    cmd << key;

    VecString::const_iterator it = fields.begin();
    VecString::const_iterator end = fields.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    int64_t num = 0;
    _getInt( cmd , num );
    return num;
}


bool CRedisClient::hexists(const string &key, const string &field)
{
    Command cmd( "HEXISTS" );
    cmd << key << field;
    int64_t num = 0;
    _getInt( cmd, num );
    return ( num==1 ? true:false );
}


uint64_t CRedisClient::hgetall(const string &key, CRedisClient::TupleString &pairs)
{
    Command cmd( "HGETALL" );
    cmd << key;

    uint64_t num = 0;
    _getArry( cmd, pairs, num );
    return num;
}


int64_t CRedisClient::hincrby(const string &key, const string &field, int64_t increment)
{
    Command cmd( "HINCRBY" );
    cmd << key << field << increment;

    int64_t num = 0;
    _getInt( cmd, num );
    return num;
}


double CRedisClient::hincrbyfloat(const string &key, const string &field, float increment)
{
    Command cmd( "HINCRBYFLOAT" );
    cmd << key << field << increment;
    string value;
    _getString( cmd , value );
    return _valueFromString<double>(  value );
}



uint64_t CRedisClient::hkeys(const string &key, CRedisClient::VecString &values)
{
    Command cmd( "HKEYS" );
    cmd << key;

    uint64_t num = 0;
    _getArry( cmd, values, num );
    return num;
}


uint64_t CRedisClient::hlen(const string &key)
{
   Command cmd( "HLEN" );
   cmd << key;
   int64_t num = 0;
  _getInt( cmd, num );
  return num;
}

void CRedisClient::hmget(const string &key, const CRedisClient::VecString &fields, CResult &result)
{
    Command cmd( "HMGET" );
    cmd << key;

    VecString::const_iterator it = fields.begin();
    VecString::const_iterator end = fields.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd , result );
}

void CRedisClient::hmset(const string &key, const CRedisClient::TupleString &pairs)
{
    Command cmd( "HMSET" );
    cmd << key;
    TupleString::const_iterator it = pairs.begin();
    TupleString::const_iterator end = pairs.end();

    for ( ; it !=end ; ++it )
    {
        cmd << std::get<0>(*it);
        cmd << std::get<1>(*it);
    }
    string status;
    _getStatus( cmd, status );
}


bool CRedisClient::hsetnx(const string &key, const string &field, const string &value)
{
    Command cmd( "HSETNX" );
    cmd << key << field << value;
    int64_t num = 0;
    _getInt( cmd, num );
    return ( num==1 ? true:false );
}


uint64_t CRedisClient::hvals(const string &key, CRedisClient::VecString &values)
{
    Command cmd( "HVALS" );
    cmd << key;

    uint64_t num;
    _getArry( cmd, values, num );
    return num;
}


bool CRedisClient::hscan(const string &key, int64_t& cursor, TupleString &values, const string &match, uint64_t count )
{
        CResult result;
        Command cmd( "HSCAN" );
        cmd << key << cursor;

        if ( "" != match )
        {
              cmd << "MATCH" << match;
        }

        if ( 0 != count )
        {
               cmd << "COUNT" << count;
        }

        _getArry( cmd, result );
        CResult::ListCResult::const_iterator it = result.getArry().begin();
       cursor = _valueFromString<uint64_t>( it->getString() );
       ++it;
       _getStringTupleFromArry( it->getArry(), values );
       return ( cursor == 0 ? false : true );

}





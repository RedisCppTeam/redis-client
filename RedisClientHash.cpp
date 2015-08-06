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


uint64_t CRedisClient::hgetall(const string &key, CRedisClient::MapString &pairs)
{
    Command cmd( "HGETALL" );
    cmd << key;

    CResult result;
    _getArry( cmd , result );
    _getStringMapFromArry( result.getArry(), pairs );
    return pairs.size();
}

void CRedisClient::hincrby(const string &key, const string &field, uint64_t increment, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "HINCRBY" );
    cmd << key << field << increment;
    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::hincrby(const string &key, const string &field, uint64_t increment)
{
    CResult result;
    hincrby( key, field, increment,result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "HINCRBY: data recved is not intgerer" );
    }
    return result.getInt();
}

void CRedisClient::hincrbyfloat(const string &key, const string &field, float increment, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "HINCRBYFLOAT" );
    cmd << key << field << increment;
    _sendCommand( cmd );
    _getReply( result );
}

float CRedisClient::hincrbyfloat(const string &key, const string &field, float increment)
{
    CResult result;
    hincrbyfloat( key, field, increment,result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_STRING != type )
    {
        throw ProtocolErr( "HINCRBYFLOAT: data recved is not string" );
    }
    return _valueFromString<float>(  result.getString() );
}

void CRedisClient::hkeys(const string&key, CResult &result)
{
   _socket.clearBuffer();
   Command cmd( "HKEYS" );
   cmd << key;
   _sendCommand( cmd );
   _getReply( result );
}

uint64_t CRedisClient::hkeys(const string &key, CRedisClient::VecString &values)
{
    CResult result;
    hkeys( key, result );

    ReplyType type = result.getType();
   if ( REDIS_REPLY_ERROR == type )
   {
       throw ReplyErr( result.getErrorString() );
   }else if ( REDIS_REPLY_ARRAY != type )
   {
        throw ProtocolErr("HKEYS: data recved is not arry");
   }

   _getStringVecFromArry( result.getArry(), values );
   return values.size();
}

void CRedisClient::hlen(const string &key, CResult &result)
{
   _socket.clearBuffer();
   Command cmd( "HLEN" );
   cmd << key;
   _sendCommand( cmd );
   _getReply( result );
}

uint64_t CRedisClient::hlen(const string &key)
{
   CResult result;
   hlen( key, result );

   ReplyType type = result.getType();
   if ( REDIS_REPLY_ERROR == type )
   {
        throw ReplyErr( result.getErrorString() );
   }else if ( REDIS_REPLY_INTEGERER != type )
   {
        throw ProtocolErr( "HLEN: data recved is not intergerer" );
   }
   return result.getInt();
}

void CRedisClient::hmget(const string &key, const CRedisClient::VecString &fields, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "HMGET" );
    cmd << key;

    VecString::const_iterator it = fields.begin();
    VecString::const_iterator end = fields.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _sendCommand( cmd );
    _getReply( result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_ARRAY != type )
    {
        throw ProtocolErr( "HMGET: data recved is not arry" );
    }
}

void CRedisClient::hmset(const string &key, const CRedisClient::MapString &pairs, CResult &result)
{
    _socket.clearBuffer();;
    Command cmd( "HMSET" );
    cmd << key;
    MapString::const_iterator it = pairs.begin();
    MapString::const_iterator end = pairs.end();

    for ( ; it !=end ; ++it )
    {
        cmd << it->first;
        cmd << it->second;
    }

    _sendCommand( cmd );
    _getReply( result );
}

void CRedisClient::hmset(const string &key, const CRedisClient::MapString &pairs)
{
    CResult result;
    hmset( key, pairs ,result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_STATUS != type )
    {
        throw ProtocolErr( "HMSET: data recved is not status" );
    }
}

void CRedisClient::hsetnx(const string &key, const string &field, const string &value, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "HSETNX" );
    cmd << key << field << value;

    _sendCommand( cmd );
    _getReply( result );
}

bool CRedisClient::hsetnx(const string &key, const string &field, const string &value)
{
    CResult result;
    hsetnx( key, field, value,result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "HSETNX: data recved is not intergerer" );
    }
    return ( result.getInt()==1?true:false );
}

void CRedisClient::hvals(const string &key, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "HVALS" );
    cmd << key ;
    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::hvals(const string &key, CRedisClient::VecString &values)
{
    CResult result;
    hvals( key, result );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_ARRAY != type )
    {
        throw ProtocolErr( "HVALS: data recved is not arry");
    }
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    CResult::ListCResult::const_iterator end = result.getArry().end();
    for ( ; it != end; ++it )
    {
        values.push_back( static_cast<string>(*it) );
    }
    return values.size();
}

void CRedisClient::hscan(const string &key, int64_t cursor, const string &match, uint64_t count, CResult &result)
{
    _socket.clearBuffer();
    Command cmd( "HSCAN" );
    cmd << key << cursor;

    if ( "" != match )
    {
          cmd << "MATH" << match;
    }

    if ( 0 != count )
    {
           cmd << "COUNT" << count;
    }

    _sendCommand( cmd );
    _getReply( result );
}

bool CRedisClient::hscan(const string &key, int64_t cursor, VecString& values,const string &match, uint64_t count )
{
    static uint64_t lastCur = 0;
    uint64_t realCur = 0;
    CResult result;

    if ( cursor >= 0 )
    {
        realCur = cursor;
    }else
    {
        realCur = lastCur;
    }

    hscan( key, realCur, match, count, result );
    ReplyType type = result.getType() ;
    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_ARRAY != type )
    {
         throw ProtocolErr( "HSCAN: data recved is not arry" );
    }
    CResult::ListCResult::const_iterator it = result.getArry().begin();

    if ( REDIS_REPLY_STRING != it->getType() )
    {
        throw ProtocolErr( "HSCAN: first ele is not string" );
    }

    lastCur = _valueFromString<uint64_t>( it->getString() );
    ++it;

    _getStringVecFromArry( it->getArry(), values );

    return ( lastCur == 0 ? false : true );
}








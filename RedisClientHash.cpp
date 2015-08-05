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
   ReplyType type = result.getType();
   if ( type == REDIS_REPLY_ERROR )
   {
        throw ReplyErr( result.getErrorString() );
   }

   if ( type != REDIS_REPLY_INTEGERER )
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
    ReplyType type = result.getType();
    if ( type == REDIS_REPLY_ERROR )
    {
        throw ReplyErr( result.getErrorString() );
    }

    if ( type == REDIS_REPLY_NIL )
    {
            return false;
    }

    if ( type == REDIS_REPLY_STRING )
    {
        value = result.getString();
        return true;
    }else
    {
          throw ProtocolErr( "HSET: data recved is not string" );
    }
}

void CRedisClient::hdel(const string &key, const CRedisClient::VecString &fields, CResult &result )
{
    _socket.clearBuffer();;
    Command cmd( "HDEL" );
    cmd << key;

    VecString::const_iterator it = fields.begin();
    VecString::const_iterator end = fields.begin();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::hdel( const string &key, const CRedisClient::VecString &fields )
{
    CResult result;
    hdel( key, fields, result );

    ReplyType type = result.getType();

    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "HDEL: data recv is not intgerer" );
    }
    return result.getInt();
}

void CRedisClient::hexists(const string &key, const string &field, CResult &result)
{
    _socket.clearBuffer();;
    Command cmd( "HEXISTS" );
    cmd << key << field;
    _sendCommand( cmd );
    _getReply( result );
}

bool CRedisClient::hexists(const string &key, const string &field)
{
    CResult result;
    hexists( key, field, result );

    ReplyType type = result.getType();

    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_INTEGERER != type )
    {
        throw ProtocolErr( "HEXISTS: data recv is not intgerer" );
    }
    return result.getInt();
}

void CRedisClient::hgetall(const string &key, CResult &result )
{
    _socket.clearBuffer();
    Command cmd( "HGETALL" );
    cmd << key;
    _sendCommand( cmd );
    _getReply( result );
}

uint64_t CRedisClient::hgetall(const string &key, CRedisClient::MapString &values)
{
    CResult result;
    hgetall( key, result );
    ReplyType type = result.getType();

    if ( REDIS_REPLY_ERROR == type )
    {
        throw ReplyErr( result.getErrorString() );
    }else if ( REDIS_REPLY_ARRAY != type )
    {
        throw ProtocolErr( "HGETALL: data recv is not arry" );
    }

    CResult::ListCResult::const_iterator it = result.getArry().begin();
    CResult::ListCResult::const_iterator it2 = it;
    CResult::ListCResult::const_iterator end = result.getArry().end();

    for ( ; it != end; ++it )
    {
        it2 = it++;		// the next element is value.
        values.insert( MapString::value_type( *it2, *it ) );
    }
    return values.size();
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

   _getValueFromArry( result.getArry(), values );
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
        throw ProtocolErr( "HLEN: data recved is not interger" );
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
        throw ProtocolErr( "HSETNX: data recved is not interger" );
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

    _getValueFromArry( it->getArry(), values );

    return ( lastCur == 0 ? false : true );
}








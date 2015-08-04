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
    for ( ; it !=fields.end(); it++ )
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

uint64_t CRedisClient::hgetall(const string &key, CRedisClient::MapString &value)
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

    for ( ; it != end; it++ )
    {
        it2 = it++;		// the next element is value.
        value.insert( MapString::value_type( *it2, *it ) );
    }
    return value.size();
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

uint64_t CRedisClient::hkeys(const string &key, CRedisClient::VecString &value)
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

   CResult::ListCResult::const_iterator it = result.getArry().begin();
   CResult::ListCResult::const_iterator end = result.getArry().end();
   for ( ; it != end; it++ )
   {
        value.push_back( static_cast<string>(*it) );
   }
   return value.size();
}






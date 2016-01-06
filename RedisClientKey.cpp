/**
 *
 * @file	RedisClientKey.cpp
 * @brief CRedisClient key 操作的代码实现。
 *
 *
 * @author: 		Stanley Huang
 * @date: 		2015年8月14日
 *
 */

#include "Command.h"
#include "CRedisClient.h"

int64_t CRedisClient::keys( const std::string &pattern , VecString &values )
{
	Command cmd("KEYS");
	cmd << pattern;

    uint64_t num;
    _getArry(cmd, values,num );
    return num;
}

int64_t CRedisClient::del( CRedisClient::VecString &keys )
{

	Command cmd("DEL");

	VecString::const_iterator it = keys.begin();
	VecString::const_iterator end = keys.end();
	for ( ; it != end ; ++it )
	{
		cmd << *it;
	}

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::exists( const string& key )
{
	Command cmd("EXISTS");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;

}

bool CRedisClient::expireAt( const string& key , const uint64_t& timestamp )
{
	Command cmd("EXPIREAT");
	cmd << key << timestamp;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::pExpireAt( const string& key , const uint64_t& timestamp )
{
	Command cmd("PEXPIREAT");
	cmd << key << timestamp;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::expire( const string& key , const uint64_t& seconds )
{
	Command cmd("EXPIRE");
	cmd << key << seconds;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::pExpire( const string& key , const uint64_t& msec )
{
	Command cmd("PEXPIRE");
	cmd << key << msec;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

int64_t CRedisClient::ttl( const string& key )
{
	Command cmd("TTL");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

int64_t CRedisClient::pttl( const string& key )
{
	Command cmd("PTTL");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::persist( const string& key )
{
	Command cmd("PERSIST");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::move( const string& key ,int dstDb )
{
	Command cmd("MOVE");
    cmd << key << dstDb;
	int64_t num = 0;
	_getInt(cmd, num);
    return num;
}

void CRedisClient::object(const EobjSubCommand& subcommand , const string& key , CResult &result)
{
    Command cmd("OBJECT");

    switch ( subcommand )
	{
	case REFCOUNT :
        cmd << "REFCOUNT" << key; break;
        case IDLETIME :
        cmd << "IDLETIME" << key;	break;
        case ENCODING :
        cmd << "ENCODING" << key;	break;
    }
    _getResult( cmd, result );
}

bool CRedisClient::randomKey( string& key )
{
	Command cmd("RANDOMKEY");

    if ( _getString(cmd, key) )
        return true;
    else
        return false;
}

void CRedisClient::rename( const string& key , const string& newKey )
{
	Command cmd("RENAME");
	string status;
	cmd << key << newKey;

    _getStatus(cmd, status);
}

bool CRedisClient::renameNx( const string& key , const string& newKey )
{
	Command cmd("RENAMENX");
	int64_t num = 0;
	cmd << key << newKey;

	if ( _getInt(cmd, num) )
	{
		return num;
	}
	return false;
}

void CRedisClient::sort(const string& key ,CResult& result ,int64_t offset, int64_t count,
                        const string& by,const VecString&get ,bool desc,bool alpha )
{
	Command cmd("SORT");
	cmd << key;

    if ( by != "" )
    {
        cmd << "BY" << by;
    }
    if (  offset >=0 )
    {
        cmd << "LIMIT" << offset << count;
    }
    if ( !get.empty() )
    {
        VecString::const_iterator it = get.begin();
        VecString::const_iterator end = get.end();
        for( ; it != end; ++it )
        {
            cmd << "GET" << *it;
        }
    }
	if ( desc )
		cmd << "DESC";
    if ( alpha )
        cmd << "ALPHA";

    _getResult( cmd, result );
    return ;
}

uint64_t CRedisClient::sort(const string& key , const string& storeKey, int64_t offset, int64_t count,
                        const string& by, const VecString&get , bool desc, bool alpha )
{
    Command cmd("SORT");
    cmd << key;

    if ( by != "" )
    {
        cmd << "BY" << by;
    }
    if (  offset >=0 && count >=0 )
    {
        cmd << "LIMIT" << offset << count;
    }
    if ( !get.empty() )
    {
        VecString::const_iterator it = get.begin();
        VecString::const_iterator end = get.end();
        for( ; it != end; ++it )
        {
            cmd << "GET" << *it;
        }
    }
    if ( desc )
        cmd << "DESC";
    if ( alpha )
        cmd << "ALPHA";
    if ( !storeKey.empty() )
    {
        cmd << "STORE" << storeKey;
    }

    int64_t num = 0;
    _getInt( cmd, num );
    return num;
}

REDIS_DATA_TYPE CRedisClient::type( const string& key )
{
	Command cmd("TYPE");
	cmd << key;

    string type;
     _getStatus(cmd, type);

     if( "none" == type )
     {
         return NONE;
     }else if ( "string" == type )
     {
         return STRING;
     }else if ( "list" == type )
     {
         return LIST;
     }else if ( "set" == type )
     {
         return SET;
     }else if ( "zset" == type )
     {
         return ZSET;
     }else if ( "hash" == type )
     {
         return HASH;
     }else
     {
         throw ProtocolErr("TYPE:" "type string is unexpection!");
     }
}

bool CRedisClient::scan( int64_t& cursor,VecString &values, const string &match, uint64_t count )
{
   CResult result;
   Command cmd( "SCAN" );
   cmd <<cursor;

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
  _getStringVecFromArry( it->getArry(), values );
  return ( cursor == 0 ? false : true );
}

bool CRedisClient::dump( const string& key , string& retStr )
{
	Command cmd("DUMP");
	cmd << key;
	return _getString(cmd, retStr);
}

bool CRedisClient::restore(const string& key , const string& buf , const int ttl )
{
	Command cmd("RESTORE");
	string status;

    cmd << key << ttl << buf;

    return _getStatus(cmd, status);
}

void CRedisClient::migrate(  const string& key ,const string& host , uint16_t port ,
        uint16_t db ,uint16_t timeout,const string& mode )
{
	Command cmd("MIGRATE");
    cmd << host << port << key << db << timeout << mode;

    string status;
    _getStatus( cmd, status );

    if ( status != "OK" )
    {
        throw ReplyErr( status );
    }
}


#include "Command.h"
#include "CRedisClient.h"



uint64_t CRedisClient::sadd(const string &key, const CRedisClient::VecString &members)
{
    Command cmd( "SADD" );
    cmd << key;
    VecString::const_iterator it = members.begin();
    VecString::const_iterator end = members.end();

    for ( ; it != end ; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt( cmd , num );
    return num;
}



uint64_t CRedisClient::scard(const string &key)
{
    Command cmd( "SCARD" );
    cmd << key;
    int64_t num;
    _getInt( cmd , num );
    return num;
}




uint64_t CRedisClient::sdiff(const CRedisClient::VecString &keys, CRedisClient::VecString &values)
{
    Command cmd( "SDIFF" );
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd, values );
    return values.size();
}

uint64_t CRedisClient::sdiffstore(const string &destKey, const CRedisClient::VecString &keys )
{
    Command cmd( "SDIFFSTORE" );
     cmd << destKey;
     VecString::const_iterator it = keys.begin();
     VecString::const_iterator end = keys.end();
     for ( ; it != end; ++it )
     {
         cmd << *it;
     }

     int64_t num = 0;
     _getInt( cmd, num );
     return num;
}

uint64_t CRedisClient::sinter(const CRedisClient::VecString &keys, VecString &values)
{
    Command cmd( "SINTER" );
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd, values );
    return values.size();
}

uint64_t CRedisClient::sinterstore( const string& destKey ,const CRedisClient::VecString &keys )
{
    Command cmd( "SINTERSTORE" );
    cmd << destKey;
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    int64_t num = 0;
    _getInt( cmd, num );
    return num;
}

bool CRedisClient::sismember(const string &key, const string &member)
{
    Command cmd( "SISMEMBER" );
    cmd << key << member;

    int64_t num = 0;
    _getInt( cmd, num );
    return ( num == 1 ? true: false );
}

uint64_t CRedisClient::smembers( const string &key, CRedisClient::VecString &members )
{
    Command cmd( "SMEMBERS" );
    cmd << key;
    _getArry( cmd, members );
    return members.size();
}

bool CRedisClient::smove(const string &source, const string &dest, const string &member)
{
    Command cmd( "SMOVE" );
    cmd << source << dest << member;
    int64_t num;
    _getInt( cmd, num );
    return	( num==0 ? false : true );
}



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

    uint64_t num = 0;
    _getArry( cmd, values , num);
    return num;
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

    uint64_t num = 0;
    _getArry( cmd, values, num );
    return num;
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

    uint64_t num = 0;
    _getArry( cmd, members,num );
    return num;
}

bool CRedisClient::smove(const string &source, const string &dest, const string &member)
{
    Command cmd( "SMOVE" );
    cmd << source << dest << member;
    int64_t num;
    _getInt( cmd, num );
    return	( num==0 ? false : true );
}

bool CRedisClient::spop(const string &key, string &member)
{
    member.clear();
    Command cmd( "SPOP" );
    cmd << key;

    return ( _getString( cmd, member ) );
}

bool CRedisClient::srandmember(const string &key, string &member)
{
    Command cmd ( "SRANDMEMBER" );
    cmd << key;
    return ( _getString( cmd, member) );
}

uint64_t CRedisClient::srandmember(const string &key, int count, CRedisClient::VecString &members)
{
    Command cmd( "SRANDMEMBER" );
    cmd << key << count ;

    uint64_t num = 0;
    _getArry( cmd, members, num );
    return num;
}

uint64_t CRedisClient::srem(const string &key,const CRedisClient::VecString &members)
{
    Command cmd ( "SREM" );
    cmd << key;

    VecString::const_iterator it = members.begin();
    VecString::const_iterator end = members.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt( cmd, num );
    return num;
}

uint64_t CRedisClient::sunion(const CRedisClient::VecString &keys , VecString &members)
{
    Command cmd( "SUNION" );

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    uint64_t num = 0;
    _getArry( cmd, members, num );
    return num;
}

uint64_t CRedisClient::sunionstroe(const string &dest, const CRedisClient::VecString &keys)
{
    Command cmd( "SUNIONSTORE" );
    cmd << dest;
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt( cmd, num );
    return num;
}

bool CRedisClient::sscan(const string &key, int64_t cursor, VecString &values, const string &match, uint64_t count)
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

     Command cmd( "SSCAN" );
     cmd << key << realCur;

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
     lastCur = _valueFromString<uint64_t>( it->getString() );
    ++it;

    _getStringVecFromArry( it->getArry(), values );
    return ( lastCur == 0 ? false : true );
}

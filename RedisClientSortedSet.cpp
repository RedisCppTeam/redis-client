
#include "Command.h"
#include "CRedisClient.h"


uint64_t CRedisClient::zadd(const string &key, const CRedisClient::MapString &pairs)
{
    Command cmd( "ZADD" );
    cmd << key;
    MapString::const_iterator it = pairs.begin();
    MapString::const_iterator end = pairs.end();

    for ( ; it !=end ; ++it )
    {
        cmd << it->first;
        cmd << it->second;
    }
    int64_t num;
    _getInt(cmd,num);
    return num;
}

uint64_t CRedisClient::zcard(const string& key)
{
    Command cmd( "ZCARD" );
    cmd << key;
    int64_t num;
    _getInt(cmd,num);
    return num;

}



uint64_t CRedisClient::zcount(const string& key,const string& min,const string& max)
{
    Command cmd( "ZCOUNT" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;
}



string CRedisClient::zincrby(const string& key,float increment,const string& member)
{
    Command cmd( "ZINCRBY" );
    cmd << key << increment<< member;
    string str;
    _getString(cmd,str);
    return str;
}





uint64_t CRedisClient::zrange(const string &key, const int64_t start, const int64_t stop, CRedisClient::VecString& reply)
{
    Command cmd( "ZRANGE" );
    cmd << key << start<< stop;
    return   _getArry(cmd,reply);
}
uint64_t CRedisClient::zrange(const string &key, const int64_t start, const int64_t stop, CRedisClient::MapString &reply)
{
    Command cmd( "ZRANGE" );
    cmd << key << start<< stop;
    cmd<<"WITHSCORES";
    return   _getArry(cmd,reply);
}


uint64_t CRedisClient::zrangebyscore(const string &key, const string &min, const string &max, CRedisClient::VecString &reply,int64_t offset,int64_t count)
{
    Command cmd( "ZRANGEBYSCORE" );
    cmd << key << min<< max;

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}

uint64_t CRedisClient::zrangebyscore(const string &key, const string &min, const string &max, CRedisClient::MapString &reply,int64_t offset,int64_t count)
{
    _socket.clearBuffer();
    Command cmd( "ZRANGEBYSCORE" );
    cmd << key << min<< max;
    cmd<<"WITHSCORES";

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;
    return   _getArry(cmd,reply);
}

bool CRedisClient::zrank(const string& key,const string& member,int64_t& reply)
{
    Command cmd( "ZRANK" );
    cmd << key << member;

    return _getInt(cmd,reply);
}


uint64_t CRedisClient::zrem(const string& key,VecString& members)

{
    Command cmd( "ZREM" );
    cmd << key;

    VecString::const_iterator it = members.begin();
    VecString::const_iterator end=members.end();
    for ( ; it !=end; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt(cmd,num);
    return num;
}


uint64_t CRedisClient::zremrangebyrank(const string& key,const int64_t start,const int64_t stop)
{
    Command cmd( "ZREMRANGEBYRANK" );
    cmd << key << start << stop;
    int64_t num;
    _getInt(cmd,num);
    return num;

}



uint64_t CRedisClient::zremrangebyscore(const string& key,const string& min,const string& max)
{
    Command cmd( "ZREMRANGEBYSCORE" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;
}

uint64_t CRedisClient::zrevrange(const string &key, const int64_t start, const int64_t stop, CRedisClient::VecString &reply)
{
    Command cmd( "ZREVRANGE" );
    cmd << key << start<< stop;
    return   _getArry(cmd,reply);
}

uint64_t CRedisClient::zrevrange(const string &key, const int64_t start, const int64_t stop, CRedisClient::MapString &reply)
{
    Command cmd( "ZREVRANGE" );
    cmd << key << start<< stop;
    cmd<<"WITHSCORES";
    return   _getArry(cmd,reply);
}





uint64_t CRedisClient::zrevrangebyscore(const string &key, const string &max, const string &min, CRedisClient::VecString &reply, int64_t offset,int64_t count)
{
    Command cmd( "ZREVRANGEBYSCORE");
    cmd << key << max<< min;

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}

uint64_t CRedisClient::zrevrangebyscore(const string &key, const string &max, const string &min, CRedisClient::MapString &reply, int64_t offset,int64_t count)
{
    Command cmd( "ZREVRANGEBYSCORE");
    cmd << key << max<< min;
    cmd<<"WITHSCORES";

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}



uint64_t CRedisClient::zrevrank(const string& key,const string& member)
{
    Command cmd( "ZREVRANK" );
    cmd << key << member;
    int64_t num;
    _getInt(cmd,num);
    return num;
}





string CRedisClient::zscore(const string& key,const string& member)
{
    Command cmd( "ZSCORE" );
    cmd << key <<  member;
    string str;
    _getString(cmd,str);
    return str;
}

void CRedisClient::addAggregate(Command& cmd,int aggregate)
{
    static const char * aggre[]={"SUM","MIN","MAX"};
    cmd <<"AGGREGATE";
    cmd<<aggre[aggregate];
}



uint64_t CRedisClient::zunionstore (const string& destination,const uint64_t numkeys,const VecString& keys,const VecString& weigets,int aggregate)
{

    Command cmd( "ZUNIONSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }

    cmd <<"WEIGHTS";
    VecString::const_iterator it2 = weigets.begin();
    VecString::const_iterator end2 = weigets.end();
    for ( ; it2 !=end2; it2++ )
    {
        cmd << *it2;
    }
    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;

}


uint64_t CRedisClient::zunionstore (const string& destination,const uint64_t numkeys,const VecString& keys,int aggregate)
{
    Command cmd( "ZUNIONSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }
    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;

}

//.................................................................................................................

uint64_t CRedisClient::zinterstore (const string& destination,const uint64_t numkeys,const VecString& keys,const VecString& weigets,int aggregate)
{
    Command cmd( "ZINTERSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }

    cmd <<"WEIGHTS";
    VecString::const_iterator it2 = weigets.begin();
    VecString::const_iterator end2 = weigets.end();
    for ( ; it2 !=end2; it2++ )
    {
        cmd << *it2;
    }

    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;

}


uint64_t CRedisClient::zinterstore (const string& destination,const uint64_t numkeys,const VecString& keys,int aggregate)
{

    Command cmd( "ZINTERSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }
    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;


}
bool CRedisClient::zscan(const string &key, int64_t cursor, MapString &reply, const string &match, uint64_t count )
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

    Command cmd( "ZSCAN" );
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
    _getStringMapFromArry( it->getArry(),reply );
    return ( lastCur == 0 ? false : true );
}

uint64_t CRedisClient::zrangebylex(const string &key, const string &min, const string &max, CRedisClient::VecString &reply, int64_t offset,int64_t count)
{
    Command cmd( "ZRANGEBYLEX" );
    cmd << key << min<< max;
    if (count>0)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}

uint64_t CRedisClient::zlexcount(const string &key, const string &min, const string &max)
{
    Command cmd( "ZLEXCOUNT" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;

}

uint64_t CRedisClient::zremrangebylex(const string &key, const string &min, const string &max)
{
    Command cmd( "ZREMRANGEBYLEX" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;
}

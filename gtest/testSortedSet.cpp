#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "ExceptRedis.h"
#include "CResult.h"

using namespace std;
using namespace Taiji::Redis;
typedef CRedisClient::VecString VEC;
typedef CRedisClient::TupleString MAP;
void TSortesSetPrint(const string &cmd,uint64_t num)
{
    cout<<cmd<<":"<<num<<endl;
    cout<<endl;
}
void TSortesSetPrint(const string &cmd,string str)
{
    cout<<cmd<<":"<<str<<endl;
    cout<<endl;
}
void TSortesSetPrint(const string& cmd,VEC& vec)
{
    cout<<cmd<<":"<<endl;
    VEC::const_iterator it = vec.begin();
    VEC::const_iterator end = vec.end();
    for ( ; it != end; ++it )
    {
        cout<<*it<<endl;
    }
    cout<<endl;
}
void TSortesSetPrint(const string &cmd,MAP& map)
{
    cout<<cmd<<":"<<endl;
    MAP::const_iterator it = map.begin();
    MAP::const_iterator end = map.end();
    for ( ; it != end; ++it )
    {
         std::cout << std::get<0>(*it) << ":" <<std::get<1>(*it) << std::endl;
    }
    cout<<endl;
}

void SetVec(VEC& vec,const string& vecName,uint64_t start,uint64_t end)
{
    vec.clear();
    string member;
    std::stringstream ss;
    uint64_t i;
    for ( i = start; i <= end; i++ )
    {
        member=vecName;
        ss.str("");
        ss << i;
        member += ss.str();
        vec.push_back(member);
    }
}

void SetMap(MAP& map,const string& mapName,uint64_t start,uint64_t end)
{
    map.clear();
    string member,score;
    std::stringstream ss;
    uint64_t i;
    for ( i = start; i <= end; i++ )
    {
        member=mapName;
        ss.str("");
        ss << i;
        score= ss.str();
        member += ss.str();
        map.push_back( CRedisClient::TupleString::value_type(score,member) );

    }
}



void TestZadd(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zadd----------"<<endl;
    MAP map;
    SetMap(map,"member_",1,1000);
    TSortesSetPrint("redis.zadd(SortedSet,map)",redis.zadd("SortedSet",map));
}
void TestZcard(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zcard----------"<<endl;
    TSortesSetPrint("redis.zcard(SortedSet)",redis.zcard("SortedSet"));
}
void TestZcount(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zcount----------"<<endl;
    TSortesSetPrint( "zcount",redis.zcount("SortedSet","35","98") );
}
void TestZincrby(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zincrby----------"<<endl;
    double dbl;
    dbl=redis.zincrby("SortedSet",1.1,"member_45");
    cout<<"redis.zincrby(SortedSet,1,member_45)="<<dbl<<endl;
}
void TestZrange(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrange----------"<<endl;
    VEC vec;
    redis.zrange("SortedSet",1,10,vec);
    TSortesSetPrint("redis.zrange(SortedSet,1,10,vec)",vec);
}
void TestZrangeWithscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrangeWithscore----------"<<endl;
    MAP map;
    redis.zrangeWithscore("SortedSet",1,10,map);
    TSortesSetPrint("redis.zrange(SortedSet,1,10,map)",map);

}
void TestZrangebyscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrangebyscore----------"<<endl;
    VEC vec;
    redis.zrangebyscore("SortedSet","20","50",vec,0,10);
    TSortesSetPrint("redis.zrangebyscore(SortedSet,20,50,vec,0,10)",vec);
}
void TestZrangebyscoreWithscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrangebyscoreWithscore----------"<<endl;
    MAP map;
    redis.zrangebyscoreWithscore("SortedSet","20","50",map,0,10);
    TSortesSetPrint("redis.zrangebyscore(SortedSet,20,50,map,0,10)",map);
}
void TestZrank(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrank----------"<<endl;
    int64_t num;
    TSortesSetPrint("redis.zrank(SortedSet,member_60,num)",redis.zrank("SortedSet","member_60",num));
}
void TestZrem(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrem----------"<<endl;
    VEC vec;
    SetVec(vec,"member_",10,60);
    TSortesSetPrint("redis.zrem(SortedSet,vec)",redis.zrem("SortedSet",vec));
}
void TestZremrangebyrank(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zremrangebyrank----------"<<endl;
    TSortesSetPrint("redis.zremrangebyrank(SortedSet,1,5)",redis.zremrangebyrank("SortedSet",1,5));
}
void TestZremrangebyscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zremrangebyscore----------"<<endl;
    TSortesSetPrint("redis.zremrangebyscore(SortedSet,(40,90)",redis.zremrangebyscore("SortedSet","(40","90"));
}

void TestZrevrange(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrevrange----------"<<endl;
    MAP map;
    VEC vec;
    redis.flushall();
    SetMap(map,"member_",1,1000);
    redis.zadd("SortedSet",map);
    vec.clear();
    redis.zrevrange("SortedSet",40,-1,vec);
    TSortesSetPrint("redis.zrevrange(SortedSet,40,-1,vec)",vec);
}

void TestZrevrangeWithscore(){

    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrevrangeWithscore----------"<<endl;
    redis.flushall();
    MAP map;
    SetMap(map,"member_",1,1000);
    redis.zadd("SortedSet",map);
    map.clear();
    redis.zrevrangeWithscore("SortedSet",40,-1,map);
    TSortesSetPrint("redis.zrevrange(SortedSet,40,-1,map)",map);

}
void TestZrevrangebyscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrevrangebyscore----------"<<endl;
    VEC vec;
    redis.zrevrange("SortedSet",40,-1,vec);
    TSortesSetPrint("redis.zrevrange(SortedSet,40,-1,vec)",vec);
}
void TestZrevrangebyscoreWithscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrevrangebyscoreWithscore----------"<<endl;
    MAP map;
    redis.zrevrangeWithscore("SortedSet",40,-1,map);
    TSortesSetPrint("redis.zrevrange(SortedSet,40,-1,map)",map);
}
void TestZrevrank(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zrevrank----------"<<endl;
    int64_t num;
    TSortesSetPrint("redis.zrevrank(SortedSet,member_87)",redis.zrevrank("SortedSet","member_87",num));
}
void TestZscore(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zscore----------"<<endl;
    string strZscore;
    TSortesSetPrint("zscore(SortedSet,member_87)",redis.zscore("SortedSet","member_87",strZscore));
}
void TestZunionstore (){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zunionstore----------"<<endl;
    MAP map;
    VEC vec;
    string unionKey,unionKey1,unionKey2;
    unionKey="sortedSetUnion";
    unionKey1="sortedSetUnion1";
    unionKey2="sortedSetUnion2";
    SetMap(map,"member_",50,500);
    redis.zadd(unionKey1,map);
    SetMap(map,"member_",99,700);
    redis.zadd(unionKey2,map);
    vec.push_back(unionKey1);
    vec.push_back(unionKey2);
    TSortesSetPrint("redis.zinterstore(unionKey,2,vec,2)",redis.zunionstore(unionKey,vec,CRedisClient::MAX));
}

void TestZinterstore (){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zinterstore----------"<<endl;
    MAP map;
    VEC vec;
    string unionKey,unionKey1,unionKey2;
    unionKey="sortedSetUnion";
    unionKey1="sortedSetUnion1";
    unionKey2="sortedSetUnion2";
    SetMap(map,"member_",50,500);
    redis.zadd(unionKey1,map);
    SetMap(map,"member_",99,700);
    redis.zadd(unionKey2,map);
    vec.push_back(unionKey1);
    vec.push_back(unionKey2);
    TSortesSetPrint("redis.zinterstore(unionKey,2,vec,2)",redis.zinterstore(unionKey,vec,CRedisClient::MAX));
}
void TestZscan(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zscan----------"<<endl;
    MAP map;
    int64_t cur = 0;
    while ( redis.zscan("SortedSet", cur, map ) );
    TSortesSetPrint("redis.zscan( SortedSet, -1, map",map);
}
void TestZrangebylex (){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    VEC vec;
    redis.zrangebylex("SortedSet","[10","(60",vec);
    TSortesSetPrint("redis.zrangebylex(SortedSet,[10,(60,vec)",vec);
}
void TestZlexcount (){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    TSortesSetPrint("redis.zlexcount(SortedSet,[10,(60)",redis.zlexcount("SortedSet","[10","(60"));
}
void TestZremrangebylex (){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    TSortesSetPrint("redis.zremrangebylex(SortedSet,[10,(60)",redis.zremrangebylex("SortedSet","[10","(60"));
}
void TestSortedSetMain(){
    TestZadd();
    TestZcard();
    TestZcount();
    TestZincrby();
    TestZrange();
    TestZrangeWithscore();
    TestZrangebyscore();
    TestZrangebyscoreWithscore();
    TestZrank();
    TestZrem();
    TestZremrangebyrank();
    TestZremrangebyscore();
    TestZrevrangeWithscore();
    TestZrevrangebyscore();
    TestZrevrangebyscoreWithscore();
    TestZrevrank();
    TestZscore();
    TestZunionstore ();
    TestZinterstore ();
    TestZscan();
    TestZrangebylex ();
    TestZlexcount ();
    TestZremrangebylex ();
}

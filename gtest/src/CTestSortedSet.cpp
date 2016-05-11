#include "CTestSortedSet.h"

CRedisClient  CTestSortedSet::redis;

void CTestSortedSet::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestSortedSet::TearDownTestCase()
{

}

void CTestSortedSet::GetVecTuple(CRedisClient::VecTuple &vecTup, const std::string &keyPre, uint64_t start, uint64_t end)
{
    vecTup.clear();
    string member,score;
    std::stringstream ss;
    uint64_t i;
    for ( i = start; i <= end; i++ )
    {
        member=keyPre;
        ss.str("");
        ss << i;
        score= ss.str();
        member += ss.str();
        vecTup.push_back( CRedisClient::VecTuple::value_type(score,member) );
    }
}


TEST_F(CTestSortedSet, zadd)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    EXPECT_EQ(10, redis.zadd("sortedSet", vecTup));
}

TEST_F(CTestSortedSet, zcard)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    EXPECT_EQ(10, redis.zcard("sortedSet"));
}

TEST_F(CTestSortedSet, zcount)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    EXPECT_EQ(6 - 3 + 1, redis.zcount("sortedSet","3","6"));
}

TEST_F(CTestSortedSet, zincrby)
{
    redis.flushall();
    redis.zadd("sortedSet", CRedisClient::VecTuple{CRedisClient::TupleString("10","a")});
    ASSERT_DOUBLE_EQ(11.1, redis.zincrby("sortedSet", 1.1, "a"));
}

TEST_F(CTestSortedSet, zrange)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    CRedisClient::VecString vec;
    redis.zrange("sortedSet", 2, 5, vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrangeWithscore)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    CRedisClient::VecTuple vec;
    redis.zrangeWithscore("sortedSet", 2, 5, vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrangebyscore)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    CRedisClient::VecString vec;
    redis.zrangebyscore("sortedSet", "2", "5", vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrangebyscoreWithscore)
{
    redis.flushall();
    CRedisClient::VecTuple vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    CRedisClient::VecTuple vec;
    redis.zrangebyscoreWithscore("sortedSet", "2", "5", vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

/*

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
*/

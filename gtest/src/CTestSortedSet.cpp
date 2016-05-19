#include "CTestSortedSet.h"

CRedisClient  CTestSortedSet::redis;
using namespace std;

void CTestSortedSet::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestSortedSet::TearDownTestCase()
{

}


void CTestSortedSet::GetVecTuple(VecTupleString &vecTup, const std::string &keyPre, uint64_t start, uint64_t end)
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
        vecTup.push_back(VecTupleString::value_type(score, member));
    }
}


TEST_F(CTestSortedSet, zadd)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    EXPECT_EQ(10, redis.zadd("sortedSet", vecTup));
}

TEST_F(CTestSortedSet, zcard)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    EXPECT_EQ(10, redis.zcard("sortedSet"));
}

TEST_F(CTestSortedSet, zcount)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    EXPECT_EQ(6 - 3 + 1, redis.zcount("sortedSet", "3", "6"));
}

TEST_F(CTestSortedSet, zincrby)
{
    redis.flushall();
    redis.zadd("sortedSet", VecTupleString{TupleString("10", "a")});
    ASSERT_DOUBLE_EQ(11.1, redis.zincrby("sortedSet", 1.1, "a"));
}

TEST_F(CTestSortedSet, zrange)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    VecString vec;
    redis.zrange("sortedSet", 2, 5, vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrangeWithscore)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    VecTupleString vec;
    redis.zrangeWithscore("sortedSet", 2, 5, vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrangebyscore)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    VecString vec;
    redis.zrangebyscore("sortedSet", "2", "5", vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrangebyscoreWithscore)
{
    redis.flushall();
    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd("sortedSet", vecTup);
    VecTupleString vec;
    redis.zrangebyscoreWithscore("sortedSet", "2", "5", vec);
    EXPECT_EQ(5 - 2 + 1, vec.size());
}

TEST_F(CTestSortedSet, zrank)
{
    redis.flushall();
    string key("sortedSet");
    string member("member_2");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 3);
    redis.zadd(key, vecTup);

    int64_t reply;
    redis.zrank(key,member,reply);
    EXPECT_EQ(1, reply);
}

TEST_F(CTestSortedSet, zrem)
{
    redis.flushall();
    string key("sortedSet");
    string member("member_5");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 10);
    redis.zadd(key, vecTup);

    uint64_t ret =redis.zrem(key,VecString{member});
    EXPECT_EQ(1, ret);
}

TEST_F(CTestSortedSet, zremrangebyrank)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key, vecTup);

    uint64_t ret =redis.zremrangebyrank(key,2,3);
    uint64_t cnt =redis.zcount(key,"0","100");
    EXPECT_EQ(2, ret);
    EXPECT_EQ(3, cnt);
}

TEST_F(CTestSortedSet, zremrangebyscore)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key, vecTup);

    uint64_t ret =redis.zremrangebyscore(key,"2","3");
    uint64_t cnt =redis.zcount(key,"0","100");
    EXPECT_EQ(2, ret);
    EXPECT_EQ(3, cnt);
}

TEST_F(CTestSortedSet, zrevrange)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    string profix("member_");
    GetVecTuple(vecTup, profix, 1, 5);
    redis.zadd(key, vecTup);

    VecString reply;
    uint64_t ret =redis.zrevrange(key,0,100,reply);
    for(uint i=0;i<reply.size();i++)
    {
        stringstream ss;
        ss<<profix<<ret--;
        EXPECT_EQ(ss.str(), reply[i]);
    }
}

TEST_F(CTestSortedSet, zrevrangeWithscore)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    string profix("member_");
    GetVecTuple(vecTup, profix, 1, 5);
    redis.zadd(key, vecTup);

    VecTupleString reply;
    uint64_t ret =redis.zrevrangeWithscore(key,0,100,reply);
    EXPECT_EQ(5*2, ret);
}

TEST_F(CTestSortedSet, zrevrangebyscore)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key, vecTup);

    VecString reply;
    uint64_t ret =redis.zrevrangebyscore(key,"3","0",reply);
    EXPECT_EQ(3, ret);
}

TEST_F(CTestSortedSet, zrevrangebyscoreWithscore)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key, vecTup);

    VecTupleString reply;
    uint64_t ret =redis.zrevrangebyscoreWithscore(key,"3","0",reply);
    EXPECT_EQ(3*2, ret);
}

TEST_F(CTestSortedSet, zrevrank)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key, vecTup);

    int64_t reply;
    redis.zrevrank(key,"member_2",reply);
    EXPECT_EQ(3, reply);
}

TEST_F(CTestSortedSet, zscore)
{
    redis.flushall();
    string key("sortedSet");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key, vecTup);

    string reply;
    redis.zscore(key,"member_2",reply);
    EXPECT_EQ("2", reply);
}

TEST_F(CTestSortedSet, zunionstore)
{
    redis.flushall();
    string key1("manager");
    string key2("programa");
    string keystore("salary");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key1, vecTup);
    GetVecTuple(vecTup, "program_", 1, 5);
    redis.zadd(key2, vecTup);

    VecString keys{key1,key2};
    uint64_t ret = redis.zunionstore(keystore,keys);
    EXPECT_EQ(10, ret);
}

TEST_F(CTestSortedSet, zinterstore)
{
    redis.flushall();
    string key1("manager");
    string key2("programa");
    string keystore("salary");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "member_", 1, 5);
    redis.zadd(key1, vecTup);
    GetVecTuple(vecTup, "member_", 3, 8);
    redis.zadd(key2, vecTup);

    VecString keys{key1,key2};
    uint64_t ret = redis.zinterstore(keystore,keys);
    EXPECT_EQ(3, ret);
}

//* eg: get all key between pair_100 and pair_199


TEST_F(CTestSortedSet, zscan)
{
    redis.flushall();
    string key("manager");

    VecTupleString vecTup;
    GetVecTuple(vecTup, "pair_", 1, 15);
    redis.zadd(key, vecTup);

    vecTup.clear();
    int64_t cursor;
    bool retb=redis.zscan( key, cursor, vecTup,"pair_??" );
    while (retb && redis.zscan( key, cursor, vecTup ,"pair_??") );

    uint64_t ret = vecTup.size();
    EXPECT_EQ(6, ret);
}

TEST_F(CTestSortedSet, zrangebylex)
{
    redis.flushall();
    string key("key");

    std::tuple<string,string> t1{"2","a"};
    std::tuple<string,string> t2{"2","b"};
    std::tuple<string,string> t3{"2","c"};
    std::tuple<string,string> t4{"2","d"};
    std::tuple<string,string> t5{"2","e"};
    VecTupleString vals{t1,t2,t3,t4,t5};
    redis.zadd(key,vals);

    VecString reply;
    uint64_t ret =redis.zrangebylex(key,"-","+", reply);
    EXPECT_EQ(5, ret);

    ret =redis.zrangebylex(key,"[b","(e", reply);
    EXPECT_EQ(3, ret);
}
TEST_F(CTestSortedSet, zlexcount)
{
    redis.flushall();
    string key("key");

    std::tuple<string,string> t1{"2","a"};
    std::tuple<string,string> t2{"2","b"};
    std::tuple<string,string> t3{"2","c"};
    std::tuple<string,string> t4{"2","d"};
    std::tuple<string,string> t5{"2","e"};
    VecTupleString vals{t1,t2,t3,t4,t5};
    redis.zadd(key,vals);

    VecString reply;
    uint64_t ret =redis.zlexcount(key,"[b","(e");
    EXPECT_EQ(3, ret);


}

TEST_F(CTestSortedSet, zremrangebylex)
{
    redis.flushall();
    string key("key");

    std::tuple<string,string> t1{"2","a"};
    std::tuple<string,string> t2{"2","b"};
    std::tuple<string,string> t3{"2","c"};
    std::tuple<string,string> t4{"2","d"};
    std::tuple<string,string> t5{"2","e"};
    VecTupleString vals{t1,t2,t3,t4,t5};
    redis.zadd(key,vals);

    VecString reply;
    uint64_t ret =redis.zremrangebylex(key,"[b","(e");
    EXPECT_EQ(3, ret);

    ret =redis.zrangebylex(key,"-","+", reply);
    EXPECT_EQ(2, ret);
}

/*


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

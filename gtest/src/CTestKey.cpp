#include "CTestKey.h"
#include <Poco/Timestamp.h>
using Poco::Timestamp;

using namespace std;

CRedisClient CTestKey::redis;


void printBin(const void* p,int len)
{
    unsigned char* c=(unsigned char*)p;
    for(int i=0;i<len;i++)
    {
        printf("%x(%c) ",c[i],c[i]);
    }
    putchar('\n');
}

void CTestKey::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestKey::TearDownTestCase()
{

}

TEST_F(CTestKey, del_keys)
{
    redis.flushall();
    redis.set("key00", "value");
    redis.set("key1", "value");
    redis.set("key2", "value");
    redis.set("key3", "value");
    redis.set("key4", "value");
    VecString keyRet;
    VecString keyV={"key1","key2","key3","key4"};
    redis.keys("key?",keyRet);
    EXPECT_EQ(keyRet.size(), keyV.size());
    //EXPECT_EQ(keyRet[1], keyV[1]);

    //
    EXPECT_EQ(1, redis.del("key00"));
    EXPECT_EQ(0, redis.del("key00"));
    EXPECT_EQ(4, redis.del(keyRet));
}

TEST_F(CTestKey, scan)
{
    redis.flushall();
    redis.set("key00", "value");
    redis.set("key1", "value");
    redis.set("key2", "value");
    redis.set("key3", "value");
    redis.set("key4", "value");

    VecString values;
    bool ret = redis.scan( 0, values, "key?" );
    while(ret && redis.scan( -1, values,"key?" ) );
    for(Poco::UInt64 i=0;i<values.size();i++)
    {
        string str = values[i];
        str =str.substr(0,3);
        EXPECT_EQ("key",str);
    }

    EXPECT_EQ(4,values.size());
}

TEST_F(CTestKey, exists)
{
    redis.flushall();
    redis.set("key", "value");
    EXPECT_EQ(true, redis.exists("key"));
}

TEST_F(CTestKey, move)
{
    redis.flushall();
    redis.set("key", "value");
    redis.move("key", 3);
    redis.select(3);
    EXPECT_EQ(true, redis.exists("key"));
}

TEST_F(CTestKey, expire_ttl)
{
    redis.flushall();
    string key("key");
    redis.set(key, "value");

    Timestamp stamp;
    int ttl=5;
    std::time_t t1 = stamp.epochTime()+ttl;
    redis.expireAt(key,t1);

    int life = redis.ttl(key);
    EXPECT_LE(life,ttl);
}

TEST_F(CTestKey, pExpire_pttl)
{
    redis.flushall();
    string key("key");
    redis.set(key, "value");

    Timestamp stamp;
    int ttl=5000;
    std::time_t t1 = stamp.epochMicroseconds()/1000+ttl;
    redis.pExpireAt(key,t1);

    Poco::Int64 pl = redis.pttl(key);
    EXPECT_LE(pl,ttl);
}

TEST_F(CTestKey, expire_persist)
{
    redis.flushall();
    string key("key");
    redis.set(key, "value");

    int ttl=5;
    redis.expire(key,ttl);

    int life = redis.ttl(key);
    EXPECT_EQ(life,ttl);

    redis.persist(key);
    life = redis.ttl(key);

    EXPECT_EQ(-1,life);
}


TEST_F(CTestKey, object)
{
    redis.flushall();
    string key("key");
    redis.set(key, "value");

    CResult result;
    redis.object(CRedisClient::REFCOUNT,key,result);

//    PRINTA(result.getInt());
    EXPECT_EQ(1,result.getInt());
}



TEST_F(CTestKey, randomKey)
{
    redis.flushall();
    string key("akey");
    redis.set(key, "value");

    string retKey;
    redis.randomKey(retKey);
    EXPECT_EQ(key, retKey);
}

TEST_F(CTestKey, rename)
{
    redis.flushall();
    string key("akey");
    string newKey("newKey");

    redis.set(key, "value");
    redis.rename(key,newKey);
    EXPECT_EQ(true, redis.exists(newKey));
}


TEST_F(CTestKey, sort)
{
    redis.flushall();
    string key("akey");

    VecString vals{"30","1.5","10","8"};
    VecString vals_sort{"1.5","8","10","30"};
    redis.lpush(key,vals);

    CResult result;
    redis.sort(key,result);

    EXPECT_EQ(CResult::REDIS_REPLY_ARRAY, result.getType());
    const CResult::ListCResult& rlist = result.getArry();

    //预期返回值为已序数字
    auto start = rlist.begin();
    auto end = rlist.end();
    for(int i=0;start!=end;start++,i++)
    {
        const string& str=*start;
        EXPECT_EQ(vals_sort[i],str);
    }

}

TEST_F(CTestKey, type)
{
    redis.flushall();
    string key("akey");
    string lkey("lkey");
    string skey("skey");
    string zkey("zkey");
    string hkey("hkey");
    redis.set(key, "value");
    redis.lpush(lkey,VecString{"30","1.5","10","8"});

    std::tuple<string,string> t1{"1","hello"};
    std::tuple<string,string> t2{"2","world"};
    VecTupleString vals{t1,t2};
    redis.zadd(zkey,vals);
    redis.sadd(skey,VecString{"hello"});
    redis.hset(hkey,"field","value");


    EXPECT_EQ(CRedisClient::NONE,redis.type("key"));
    EXPECT_EQ(CRedisClient::STRING,redis.type(key));
    EXPECT_EQ(CRedisClient::LIST,redis.type(lkey));
    EXPECT_EQ(CRedisClient::SET,redis.type(skey));
    EXPECT_EQ(CRedisClient::ZSET,redis.type(zkey));
    EXPECT_EQ(CRedisClient::HASH,redis.type(hkey));
}

TEST_F(CTestKey, dump_serial)
{
    redis.flushall();
    string key("akey");
    string oldVal("hello, dumping world!");
    string val,newkey("newkey");
    string serialKey;
    //    string ok{"\x00\x15hello, dumping world!\x06\x00E\xa0Z\x82\xd8r\xc1\xde"};
    redis.set(key, oldVal);

    redis.dump(key, serialKey);
    redis.restore(newkey,serialKey,0);
    redis.get(newkey,val);

    EXPECT_EQ(oldVal,val);
}


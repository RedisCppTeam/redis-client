#include "CTestString.h"

CRedisClient CTestString::redis;

void CTestString::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestString::TearDownTestCase()
{

}

TEST_F(CTestString, set)
{
    redis.flushall();
    redis.set("string", "value");
    ASSERT_NO_THROW(redis.set("string", "value"););
}

TEST_F(CTestString, get)
{
    redis.flushall();
    redis.set("string", "value");
    string value;
    redis.get("string", value);
    EXPECT_EQ("value", value);
}






TEST_F(CTestString, APPEND)
{
    redis.flushall();
    redis.append("myphone", "nokia");

    std::string value;
    redis.get("myphone", value);
    EXPECT_EQ("nokia", value);
}

TEST_F(CTestString, BITCOUNT)
{
    redis.flushall();

    uint64_t iRet = redis.bitcount("bits", 0, -1);
    EXPECT_EQ(0, iRet);

    redis.setbit_("bits", 0, "1");
    iRet = redis.bitcount("bits");
    EXPECT_EQ(1, iRet);
}

TEST_F(CTestString, BITOP)
{
    redis.flushall();

    //bits-1 = 1001
    redis.setbit_("bits-1", 0, "1");
    redis.setbit_("bits-1", 3, "1");
    //bits-2 = 1011
    redis.setbit_("bits-2", 0, "1");
    redis.setbit_("bits-2", 1, "1");
    redis.setbit_("bits-2", 3, "1");

    VecString keys;
    keys.push_back("bits-1");
    keys.push_back("bits-2");
    uint64_t iRet = redis.bitop("AND", "and-result", keys);


    iRet = redis.getbit("and-result", 0);
    EXPECT_EQ(1, iRet);

    iRet = redis.getbit("and-result", 1);
    EXPECT_EQ(0, iRet);

    iRet = redis.getbit("and-result", 2);
    EXPECT_EQ(0, iRet);

    iRet = redis.getbit("and-result", 3);
    EXPECT_EQ(1, iRet);

}


TEST_F(CTestString, DECR)
{
    redis.flushall();

    redis.set("failure_times", "10");
    int64_t iRet = redis.decr("failure_times");
    EXPECT_EQ(9, iRet);
}

TEST_F(CTestString, DECRBY)
{
    redis.flushall();

    redis.set("count", "100");
    int64_t iRet = redis.decrby("count", 20);
    EXPECT_EQ(80, iRet);
}

TEST_F(CTestString, GET)
{
    redis.flushall();

    redis.set("db", "redis");
    std::string value;
    redis.get("db", value);
    EXPECT_EQ("redis", value);
}


TEST_F(CTestString, GETBIT)
{
    redis.flushall();

    redis.setbit_("bit", 10086, "1");
    int iRet = redis.getbit("bit", 10086);
    EXPECT_EQ(1, iRet);
}

TEST_F(CTestString, GETRANGE)
{
    redis.flushall();

    redis.set("greeting", "hello, my friend");
    std::string value;
    redis.getrange("greeting", 0, 4, value);
    EXPECT_EQ("hello", value);
}


TEST_F(CTestString, GETSET)
{
    redis.flushall();

    std::string oldvalue;
    bool bRet = redis.getset("db", "mongodb", oldvalue);

    redis.get("db", oldvalue);

    bRet = redis.getset("db", "redis", oldvalue);
    EXPECT_EQ("mongodb", oldvalue);

    redis.get("db", oldvalue);
    EXPECT_EQ("redis", oldvalue);
}

TEST_F(CTestString, INCR)
{
    redis.flushall();

    redis.set("page_view", "20");
    redis.incr("page_view");

    std::string value;
    redis.get("page_view", value);
    EXPECT_EQ("21", value);
}

TEST_F(CTestString, INCRBY)
{
    redis.flushall();

    redis.set("rank", "50");
    int64_t iRet = redis.incrby("rank", 20);

    std::string value;
    redis.get("rank", value);
    EXPECT_EQ("70", value);
}


TEST_F(CTestString, INCRBYFLOAT)
{
    redis.flushall();

    redis.set("mykey", "10.50");
    float fValue;
    redis.incrbyfloat("mykey", 0.1, fValue);


    std::string value;
    redis.get("mykey", value);
    EXPECT_EQ("10.6", value);
}



TEST_F(CTestString, MGET)
{
    redis.flushall();

    redis.set("redis", "redis.com");
    redis.set("mongodb", "mongodb.org");
    VecString keys;
    keys.push_back("redis");
    keys.push_back("mongodb");
    CResult result;
    redis.mget(keys, result);

    CResult::ListCResult::const_iterator it = result.getArry().begin();
    CResult::ListCResult::const_iterator end = result.getArry().end();
    for ( int i = 0; it != end; ++it, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("redis.com", static_cast<string>(*it));
        }
        else if (i == 1)
        {
            EXPECT_EQ("mongodb.org", static_cast<string>(*it));
        }
    }

}


TEST_F(CTestString, MSET)
{
    redis.flushall();

     VecTupleString value{std::tuple<string,string>("date","2012.3.30"),
                 std::tuple<string,string>("time","11:00 a.m.")};

    redis.mset(value);

    CResult result;
    VecString keys;
    keys.push_back("date");
    keys.push_back("time");
    redis.mget(keys, result);
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    CResult::ListCResult::const_iterator end = result.getArry().end();
    for ( int i = 0; it != end; ++it, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("2012.3.30", static_cast<string>(*it));
        }
        else if (i == 1)
        {
            EXPECT_EQ("11:00 a.m.", static_cast<string>(*it));
        }
    }
}


TEST_F(CTestString, MSETNX)
{
    redis.flushall();

    VecTupleString value{std::tuple<string,string>("rmdbs","MySQL"),
                std::tuple<string,string>("nosql","MongoDB")};
    int iRet = redis.msetnx(value);


    CResult result;
    VecString keys;
    keys.push_back("rmdbs");
    keys.push_back("nosql");
    redis.mget(keys, result);
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    CResult::ListCResult::const_iterator end = result.getArry().end();
    for ( int i = 0; it != end; ++it, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("MySQL", static_cast<string>(*it));
        }
        else if (i == 1)
        {
            EXPECT_EQ("MongoDB", static_cast<string>(*it));
        }
    }
}


TEST_F(CTestString, PSETEX)
{
    redis.flushall();

    redis.setPX("mykey", "Hello", 1000);
    std::string value;
    redis.get("mykey", value);
    EXPECT_EQ("Hello", value);
}


TEST_F(CTestString, SETBIT)
{
    redis.flushall();

    int iRet = redis.setbit_("bit", 10086, "1");

    iRet = redis.getbit("bit", 10086);
    EXPECT_EQ(1, iRet);
}


TEST_F(CTestString, SETEX)
{
    redis.flushall();

    redis.setEX("cache_user_id", "10086", 60);

    std::string value;
    redis.get("cache_user_id", value);
    EXPECT_EQ("10086", value);
}


TEST_F(CTestString, SETNX)
{
    redis.flushall();

    bool bRet = redis.setNX("job", "programmer");

    std::string value;
    redis.get("job", value);
    redis.setNX("job", "code-farmer");

    redis.get("job", value);
    EXPECT_EQ("programmer", value);
}

TEST_F(CTestString, SETRANGE)
{
    redis.flushall();

    redis.set("greeting", "hello world");
    uint64_t iRet = redis.setrange("greeting", 6, "Redis");

    std::string value;
    redis.get("greeting", value);
    EXPECT_EQ("hello Redis", value);
}


TEST_F(CTestString, STRLEN)
{
    redis.flushall();

    redis.set("mykey", "Hello world");
    uint64_t iRet = redis.strlen("mykey");
    EXPECT_EQ(11, iRet);
}



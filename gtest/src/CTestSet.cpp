#include "CTestSet.h"

CRedisClient CTestSet::redis;

void CTestSet::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestSet::TearDownTestCase()
{

}

TEST_F(CTestSet, sadd)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("A");
    EXPECT_EQ(2, redis.sadd("set", vec));
    EXPECT_EQ(1, redis.sadd("set", VecString{"C"}));
    EXPECT_EQ(0, redis.sadd("set", VecString{"A"}));
}


TEST_F(CTestSet, scard)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    EXPECT_EQ(3, redis.sadd("set", vec));
    EXPECT_EQ(3, redis.scard("set"));
}

TEST_F(CTestSet, sdiff)
{
    redis.flushall();
    VecString vec;

    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set0", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("B");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set2", vec);

    VecString keys = {"set0", "set1", "set2"};
    VecString values;
    EXPECT_EQ(1, redis.sdiff(keys, values));
    EXPECT_EQ("A", values[0]);
}


TEST_F(CTestSet, sdiffstore)
{
    redis.flushall();
    VecString vec;

    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set0", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("B");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set2", vec);

    VecString keys = {"set0", "set1", "set2"};

    EXPECT_EQ(1, redis.sdiffstore("set", keys));
    EXPECT_EQ(true, redis.sismember("set", "A"));

}

TEST_F(CTestSet, sinter)
{
    redis.flushall();
    VecString vec;

    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set0", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("B");
    vec.push_back("E");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("B");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set2", vec);


    VecString keys = {"set0", "set1", "set2"};
    VecString values;
    EXPECT_EQ(1, redis.sinter(keys, values));
    EXPECT_EQ("B", values[0]);

}

TEST_F(CTestSet, sinterstore)
{
    redis.flushall();
    VecString vec;

    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set0", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("B");
    vec.push_back("E");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("B");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set2", vec);


    VecString keys = {"set0", "set1", "set2"};

    EXPECT_EQ(1, redis.sinterstore("set", keys));
    EXPECT_EQ(true, redis.sismember("set", "B"));
}


TEST_F(CTestSet, sismember)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set", vec);
    EXPECT_EQ(true, redis.sismember("set", "A"));
    EXPECT_EQ(true, redis.sismember("set", "B"));
    EXPECT_EQ(true, redis.sismember("set", "C"));
    EXPECT_EQ(false, redis.sismember("set", "D"));

}

TEST_F(CTestSet, smembers)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set", vec);
    VecString sets;
    redis.smembers("set", sets);
    auto it = std::find(vec.begin(), vec.end(), "A");
    EXPECT_EQ(vec.begin(), it);
    it = std::find(vec.begin(), vec.end(), "D");
    EXPECT_EQ(vec.end(), it);
}

TEST_F(CTestSet, smove)
{
    redis.flushall();
    VecString vec;


    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("D");
    redis.sadd("set2", vec);

    redis.smove("set1", "set2", "A");
    EXPECT_EQ(false, redis.sismember("set1", "A"));
    EXPECT_EQ(true, redis.sismember("set2", "A"));

}


TEST_F(CTestSet, spop)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set", vec);

    string value;
    redis.spop("set", value);

    VecString sets;
    redis.smembers("set", sets);
    EXPECT_EQ(2, sets.size());
}

TEST_F(CTestSet, srandmember)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set", vec);

    VecString values;
    redis.srandmember("set", 4, values);
    EXPECT_EQ(3, values.size());
}

TEST_F(CTestSet, srem)
{
    redis.flushall();
    VecString vec;


    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    redis.sadd("set", vec);

    redis.srem("set", VecString{"A"});
    EXPECT_EQ(false, redis.sismember("set", "A"));
}

TEST_F(CTestSet, sunion)
{
    redis.flushall();
    VecString vec;

    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set0", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("B");
    vec.push_back("E");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("B");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set2", vec);


    VecString keys = {"set0", "set1", "set2"};
    VecString values;
    EXPECT_EQ(5, redis.sunion(keys, values));
    EXPECT_EQ(5, values.size());

}

TEST_F(CTestSet, sunionstore)
{
    redis.flushall();
    VecString vec;

    vec.clear();
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("C");
    redis.sadd("set0", vec);

    vec.clear();
    vec.push_back("C");
    vec.push_back("B");
    vec.push_back("E");
    redis.sadd("set1", vec);

    vec.clear();
    vec.push_back("B");
    vec.push_back("D");
    vec.push_back("E");
    redis.sadd("set2", vec);


    VecString keys = {"set0", "set1", "set2"};

    EXPECT_EQ(5, redis.sunionstroe("set", keys));
}

TEST_F(CTestSet, sscan)
{
    redis.flushall();
    string value = "value_";
    std::stringstream ss;
    VecString sscanMembers1;
    for ( int i = 0; i < 100; ++i )
    {
        value = "value_";
        ss.clear();
        ss.str("");
        ss << i;
        value += ss.str();
        sscanMembers1.push_back( value );
    }
    redis.sadd( "set", sscanMembers1 );

    VecString sscanMembers;
    int64_t cur = 0;
    while ( redis.sscan( "set", cur, sscanMembers ,"value_?") );
    EXPECT_EQ(10, sscanMembers.size());
    sscanMembers.clear();
    while ( redis.sscan( "set", cur, sscanMembers) );
    EXPECT_EQ(100, sscanMembers.size());
}




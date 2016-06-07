#include "CTestList.h"

CRedisClient CTestList::redis;

void CTestList::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestList::TearDownTestCase()
{

}

TEST_F(CTestList, LPUSH)
{
    redis.flushall();

    VecString vec;
    std::string key="test";
    vec.push_back("value1");
    vec.push_back("value2");
    EXPECT_EQ(2, redis.lpush(key, vec));

    vec.clear();
    redis.lrange(key,0,-1,vec);
    VecString::iterator it=vec.begin();
    VecString::iterator end=vec.end();
    for(int i=0;it!=end;i++,it++){
        if(0==i){
            EXPECT_EQ("value2",*it);
        }else if(1==i){
            EXPECT_EQ("value1",*it);
        }
    }
}

TEST_F(CTestList,RPUSH){
    redis.flushall();

    VecString vec;
    std::string key="test";
    vec.push_back("value1");
    vec.push_back("value2");
    EXPECT_EQ(2,redis.rpush(key,vec));

    vec.clear();
    redis.lrange(key,0,-1,vec);
    VecString::iterator it=vec.begin();
    VecString::iterator end=vec.end();
    for(int i=0;it!=end;i++,it++){
        if(0==i){
            EXPECT_EQ("value1",*it);
        }else if(1==i){
            EXPECT_EQ("value2",*it);
        }
    }

}

TEST_F(CTestList,LPOP){
    redis.flushall();

    VecString vec;
    std::string key="test";
    std::string value;
    vec.push_back("value1");
    vec.push_back("value2");
    redis.rpush(key,vec);
    redis.lpop(key,value);
    EXPECT_EQ("value1",value);

    vec.clear();
    redis.lrange(key,0,-1,vec);
    VecString::iterator it=vec.begin();
    EXPECT_EQ("value2",*it);
}

TEST_F(CTestList,RPOP){
    redis.flushall();

    VecString vec;
    std::string key="test";
    std::string value;
    vec.push_back("value1");
    vec.push_back("value2");
    redis.rpush(key,vec);
    redis.rpop(key,value);
    EXPECT_EQ("value2",value);

    vec.clear();
    redis.lrange(key,0,-1,vec);
    VecString::iterator it=vec.begin();
    EXPECT_EQ("value1",*it);
}

TEST_F(CTestList,LINDEX){
    redis.flushall();

    VecString vec;
    std::string key="test";
    std::string value;
    vec.push_back("value1");
    vec.push_back("value2");
    vec.push_back("value3");
    redis.rpush(key,vec);
    redis.lindex(key,1,value);
    EXPECT_EQ("value2",value);
}

TEST_F(CTestList,LLEN){
    redis.flushall();

    VecString vec;
    std::string key="test";
    vec.push_back("value1");
    vec.push_back("value2");
    redis.rpush(key,vec);
    EXPECT_EQ(2,redis.llen(key));
}

TEST_F(CTestList,LPUSHX){
    redis.flushall();

    std::string key="test";
    EXPECT_EQ(0,redis.lpushx(key,"value1"));

    VecString vec;
    vec.push_back("value1");
    redis.rpush(key,vec);
    EXPECT_EQ(2,redis.lpushx(key,"value2"));
}

TEST_F(CTestList,RPUSHX){
    redis.flushall();

    std::string key="test";
    EXPECT_EQ(0,redis.rpushx(key,"value1"));
    VecString vec;
    vec.push_back("value1");
    redis.rpush(key,vec);
    EXPECT_EQ(2,redis.rpushx(key,"value2"));
}

TEST_F(CTestList,LINSERT){
    redis.flushall();

    std::string key="test";
    EXPECT_EQ(0,redis.linsert(key,CRedisClient::BEFORE,"zhongwei","hello"));

    VecString vec;
    vec.push_back("value1");
    redis.rpush(key,vec);
    EXPECT_EQ(-1,redis.linsert(key,CRedisClient::BEFORE,"zhongwei","hello"));

    EXPECT_EQ(2,redis.linsert(key,CRedisClient::BEFORE,"value1","hello"));

}

TEST_F(CTestList,LREM){
    redis.flushall();

    std::string key="test";
    VecString vec;
    vec.push_back("value1");
    vec.push_back("value2");
    vec.push_back("value1");
    redis.rpush(key,vec);
    EXPECT_EQ(0,redis.lrem("111",0,"value1"));

    EXPECT_EQ(2,redis.lrem(key,0,"value1"));

}

TEST_F(CTestList,LTRIM){
    redis.flushall();

    VecString vec;
    std::string key="test";
    vec.push_back("value1");
    vec.push_back("value2");
    vec.push_back("value3");
    redis.rpush(key,vec);
    redis.ltrim(key,1,2);

    vec.clear();
    redis.lrange(key,0,-1,vec);
    VecString::iterator it =vec.begin();
    VecString::iterator end=vec.end();
    for(int i=0;it!=end;i++,it++){
        if(0==i){
            EXPECT_EQ("value2",*it);
        }else if(1==i){
            EXPECT_EQ("value3",*it);
        }
    }
}

TEST_F(CTestList,LSET){
    redis.flushall();

    VecString vec;
    std::string key="test";
    vec.push_back("value1");
    vec.push_back("value2");
    redis.rpush(key,vec);
    redis.lset(key,1,"b");

    vec.clear();
    redis.lrange(key,0,-1,vec);
    VecString::iterator it=vec.begin();
    it++;
    EXPECT_EQ("b",*it);

}

TEST_F(CTestList,LRANGE){
    redis.flushall();

    VecString vec;
    std::string key="test";
    vec.push_back("value1");
    vec.push_back("value2");
    vec.push_back("value3");
    redis.rpush(key,vec);
    EXPECT_EQ(3,redis.lrange(key,0,-1,vec));
}

TEST_F(CTestList,RPOPLPUSH){
    redis.flushall();

    VecString vec,vec2;
    std::string key="test";
    std::string key2="test2";
    vec.push_back("A");
    vec.push_back("B");
    vec2.push_back("1");
    vec2.push_back("2");
    redis.rpush(key,vec);
    redis.rpush(key2,vec2);
    std::string value;
    redis.rpoplpush(key,key2,value);
    EXPECT_EQ("B",value);

    vec.clear();
    vec2.clear();
    redis.lrange(key,0,-1,vec);
    redis.lrange(key2,0,-1,vec2);
    VecString::iterator itVec=vec.begin();
    EXPECT_EQ(1,vec.size());
    EXPECT_EQ("A",*itVec);
    VecString::iterator itVec2=vec2.begin();
    EXPECT_EQ(3,vec2.size());
    EXPECT_EQ("B",*itVec2);

}

TEST_F(CTestList,BLPOP){
    redis.flushall();

    std::string key="key";
    std::string key2="key2";
    VecString keys;
    keys.push_back(key);
    keys.push_back(key2);
    uint64_t timeout=2;
    VecTupleString value;

    EXPECT_EQ(false,redis.blpop(keys,timeout,value));

    VecString keyValue;
    keyValue.push_back("a");
    keyValue.push_back("b");
    VecString key2Value;
    key2Value.push_back("1");
    key2Value.push_back("2");

    redis.rpush(key,keyValue);
    redis.rpush(key2,key2Value);

    EXPECT_EQ(true,redis.blpop(keys,timeout,value));
    VecTupleString::iterator it=value.begin();
    VecTupleString::iterator end=value.end();

    EXPECT_EQ("key",std::get<0>(*it));
    EXPECT_EQ("a",std::get<1>(*it));
}

TEST_F(CTestList,BRPOP){
    redis.flushall();

    std::string key="key";
    std::string key2="key2";
    VecString keys;
    keys.push_back(key);
    keys.push_back(key2);
    uint64_t timeout=2;
    VecTupleString value;

    EXPECT_EQ(false,redis.brpop(keys,timeout,value));

    VecString keyValue;
    keyValue.push_back("a");
    keyValue.push_back("b");
    VecString key2Value;
    key2Value.push_back("1");
    key2Value.push_back("2");

    redis.rpush(key,keyValue);
    redis.rpush(key2,key2Value);

    EXPECT_EQ(true,redis.brpop(keys,timeout,value));
    VecTupleString::iterator it=value.begin();
    VecTupleString::iterator end=value.end();

    EXPECT_EQ("key",std::get<0>(*it));
    EXPECT_EQ("b",std::get<1>(*it));
}

TEST_F(CTestList,BRPOPLPUSH){
    redis.flushall();

    VecString vec,vec2;
    std::string value;
    uint64_t timeout=2;
    std::string key="test";
    std::string key2="test2";
    EXPECT_EQ(false,redis.brpoplpush(key,key2,timeout,value));

    vec.push_back("A");
    vec.push_back("B");
    vec2.push_back("1");
    vec2.push_back("2");
    redis.rpush(key,vec);
    redis.rpush(key2,vec2);
    redis.brpoplpush(key,key2,timeout,value);
    EXPECT_EQ("B",value);

    vec.clear();
    vec2.clear();
    redis.lrange(key,0,-1,vec);
    redis.lrange(key2,0,-1,vec2);
    VecString::iterator itVec=vec.begin();
    EXPECT_EQ(1,vec.size());
    EXPECT_EQ("A",*itVec);
    VecString::iterator itVec2=vec2.begin();
    EXPECT_EQ(3,vec2.size());
    EXPECT_EQ("B",*itVec2);
}

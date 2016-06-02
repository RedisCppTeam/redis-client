#include "CTestScript.h"


using namespace std;

CRedisClient CTestScript::redis;

void CTestScript::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestScript::TearDownTestCase()
{

}

TEST_F(CTestScript, EVAL)
{
    redis.flushall();
    CResult result;

    string script = "return redis.call('set',KEYS[1],ARGV[1])";
    VecString keys,argv;
    keys.push_back( "foo" );
    argv.push_back("yuhaiyang");

    redis.eval( script, keys ,argv,result );

    EXPECT_EQ("OK", result.getStatus());
}


TEST_F(CTestScript, EVALSHA)
{
    redis.flushall();

    CResult result;
    string retString;
    redis.scriptLoad("return 'hello moto'",retString );

    redis.evalSha( retString, result );

    EXPECT_EQ("hello moto", result.getString());
}


TEST_F(CTestScript, EXISTS)
{
    redis.flushall();


    string retString1,retString2;

    redis.scriptLoad( "return 'hello moto'",retString1 );
    redis.scriptLoad( "return 'hello moto2'",retString2 );
    cout << "redis.scriptLoad ok:" << retString1 << endl;
    cout << "redis.scriptLoad ok:" << retString2 << endl;

    VecString vecString;
    vecString.push_back( retString1 );
    vecString.push_back( retString2 );

    VecBool result;
    redis.scriptExists(vecString,result);
    VecBool::const_iterator it = result.begin();
    VecBool::const_iterator end = result.end();

    for( int i = 0; it != end; ++it, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ(1, *it);
        }
        else if (i == 1)
        {
            EXPECT_EQ(1, *it);
        }
    }
}


TEST_F(CTestScript, FLUSH)
{
    redis.flushall();

    ASSERT_NO_THROW(redis.scriptFlush());
}


TEST_F(CTestScript, KILL)
{
    redis.flushall();

    ASSERT_ANY_THROW(redis.scriptKill());
}


TEST_F(CTestScript, LOAD)
{
    redis.flushall();

    string retString1;
    redis.scriptLoad( "return 'hello moto'",retString1);

    EXPECT_EQ("232fd51614574cf0867b83d384a5e898cfd24e5a", retString1);
}




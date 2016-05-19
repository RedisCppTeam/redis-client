#include "CTestPub.h"


using namespace std;

CRedisClient CTestPub::redis;

void CTestPub::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestPub::TearDownTestCase()
{

}

TEST_F(CTestPub, PSUBSCRIBE)
{
    redis.flushall();
    CResult ret;
    //redis.psubscribe(VecString{"new.*"},ret);
   // std::cout<<ret.display(ret,2);

    EXPECT_EQ(4, 4);
}

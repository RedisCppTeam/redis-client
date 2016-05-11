#ifndef CTESTSORTEDSET_H
#define CTESTSORTEDSET_H
#include <gtest/gtest.h>
#include "CRedisClient.h"
using namespace Taiji::Redis;
class CTestSortedSet : public testing::Test
{
public:
    CTestSortedSet() = default;
    ~CTestSortedSet() = default;
    static void SetUpTestCase( void );
    static void TearDownTestCase( void );
    static void SetVecTuple(CRedisClient::VecTuple& map,const string& mapName,uint64_t start,uint64_t end);
    static CRedisClient  redis;


};

#endif // CTESTSORTEDSET_H

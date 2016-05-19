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

    //向vecTup内填充end-start个数据，每个数据是Trup<"n","keyPre_n">
    static void GetVecTuple(VecTupleString& vecTup, const string& keyPre, uint64_t start, uint64_t end);
    static CRedisClient  redis;


};

#endif // CTESTSORTEDSET_H

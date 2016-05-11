#ifndef CTESTSET_H
#define CTESTSET_H
#include <gtest/gtest.h>
#include "CRedisClient.h"
using namespace Taiji::Redis;
class CTestSet : public testing::Test
{
public:
    CTestSet() = default;
    ~CTestSet() = default;
    static void SetUpTestCase( void );
    static void TearDownTestCase( void );

    static CRedisClient redis;
};



#endif // CTESTSET_H

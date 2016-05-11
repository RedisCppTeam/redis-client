#ifndef CTESTKEY_H
#define CTESTKEY_H
#include <gtest/gtest.h>
#include "CRedisClient.h"
using namespace Taiji::Redis;
class CTestKey : public testing::Test
{
public:
    CTestKey() = default;
    ~CTestKey() = default;
    static void SetUpTestCase( void );
    static void TearDownTestCase( void );

    static CRedisClient redis;
};

#endif // CTESTKEY_H

#ifndef CTESTREDIS_H
#define CTESTREDIS_H
#include <gtest/gtest.h>
#include "CRedisPool.h"

class CTestRedis : public testing::Test
{
public:
    CTestRedis() = default;
    ~CTestRedis() = default;
    static void SetUpTestCase( void );
    static void TearDownTestCase( void );
};

#endif // CTESTREDIS_H

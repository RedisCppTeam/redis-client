#ifndef CTESTPUB_H
#define CTESTPUB_H
#include <gtest/gtest.h>
#include "CRedisClient.h"
using namespace Taiji::Redis;


class CTestPub : public testing::Test
{
public:
    CTestPub() = default;
    ~CTestPub() = default;
    static void SetUpTestCase( void );
    static void TearDownTestCase( void );

    static CRedisClient redis;
};



#endif // CTESTPUB_H

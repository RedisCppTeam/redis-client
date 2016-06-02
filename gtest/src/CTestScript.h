#ifndef CTESTSCRIPT_H
#define CTESTSCRIPT_H
#include <gtest/gtest.h>
#include "CRedisClient.h"
using namespace Taiji::Redis;


class CTestScript : public testing::Test
{
public:
    CTestScript() = default;
    ~CTestScript() = default;
    static void SetUpTestCase( void );
    static void TearDownTestCase( void );

    static CRedisClient redis;
};



#endif // CTESTSCRIPT_H

/**
 * @file	testSet.cpp
 * @brief 测试 Transaction 模块
 *
 * @author: 		yuhaiyang
 * @date: 		2015年8月18日
 *
 */


#include "../Command.h"
#include "../CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "../RdException.hpp"
#include "../CResult.h"


//////////////////////////////////////// test transaction////////////////////////////////////////
void TranSactionMain( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        redis.set( "yuhaiyang", "nan" );
        redis.set( "yuhaiyang2", "nan2" );
        CResult result;

        //------------------------test multi, transactionCmd, exec---------------------
        DEBUGOUT("test multi, transactionCmd, and exec","");
        redis.multi();
        CRedisClient::VecString params;
        params.push_back( "*" );
        redis.transactionCmd( "keys", params );

        params.clear();
        params.push_back("yuhaiyang");
        redis.transactionCmd("get", params );
        redis.exec( result );

        DEBUGOUT("result", result );

        //---------------------------Watch---------------------------------------
        result.clear();
        params.clear();
        CRedisClient::VecString keys;
        keys.push_back( "yuhaiyang" );
        keys.push_back("yuhaiyang2");
        redis.watch( keys );

        redis.multi();
        params.push_back("yuhaiyang");
        params.push_back("yuhaiyang2");
        redis.transactionCmd("mget",params);
        getchar();
        redis.exec( result );
        DEBUGOUT("result", result );

    }catch ( std::exception& e )
    {
        std::cout << e.what() << std::endl;
    }


}


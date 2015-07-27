/**
 *
 * @file	main.cpp
 * @brief CRedisClient 测试代码。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 * 修订说明:初始版本
 */
#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>

#include "RdException.hpp"
#include "CResult.h"

using namespace std;


/*
void TestKey( void )
{
    try
    {
        CRedisClient::VecResult value;
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 10 ; i++ )
        {
            //--------------------------keys------------------------------
            redis.keys("test*", value );
            CRedisClient::VecResult::iterator it = value.begin();

            std::cout << "------------------------start value---------------------------" << std::endl;
            for ( ; it != value.end(); it++ )
            {
               DEBUGOUT( "value", *it );
            }
            std::cout << "----------------------end value----------------------------" << std::endl;

            CRedisClient::VecString keys;
            keys.push_back( "testHash" );
            keys.push_back( "testString" );
            uint64_t num = redis.del( keys );

            std::cout << "del data:" << num << std::endl;

            sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}


void TestList( void )
{
    try
    {
        CRedisClient::VecString value;
        CResult ret;
        value.push_back( "yu" );
        value.push_back( "hai" );
        value.push_back( "yang" );

        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 10 ; i++ )
        {
            std::cout << "======================testList=========================" << std::endl;
            //uint64_t num = redis.lpush( "name", value );
            //std::cout << "num" << num << std::endl;

            int32_t listNum = redis.lpop( "name", ret );
            std::cout << "listNume: " << listNum << std::endl;
            std::cout << "listRet: " << ret << std::endl;

            sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void TestString( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );

        CResult value;
        for ( int i = 0; i < 10 ; i++ )
        {

           std::cout << "====testString====" << std::endl;
           //------------------test set---------------------------
           //redis.set( "name", "yanxingjun" );

           //------------------test get---------------------------
            int32_t num = redis.get( "name",value );
            std::cout << "num: " << num << std::endl;
            std::cout << "value:" << value << std::endl;

            //sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void TestHash( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );

        CResult result;

        int32_t hashRet = redis.hset( "testHash", "name2", "yang" );
        std::cout << "hashRet: " << hashRet << std::endl;

        int32_t num = redis.hget( "testHash", "name3" ,result );

        std::cout << "num: " << num << std::endl;
        std::cout << result << std::endl;
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}


//  写到 TestHash().
//int main()
//{
//    CRedisClient redis;
//    redis.connect( "127.0.0.1", 6379 );
//
//    CRedisClient::VecResult result;
//    redis.keys( "*", result );
//    CRedisClient::VecResult::iterator it = result.begin();
//
//    for ( ; it != result.end(); it++ )
//    {
//        std::cout << *it << std::endl;
//    }
//
//    //TestList();
//    //TestHash();
//    //TestKey();
//    TestString();
//}



///////////////////////////////////////// test CResult////////////////////////////////////////
//int main()
//{
//    CResult result("123");
//    result.setType( REDIS_REPLY_STRING );
//
//    result += "345";
//    CResult result2  ;
//    result2 = result;
//
//   // result.clear();
//
//    std::cout << result<< std::endl;
//    std::cout << result2<< std::endl;
//}



//////////////////////////////////////// test transaction////////////////////////////////////////
int main()
{
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    CRedisClient::VecString keys;
    keys.push_back( "user_name_4");

    redis.watch(  keys );

    return 0;
}


*/


int main()
{
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    CResult result;
    redis.multi();
    CRedisClient::VecString params;
    params.push_back( "*" );
    redis.runCmd("keys", params );
    params.clear();
    params.push_back("asd");
    redis.runCmd("get", params );
    redis.exec( result );

    std::cout << result << std::endl;

    CRedisClient::VecString ret;
    redis.keys( "*", ret );
    CRedisClient::VecString::const_iterator it = ret.begin();
    for ( ; it != ret.end(); it++ )
        std::cout << *it << std::endl;
}







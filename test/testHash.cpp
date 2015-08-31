/**
 * @file	testSet.cpp
 * @brief 测试 Hash 模块
 *
 * @author: 		yuhaiyang
 * @date: 		2015年8月18日
 *
 */
#include <iostream>
#include "../Command.h"
#include "../CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "../RdException.hpp"
#include "../CResult.h"

using namespace std;


void TestHash( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );

        //-------------------------test hset hget---------------------------------
        string value;
        int32_t hashRet = redis.hset( "testHash", "name5", "{ \n\"value\" : \"yuhaiyang\"\r\n}\n" );
        std::cout << "hashRet: " << hashRet << std::endl;

        bool ret = redis.hget( "testHash", "name5" ,value );

        std::cout << "ret: " << ret << std::endl;
        std::cout << "value:" << value << std::endl;

        // //------------------------test hdel------------------------------------------
        // CRedisClient::VecString fields;
        // fields.push_back( "name4" );
        // fields.push_back( "name5" );

        // uint64_t delNum = redis.hdel( "member", fields );
        // std::cout << "delNum: " << delNum << std::endl;
        // //------------------------test hexists---------------------------------------
        // string field  = "name4";
        // bool isExists = redis.hexists( "testHash", field );
        // if ( isExists )
        //     std::cout << "testHash key exists field:" << field << std::endl;
        // else
        //     std::cout << "testHash key not exists field:" << field << std::endl;
        //-----------------------test hgetall-------------------------------------------
        //    CRedisClient::MapString allValue;
        //    uint64_t allNum = redis.hgetall( "testHash", allValue );
        //    std::cout << "allNum: " << allNum <<std::endl;
        //    CRedisClient::MapString::const_iterator it = allValue.begin();

        //    for ( ; it != allValue.end(); it++ )
        //    {
        //        std::cout << it->first << ":" << it->second << std::endl;
        //    }
        //------------------------test incrby-------------------------------------------
        //      uint64_t incrybyNum = redis.hincrby( "testHash", "num2", 20 );
        //      std::cout << "incrybyNum: " << incrybyNum << std::endl;
        //      //------------------------test incrbyfloat-------------------------------------
        //      float floatNum = redis.hincrbyfloat( "testHash", "float", 10.1e2 );
        //      std::cout << "floatNum: " << floatNum << std::endl;
        //------------------------test hkeys-------------------------------------------
        //    CRedisClient::VecString hkeysValue;
        //    uint64_t hkeysNum = redis.hkeys( "testHash", hkeysValue );
        //    std::cout << "hkeysNum: " << hkeysNum << std::endl;

        //    CRedisClient::VecString::const_iterator hkeysit = hkeysValue.begin();
        //    for ( ; hkeysit != hkeysValue.end(); hkeysit++ )
        //    {
        //        std::cout << *hkeysit << std::endl;
        //    }
        //------------------------test hlen-----------------------------------------------
        //     uint64_t fieldNum = redis.hlen( "testHash" );
        //     std::cout << "fieldNum: " << fieldNum << std::endl;
        //     //------------------------test hmget---------------------------------------------
        //     CResult result;
        //     CRedisClient::VecString hmgeFields;
        //     hmgeFields.push_back("name4");
        //     hmgeFields.push_back("yuhaiyang");
        //     hmgeFields.push_back("num");
        //     redis.hmget( "testHash", hmgeFields,result );
        //     std::cout << "hmget:" << std::endl;
        //     std::cout << result << std::endl;
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void TestHash2()
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        CResult result;
        //-------------------------------test hmset---------------------------
        //       CRedisClient::MapString pairs;
        //       string pair, value;
        //       std::stringstream ss;
        //       for ( int i = 0; i < 1000; i++ )
        //       {
        //           pair = "pair_";
        //           value = "value_";
        //           ss.str("");

        //           ss << i;
        //           pair += ss.str();
        //           value += ss.str();
        //           pairs.insert( CRedisClient::MapString::value_type(pair,value) );
        //       }
        //       redis.hmset( "testHash", pairs );
        //-------------------------------test hsetnx---------------------------
        //       bool hsetNxRet = redis.hsetnx( "testHash", "num4", "123" );
        //       if ( hsetNxRet )
        //          std::cout << "ok" << std::endl;
        //       else
        //           std::cout << "false" << std::endl;
        //------------------------------test hvals--------------------------------
        //        CRedisClient::VecString hvals;
        //        uint64_t num = redis.hvals( "testHash", hvals );
        //        std::cout << "num: " << num << std::endl;
        //        CRedisClient::VecString::const_iterator it = hvals.begin();
        //        CRedisClient::VecString::const_iterator end = hvals.end();
        //        for ( ; it != end; ++it )
        //        {
        //            std::cout << *it << std::endl;
        //        }
        //
        //-------------------------------test hscan-----------------------------

        std::cout << "====================hscan value===================" << std::endl;
        CRedisClient::MapString hscanPairs;

        redis.hscan( "testHash", 0, hscanPairs,"pair_1??" );
        while ( redis.hscan( "testHash", -1, hscanPairs ,"pair_1??") );

        CRedisClient::MapString::const_iterator hscanIt = hscanPairs.begin();
        CRedisClient::MapString::const_iterator hscanEnd = hscanPairs.end();

        for ( ; hscanIt != hscanEnd ; ++hscanIt )
        {
            std::cout << hscanIt->first <<": " << hscanIt->second << std::endl;
        }
        std::cout << "totalNum: " << hscanPairs.size() << std::endl;


    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void Hmain( void )
{
    TestHash();
    TestHash2();
}


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
#include <sstream>
#include "RdException.hpp"
#include "CResult.h"

using namespace std;



void TestKey( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 5 ; i++ )
        {
            CRedisClient::VecString value;
            //--------------------------keys------------------------------
            std::cout << "keysNum:" << redis.keys("test*", value ) << std::endl;
            CRedisClient::VecString::iterator it = value.begin();

            std::cout << "------------------------start value---------------------------" << std::endl;
            for ( ; it != value.end(); it++ )
            {
               DEBUGOUT( "value", *it );
            }
            std::cout << "----------------------end value----------------------------" << std::endl;

            CRedisClient::VecString keys;
            keys.push_back( "testStringList" );
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
        string ret;
        value.push_back( "yu" );

        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 5 ; i++ )
        {
            std::cout << "======================testList=========================" << std::endl;
            int64_t listNum1 = redis.lpush( "name", value );
            std::cout << "listNum1:" << listNum1 << std::endl;

            int32_t listNum2 = redis.lpop( "name", ret );
            std::cout << "listNume2: " << listNum2 << std::endl;
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

        string value;
        CResult result;


           std::cout << "====testString====" << std::endl;
           //------------------test set---------------------------
           redis._set( "name", "123" );

           //------------------test set---------------------------
           // bool setRet = redis.setPX( "name2", "yuhaiyang", 20000,NX );
           bool setRet = redis.setXX( "name2", "yuhaiyang");
           std::cout << "setRet: " << setRet << std::endl;

           //------------------test get---------------------------
           bool strRet = redis.get( "name",value );
           std::cout << "strRet: " << strRet << std::endl;
           std::cout << "value:" << value << std::endl;

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

        //-------------------------test hset hget---------------------------------
       // string value;
       // int32_t hashRet = redis.hset( "testHash", "name5", "yang" );
       // std::cout << "hashRet: " << hashRet << std::endl;

       // bool ret = redis.hget( "testHash", "name4" ,value );

       // std::cout << "ret: " << ret << std::endl;
       // std::cout << "value:" << value << std::endl;

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
        CRedisClient::MapString allValue;
        uint64_t allNum = redis.hgetall( "testHash", allValue );
        std::cout << "allNum: " << allNum <<std::endl;
        CRedisClient::MapString::const_iterator it = allValue.begin();

        for ( ; it != allValue.end(); it++ )
        {
            std::cout << it->first << ":" << it->second << std::endl;
        }
        //------------------------test incrby-------------------------------------------
        uint64_t incrybyNum = redis.hincrby( "testHash", "num", 20 );
        std::cout << "incrybyNum: " << incrybyNum << std::endl;
        //------------------------test incrbyfloat-------------------------------------
        float floatNum = redis.hincrbyfloat( "testHast", "float", 10.1e2 );
        std::cout << "floatNum: " << floatNum << std::endl;
        //------------------------test hkeys-------------------------------------------
      //  CRedisClient::VecString hkeysValue;
      //  uint64_t hkeysNum = redis.hkeys( "testHash", hkeysValue );
      //  std::cout << "hkeysNum: " << hkeysNum << std::endl;

      //  CRedisClient::VecString::const_iterator hkeysit = hkeysValue.begin();
      //  for ( ; hkeysit != hkeysValue.end(); hkeysit++ )
      //  {
      //      std::cout << *hkeysit << std::endl;
      //  }
        //------------------------test hlen-----------------------------------------------
        uint64_t fieldNum = redis.hlen( "testHash" );
        std::cout << "fieldNum: " << fieldNum << std::endl;
        //------------------------test hmget---------------------------------------------
        CResult result;
        CRedisClient::VecString hmgeFields;
        hmgeFields.push_back("name4");
        hmgeFields.push_back("yuhaiyang");
        hmgeFields.push_back("num");
        redis.hmget( "testHash", hmgeFields,result );
        std::cout << "hmget:" << std::endl;
        std::cout << result << std::endl;
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
   //     CRedisClient::MapString pairs;
   //     string pair, value;
   //     std::stringstream ss;
   //     for ( int i = 0; i < 1000; i++ )
   //     {
   //         pair = "pair_";
   //         value = "value_";
   //         ss.str("");

   //         ss << i;
   //         pair += ss.str();
   //         value += ss.str();
   //         pairs.insert( CRedisClient::MapString::value_type(pair,value) );
   //     }
   //     redis.hmset( "testHash", pairs );
        //-------------------------------test hsetnx---------------------------
 //       bool hsetNxRet = redis.hsetnx( "testHash", "num3", "123" );
 //       if ( hsetNxRet )
 //          std::cout << "ok" << std::endl;
 //       else
 //           std::cout << "false" << std::endl;
        //------------------------------test hvals--------------------------------
   //     CRedisClient::VecString hvals;
   //     uint64_t num = redis.hvals( "testHash", hvals );
   //     std::cout << "num: " << num << std::endl;
   //     CRedisClient::VecString::const_iterator it = hvals.begin();
   //     CRedisClient::VecString::const_iterator end = hvals.end();
   //     for ( ; it != end; ++it )
   //     {
   //         std::cout << *it << std::endl;
   //     }

        //-------------------------------test hscan-----------------------------

        std::cout << "====================hscan value===================" << std::endl;
        CRedisClient::VecString hscanValues;

       redis.hscan( "testHash", 0, hscanValues );
       while ( redis.hscan( "testHash", -2, hscanValues ) );

    CRedisClient::VecString::const_iterator hscanIt = hscanValues.begin();
    CRedisClient::VecString::const_iterator hscanEnd = hscanValues.end();

    for ( ; hscanIt != hscanEnd ; ++hscanIt )
    {
        std::cout << *hscanIt << std::endl;
    }
    std::cout << "totalNum: " << hscanValues.size() << std::endl;


    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

///////////////////////////////////////////// test set //////////////////////////////////////////
void TestSet( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );

        string value;
        CResult result;

        //--------------------------test sadd-------------------------------
        CRedisClient::VecString members;
        members.push_back( "yuhaiyang" );
        members.push_back( "zhouli" );
        members.push_back( "严兴俊" );
        uint64_t saddNum = redis.sadd( "testSet", members );
        std::cout << "saddNum: " << saddNum << std::endl;
        //-------------------------test scard--------------------------------
        uint64_t scardNum = redis.scard( "testSet" );
        std::cout << "scardNum:" << scardNum << std::endl;
        //-------------------------test sdiff----------------------------------
       // CRedisClient::VecString keys;
       // keys.push_back("testSet");
       // keys.push_back("testSet2");
       // CRedisClient::VecString sdiffValues;
       // uint64_t sdiffNum = redis.sdiff( keys, sdiffValues );
       // std::cout << "sdiffNum: " << sdiffNum << std::endl;
       // CRedisClient::VecString::const_iterator it = sdiffValues.begin();
       // CRedisClient::VecString::const_iterator end = sdiffValues.end();
       //
       // for ( ; it != end ; ++it )
       // {
       //     std::cout << "sdiffData: " <<*it << std::endl;
       // }
        //-------------------------test sdiffsore----------------------------
        CRedisClient::VecString sdiffstorekeys;
        sdiffstorekeys.push_back("testSet");
        sdiffstorekeys.push_back("testSet2");
        uint64_t sdiffstoreNum = redis.sdiffstore( "diffSetKey",sdiffstorekeys );
        std::cout << "sdiffstoreNum: " << sdiffstoreNum << std::endl;

    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

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
//
//   // result.clear();
//
//    std::cout << result<< std::endl;
//    std::cout << result2<< std::endl;
//}


/*
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


/*
//////////////////////////////////////// test transaction////////////////////////////////////////
int main()
{
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    CResult result;
    redis.multi();
    CRedisClient::VecString params;
    params.push_back( "*" );
    redis.transactionCmd("keys", params );
    params.clear();
    params.push_back("asd");
    redis.transactionCmd("get", params );
    redis.exec( result );

    std::cout << result << std::endl;
}
*/


int main()
{
    TestHash();
   // TestHash2();
   // TestList();
    //TestKey();
   // TestString();
   // TestSet();
    return 0;
}







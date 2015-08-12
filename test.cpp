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


        std::cout << "======testString======" << std::endl;


//        std::cout << "------testAPPEND------" << std::endl;
//        uint16_t iRet = redis.append("myphone", "nokia");
//        std::cout << "iRet: " << iRet << std::endl;
//        std::string value;
//        bool bRet = redis.get("myphone", value);
//        std::cout << "bRet: " << bRet << std::endl;
//        std::cout << "value:" << value << std::endl;



//	std::cout << "------testBITCOUNT------" << std::endl;
//	uint64_t iRet = redis.bitcount("bits", 0, -1);
//	std::cout << "iRet: " << iRet << std::endl;
//	iRet = redis.setbit("bits", 0, "1");
//	std::cout << "iRet: " << iRet << std::endl;
//	iRet = redis.bitcount("bits");
//	std::cout << "iRet: " << iRet << std::endl;



//        std::cout << "------testBITOP------" << std::endl;
//        //bits-1 = 1001
//        redis.setbit("bits-1", 0, "1");
//        redis.setbit("bits-1", 3, "1");
//        //bits-2 = 1011
//        redis.setbit("bits-2", 0, "1");
//        redis.setbit("bits-2", 1, "1");
//        redis.setbit("bits-2", 3, "1");
//
//        CRedisClient::VecString keys;
//        keys.push_back("bits-1");
//        keys.push_back("bits-2");
//        uint64_t iRet = redis.bitop("AND", "and-result", keys);
//        std::cout << "iRet: " << iRet << std::endl;
//
//        iRet = redis.getbit("and-result", 0);
//        std::cout << "iRet: " << iRet << std::endl;
//	iRet = redis.getbit("and-result", 1);
//	std::cout << "iRet: " << iRet << std::endl;
//	iRet = redis.getbit("and-result", 2);
//	std::cout << "iRet: " << iRet << std::endl;
//	iRet = redis.getbit("and-result", 3);
//	std::cout << "iRet: " << iRet << std::endl;



//	std::cout << "------testDECR------" << std::endl;
//	redis._set("failure_times", "10");
//	int64_t iRet = redis.decr("failure_times");
//	std::cout << "iRet: " << iRet << std::endl;





//	std::cout << "------testDECRBY------" << std::endl;
//	redis._set("count", "100");
//	int64_t iRet = redis.decrby("count", 20);
//	std::cout << "iRet: " << iRet << std::endl;



//	std::cout << "------testGET------" << std::endl;
//	redis._set("db", "redis");
//	std::string value;
//	int64_t iRet = redis.get("db", value);
//	std::cout << "iRet: " << iRet << std::endl;
//	std::cout << "value:" << value << std::endl;




//	std::cout << "------testGETBIT------" << std::endl;
//	redis.setbit("bit", 10086, "1");
//	int iRet = redis.getbit("bit", 10086);
//	std::cout << "iRet: " << iRet << std::endl;



//	std::cout << "------testGETRANGE------" << std::endl;
//	redis._set("greeting", "hello, my friend");
//	std::string value;
//	redis.getrange("greeting", 0, 4, value);
//	std::cout << "value:" << value << std::endl;



//	std::cout << "------testGETSET------" << std::endl;
//	std::string oldvalue;
//	bool bRet = redis.getset("db", "mongodb", oldvalue);
//	std::cout << "bRet: " << bRet << std::endl;
//	redis.get("db", oldvalue);
//	std::cout << "oldvalue:" << oldvalue << std::endl;
//	bRet = redis.getset("db", "redis", oldvalue);
//	std::cout << "bRet: " << bRet << std::endl;
//	std::cout << "oldvalue:" << oldvalue << std::endl;
//	redis.get("db", oldvalue);
//	std::cout << "oldvalue:" << oldvalue << std::endl;



//	std::cout << "------testINCR------" << std::endl;
//	redis._set("page_view", "20");
//	int64_t iRet = redis.incr("page_view");
//	std::cout << "iRet: " << iRet << std::endl;
//	std::string value;
//	redis.get("page_view", value);
//	std::cout << "value:" << value << std::endl;




//	std::cout << "------testINCRBY------" << std::endl;
//	redis._set("rank", "50");
//	int64_t iRet = redis.incrby("rank", 20);
//	std::cout << "iRet: " << iRet << std::endl;
//	std::string value;
//	redis.get("rank", value);
//	std::cout << "value:" << value << std::endl;





//	std::cout << "------testINCRBYFLOAT------" << std::endl;
//	redis._set("mykey", "10.50");
//	float fRet = redis.incrbyfloat("mykey", 0.1);
//	std::cout << "fRet: " << fRet << std::endl;
//	std::string value;
//	redis.get("mykey", value);
//	std::cout << "value:" << value << std::endl;


//	std::cout << "------testMGET------" << std::endl;
//	redis._set("redis", "redis.com");
//	redis._set("mongodb", "mongodb.org");
//	CRedisClient::VecString keys;
//	keys.push_back("redis");
//	keys.push_back("mongodb");
//	CResult result;
//	redis.mget(keys, result);
//
//	CResult::ListCResult::const_iterator it = result.getArry().begin();
//	CResult::ListCResult::const_iterator end = result.getArry().end();
//	for ( ; it != end; ++it )
//	{
//		std::cout << "value: " << static_cast<string>(*it) << std::endl;
//	}





//	std::cout << "------testMSET------" << std::endl;
//	CRedisClient::MapString value;
//	value["date"] = "2012.3.30";
//	value["time"] = "11:00 a.m.";
//	value["weather"] = "sunny";
//	redis.mset(value);
//
//	CResult result;
//	CRedisClient::VecString keys;
//	keys.push_back("date");
//	keys.push_back("time");
//	keys.push_back("weather");
//	redis.mget(keys, result);
//	CResult::ListCResult::const_iterator it = result.getArry().begin();
//	CResult::ListCResult::const_iterator end = result.getArry().end();
//	for ( ; it != end; ++it )
//	{
//		std::cout << "value: " << static_cast<string>(*it) << std::endl;
//	}




//	std::cout << "------testMSETNX------" << std::endl;
//	CRedisClient::MapString value;
//	value["rmdbs"] = "MySQL";
//	value["nosql"] = "MongoDB";
//	value["key-value-store"] = "redis";
//	int iRet = redis.msetnx(value);
//	std::cout << "iRet: " << iRet << std::endl;
//
//	CResult result;
//	CRedisClient::VecString keys;
//	keys.push_back("rmdbs");
//	keys.push_back("nosql");
//	keys.push_back("key-value-store");
//	redis.mget(keys, result);
//	CResult::ListCResult::const_iterator it = result.getArry().begin();
//	CResult::ListCResult::const_iterator end = result.getArry().end();
//	for ( ; it != end; ++it )
//	{
//		std::cout << "value: " << static_cast<string>(*it) << std::endl;
//	}





//	std::cout << "------testPSETEX------" << std::endl;
//	bool bRet = redis.setPX("mykey", "Hello", 1000);
//	std::cout << "bRet: " << bRet << std::endl;
//	std::string value;
//	redis.get("mykey", value);
//	std::cout << "value:" << value << std::endl;




//	std::cout << "------testPSET------" << std::endl;
//	CResult result;
//	redis._set("key-with-expire-and-NX", "hello", result, "EX", 10086, "NX");
//	std::string status = result.getStatus();
//	std::cout << "status:" << status << std::endl;
//	std::string value;
//	redis.get("key-with-expire-and-NX", value);
//	std::cout << "value:" << value << std::endl;


//	std::cout << "------testSETBIT------" << std::endl;
//	int iRet = redis.setbit("bit", 10086, "1");
//	std::cout << "iRet: " << iRet << std::endl;
//	iRet = redis.getbit("bit", 10086);
//	std::cout << "iRet: " << iRet << std::endl;



//	std::cout << "------testSETEX------" << std::endl;
//	bool bRet = redis.setEX("cache_user_id", "10086", 60);
//	std::cout << "bRet: " << bRet << std::endl;
//	std::string value;
//	redis.get("cache_user_id", value);
//	std::cout << "value:" << value << std::endl;


//	std::cout << "------testSETNX------" << std::endl;
//	bool bRet = redis.setNX("job", "programmer");
//	std::cout << "bRet: " << bRet << std::endl;
//	std::string value;
//	redis.get("job", value);
//	std::cout << "value:" << value << std::endl;
//
//	bRet = redis.setXX("job", "code-farmer");
//	std::cout << "bRet: " << bRet << std::endl;
//	redis.get("job", value);
//	std::cout << "value:" << value << std::endl;




//	std::cout << "------testSETRANGE------" << std::endl;
//	redis._set("greeting", "hello world");
//	uint64_t iRet = redis.setrange("greeting", 6, "Redis");
//	std::cout << "iRet: " << iRet << std::endl;
//	std::string value;
//	redis.get("greeting", value);
//	std::cout << "value:" << value << std::endl;


//	std::cout << "------testSTRLEN------" << std::endl;
//	redis._set("mykey", "Hello world");
//	uint64_t iRet = redis.strlen("mykey");
//	std::cout << "iRet: " << iRet << std::endl;





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



void TestPSub( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );


        std::cout << "======testPSub======" << std::endl;

//	std::cout << "------testPUBLISH------" << std::endl;
//	uint64_t iRet = redis.publish("msg", "good morning");
//	std::cout << "iRet: " << iRet << std::endl;


//	std::cout << "------testPUBSUB CHANNELS------" << std::endl;
//	CRedisClient::VecString pattern, value;
//	redis.psubchannels(pattern, value);
//	CRedisClient::VecString::const_iterator it = value.begin();
//	CRedisClient::VecString::const_iterator end = value.end();
//	for ( ; it != end; ++it )
//	{
//		std::cout << "value: " << static_cast<string>(*it) << std::endl;
//	}



//	std::cout << "------testPUBSUB NUMSUB------" << std::endl;
//	CRedisClient::VecString channel;
//	channel.push_back("news.it");
//	channel.push_back("news.internet");
//	channel.push_back("news.sport");
//	channel.push_back("news.music");
//	CRedisClient::MapString value;
//	redis.psubnumsub(channel, value);
//	CRedisClient::MapString::const_iterator it = value.begin();
//	CRedisClient::MapString::const_iterator end = value.end();
//	for ( ; it != end; ++it )
//	{
//		std::cout << "value first: " << static_cast<string>(it->first) << std::endl;
//		std::cout << "value second: " << static_cast<string>(it->second) << std::endl;
//	}




//	std::cout << "------testPUBSUB NUMPAT------" << std::endl;
//	uint64_t iRet = redis.psubnumpat();
//	std::cout << "iRet: " << iRet << std::endl;



//	std::cout << "------testPUNSUBSCRIBE------" << std::endl;
//	CRedisClient::VecString pattern;
//	CResult result;
//	redis.punsubscribe(pattern, result);



//	std::cout << "------testUNSUBSCRIBE------" << std::endl;
//	CRedisClient::VecString channel;
//	CResult result;
//	redis.unsubscribe(channel, result);








    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}





int main()
{
    //TestHash();
    //TestHash2();
   // TestList();
    //TestKey();
    //TestString();
   // TestSet();
      TestPSub();
    return 0;
}







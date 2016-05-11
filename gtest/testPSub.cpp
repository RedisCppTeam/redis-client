/**
 * @file	testPSub.cpp
 * @brief test the module of PSub
 *
 * @author: 		yp
 * @date: 		Aug 6, 2015
 *
 */
#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "ExceptRedis.h"
#include "CResult.h"

using namespace std;
using namespace Taiji::Redis;



void TestPSUBSCRIBE( CRedisClient& redis )
{
	std::cout << "------testPSUBSCRIBE------" << std::endl;
	CRedisClient::VecString pattern;
	pattern.push_back("news.*");
	pattern.push_back("tweet.*");
	CResult result;
	redis.psubscribe(pattern, result);
}


void TestPUBLISH( CRedisClient& redis )
{
	std::cout << "------testPUBLISH------" << std::endl;
	uint64_t iRet = redis.publish("msg", "good morning");
	std::cout << "iRet: " << iRet << std::endl;
}


void TestPUBSUBCHANNELS( CRedisClient& redis )
{
	std::cout << "------testPUBSUB CHANNELS------" << std::endl;
	CRedisClient::VecString value;
	uint64_t iRet = redis.psubchannels(value);
	std::cout << "iRet: " << iRet << std::endl;
	CRedisClient::VecString::const_iterator it = value.begin();
	CRedisClient::VecString::const_iterator end = value.end();
	for ( ; it != end; ++it )
	{
		std::cout << "value: " << static_cast<string>(*it) << std::endl;
	}
}


void TestPUBSUBNUMSUB( CRedisClient& redis )
{
	std::cout << "------testPUBSUB NUMSUB------" << std::endl;
	CRedisClient::VecString channel;
	channel.push_back("news.it");
	channel.push_back("news.internet");
	channel.push_back("news.sport");
	channel.push_back("news.music");
	CRedisClient::TupleString value;
	uint64_t iRet = redis.psubnumsub(value, channel);
	std::cout << "iRet: " << iRet << std::endl;
	CRedisClient::TupleString::const_iterator it = value.begin();
	CRedisClient::TupleString::const_iterator end = value.end();
	for ( ; it != end; ++it )
	{
        std::cout << "value first: " << static_cast<string>(std::get<0>(*it)) << std::endl;
        std::cout << "value second: " << static_cast<string>(std::get<1>(*it)) << std::endl;

	}
}


void TestPUBSUBNUMPAT( CRedisClient& redis )
{
	std::cout << "------testPUBSUB NUMPAT------" << std::endl;
	uint64_t iRet = redis.psubnumpat();
	std::cout << "iRet: " << iRet << std::endl;
}


void TestPUNSUBSCRIBE( CRedisClient& redis )
{
	std::cout << "------testPUNSUBSCRIBE------" << std::endl;
	CResult result;
	redis.punsubscribe(result);
}



void TestSUBSCRIBE( CRedisClient& redis )
{
	std::cout << "------testSUBSCRIBE------" << std::endl;
	CRedisClient::VecString channel;
	channel.push_back("msg");
	channel.push_back("chat_room");
	CResult result;
	redis.subscribe(channel, result);
}





void TestUNSUBSCRIBE( CRedisClient& redis )
{
	std::cout << "------testUNSUBSCRIBE------" << std::endl;
	CResult result;
	redis.unsubscribe( result );
}



void TestPSubMain( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );


        std::cout << "======testPSub======" << std::endl;


        TestPUBLISH( redis );
        TestPUBSUBCHANNELS( redis );
        //TestPSUBSCRIBE( redis );
        TestPUBLISH( redis );
        TestPUBSUBCHANNELS( redis );
        TestPUBSUBNUMSUB( redis );
        TestPUBSUBNUMPAT( redis );
        TestPUNSUBSCRIBE( redis );
        //TestSUBSCRIBE( redis );
        TestUNSUBSCRIBE( redis );


    }catch( ExceptRedis& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}





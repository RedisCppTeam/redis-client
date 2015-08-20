/**
 * @file	testString.cpp
 * @brief test the module of String
 *
 * @author: 		yp
 * @date: 		June 14, 2015
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




void TestAPPEND( CRedisClient& redis )
{
	std::cout << "------testAPPEND------" << std::endl;
	uint16_t iRet = redis.append("myphone", "nokia");
	std::cout << "iRet: " << iRet << std::endl;
	std::string value;
	bool bRet = redis.get("myphone", value);
	std::cout << "bRet: " << bRet << std::endl;
	std::cout << "value:" << value << std::endl;
}

void TestBITCOUNT( CRedisClient& redis )
{
	std::cout << "------testBITCOUNT------" << std::endl;
	uint64_t iRet = redis.bitcount("bits", 0, -1);
	std::cout << "iRet: " << iRet << std::endl;
	iRet = redis.setbit("bits", 0, "1");
	std::cout << "iRet: " << iRet << std::endl;
	iRet = redis.bitcount("bits");
	std::cout << "iRet: " << iRet << std::endl;
}

void TestBITOP( CRedisClient& redis )
{
	std::cout << "------testBITOP------" << std::endl;
	//bits-1 = 1001
	redis.setbit("bits-1", 0, "1");
	redis.setbit("bits-1", 3, "1");
	//bits-2 = 1011
	redis.setbit("bits-2", 0, "1");
	redis.setbit("bits-2", 1, "1");
	redis.setbit("bits-2", 3, "1");

	CRedisClient::VecString keys;
	keys.push_back("bits-1");
	keys.push_back("bits-2");
	uint64_t iRet = redis.bitop("AND", "and-result", keys);
	std::cout << "iRet: " << iRet << std::endl;

	iRet = redis.getbit("and-result", 0);
	std::cout << "iRet: " << iRet << std::endl;
	iRet = redis.getbit("and-result", 1);
	std::cout << "iRet: " << iRet << std::endl;
	iRet = redis.getbit("and-result", 2);
	std::cout << "iRet: " << iRet << std::endl;
	iRet = redis.getbit("and-result", 3);
	std::cout << "iRet: " << iRet << std::endl;
}


void TestDECR( CRedisClient& redis )
{
	std::cout << "------testDECR------" << std::endl;
	redis.set("failure_times", "10");
	int64_t iRet = redis.decr("failure_times");
	std::cout << "iRet: " << iRet << std::endl;
}

void TestDECRBY( CRedisClient& redis )
{
	std::cout << "------testDECRBY------" << std::endl;
	redis.set("count", "100");
	int64_t iRet = redis.decrby("count", 20);
	std::cout << "iRet: " << iRet << std::endl;
}

void TestGET( CRedisClient& redis )
{
	std::cout << "------testGET------" << std::endl;
	redis.set("db", "redis");
	std::string value;
	int64_t iRet = redis.get("db", value);
	std::cout << "iRet: " << iRet << std::endl;
	std::cout << "value:" << value << std::endl;
}


void TestGETBIT( CRedisClient& redis )
{
	std::cout << "------testGETBIT------" << std::endl;
	redis.setbit("bit", 10086, "1");
	int iRet = redis.getbit("bit", 10086);
	std::cout << "iRet: " << iRet << std::endl;
}

void TestGETRANGE( CRedisClient& redis )
{
	std::cout << "------testGETRANGE------" << std::endl;
	redis.set("greeting", "hello, my friend");
	std::string value;
	redis.getrange("greeting", 0, 4, value);
	std::cout << "value:" << value << std::endl;
}


void TestGETSET( CRedisClient& redis )
{
	std::cout << "------testGETSET------" << std::endl;
	std::string oldvalue;
	bool bRet = redis.getset("db", "mongodb", oldvalue);
	std::cout << "bRet: " << bRet << std::endl;
	redis.get("db", oldvalue);
	std::cout << "oldvalue:" << oldvalue << std::endl;
	bRet = redis.getset("db", "redis", oldvalue);
	std::cout << "bRet: " << bRet << std::endl;
	std::cout << "oldvalue:" << oldvalue << std::endl;
	redis.get("db", oldvalue);
	std::cout << "oldvalue:" << oldvalue << std::endl;
}

void TestINCR( CRedisClient& redis )
{
	std::cout << "------testINCR------" << std::endl;
	redis.set("page_view", "20");
	int64_t iRet = redis.incr("page_view");
	std::cout << "iRet: " << iRet << std::endl;
	std::string value;
	redis.get("page_view", value);
	std::cout << "value:" << value << std::endl;
}

void TestINCRBY( CRedisClient& redis )
{
	std::cout << "------testINCRBY------" << std::endl;
	redis.set("rank", "50");
	int64_t iRet = redis.incrby("rank", 20);
	std::cout << "iRet: " << iRet << std::endl;
	std::string value;
	redis.get("rank", value);
	std::cout << "value:" << value << std::endl;
}


void TestINCRBYFLOAT( CRedisClient& redis )
{
	std::cout << "------testINCRBYFLOAT------" << std::endl;
	redis.set("mykey", "10.50");
	float fValue;
	bool bRet = redis.incrbyfloat("mykey", 0.1, fValue);
	std::cout << "bRet: " << bRet << std::endl;
	std::cout << "fValue: " << fValue << std::endl;
	std::string value;
	redis.get("mykey", value);
	std::cout << "value:" << value << std::endl;
}



void TestMGET( CRedisClient& redis )
{
	std::cout << "------testMGET------" << std::endl;
	redis.set("redis", "redis.com");
	redis.set("mongodb", "mongodb.org");
	CRedisClient::VecString keys;
	keys.push_back("redis");
	keys.push_back("mongodb");
	CResult result;
	redis.mget(keys, result);

	CResult::ListCResult::const_iterator it = result.getArry().begin();
	CResult::ListCResult::const_iterator end = result.getArry().end();
	for ( ; it != end; ++it )
	{
		std::cout << "value: " << static_cast<string>(*it) << std::endl;
	}
}


void TestMSET( CRedisClient& redis )
{
	std::cout << "------testMSET------" << std::endl;
	CRedisClient::MapString value;
	value["date"] = "2012.3.30";
	value["time"] = "11:00 a.m.";
	value["weather"] = "sunny";
	redis.mset(value);

	CResult result;
	CRedisClient::VecString keys;
	keys.push_back("date");
	keys.push_back("time");
	keys.push_back("weather");
	redis.mget(keys, result);
	CResult::ListCResult::const_iterator it = result.getArry().begin();
	CResult::ListCResult::const_iterator end = result.getArry().end();
	for ( ; it != end; ++it )
	{
		std::cout << "value: " << static_cast<string>(*it) << std::endl;
	}
}


void TestMSETNX( CRedisClient& redis )
{
	std::cout << "------testMSETNX------" << std::endl;
	CRedisClient::MapString value;
	value["rmdbs"] = "MySQL";
	value["nosql"] = "MongoDB";
	value["key-value-store"] = "redis";
	int iRet = redis.msetnx(value);
	std::cout << "iRet: " << iRet << std::endl;

	CResult result;
	CRedisClient::VecString keys;
	keys.push_back("rmdbs");
	keys.push_back("nosql");
	keys.push_back("key-value-store");
	redis.mget(keys, result);
	CResult::ListCResult::const_iterator it = result.getArry().begin();
	CResult::ListCResult::const_iterator end = result.getArry().end();
	for ( ; it != end; ++it )
	{
		std::cout << "value: " << static_cast<string>(*it) << std::endl;
	}
}


void TestPSETEX( CRedisClient& redis )
{
	std::cout << "------testPSETEX------" << std::endl;
	bool bRet = redis.setPX("mykey", "Hello", 1000);
	std::cout << "bRet: " << bRet << std::endl;
	std::string value;
	redis.get("mykey", value);
	std::cout << "value:" << value << std::endl;
}


void TestSETBIT( CRedisClient& redis )
{
	std::cout << "------testSETBIT------" << std::endl;
	int iRet = redis.setbit("bit", 10086, "1");
	std::cout << "iRet: " << iRet << std::endl;
	iRet = redis.getbit("bit", 10086);
	std::cout << "iRet: " << iRet << std::endl;
}


void TestSETEX( CRedisClient& redis )
{
	std::cout << "------testSETEX------" << std::endl;
	bool bRet = redis.setEX("cache_user_id", "10086", 60);
	std::cout << "bRet: " << bRet << std::endl;
	std::string value;
	redis.get("cache_user_id", value);
	std::cout << "value:" << value << std::endl;
}


void TestSETNX( CRedisClient& redis )
{
	std::cout << "------testSETNX------" << std::endl;
	bool bRet = redis.setNX("job", "programmer");
	std::cout << "bRet: " << bRet << std::endl;
	std::string value;
	redis.get("job", value);
	std::cout << "value:" << value << std::endl;



	bRet = redis.setXX("job", "code-farmer");
	std::cout << "bRet: " << bRet << std::endl;
	redis.get("job", value);
	std::cout << "value:" << value << std::endl;
}

void TestSETRANGE( CRedisClient& redis )
{
	std::cout << "------testSETRANGE------" << std::endl;
	redis.set("greeting", "hello world");
	uint64_t iRet = redis.setrange("greeting", 6, "Redis");
	std::cout << "iRet: " << iRet << std::endl;
	std::string value;
	redis.get("greeting", value);
	std::cout << "value:" << value << std::endl;
}


void TestSTRLEN( CRedisClient& redis )
{
	std::cout << "------testSTRLEN------" << std::endl;
	redis.set("mykey", "Hello world");
	uint64_t iRet = redis.strlen("mykey");
	std::cout << "iRet: " << iRet << std::endl;
}




void TestString( void )
{
    try
    {
        std::cout << "======testString======" << std::endl;
	CRedisClient redis;
	redis.connect( "127.0.0.1", 6379 );

	TestAPPEND( redis );
	TestBITCOUNT( redis );
	TestBITOP( redis );
	TestDECR( redis );
	TestDECRBY( redis );
	TestGET( redis );
	TestGETBIT( redis );
	TestGETRANGE( redis );
	TestGETSET( redis );
	TestINCR( redis );
	TestINCRBY( redis );
	TestINCRBYFLOAT( redis );
	TestMGET( redis );
	TestMSET( redis );
	TestMSETNX( redis );
	TestPSETEX( redis );
	TestSETBIT( redis );
	TestSETEX( redis );
	TestSETNX( redis );
	TestSETRANGE( redis );
	TestSTRLEN( redis );

    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

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
#include "RdException.h"
#include "CResult.h"

using namespace std;
using namespace Taiji::Redis;

void TestPing( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		string value;
        redis.ping(value);
    } catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestQuit( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

        redis.quit();
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestEcho( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		string value;
        redis.echo("hello world", value);
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestAuth( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

        redis.auth("1234");
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestSelect( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

        redis.select(1);
    } catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestConnectionMain( void )
{
//	TestPing();
	TestQuit();
//	TestEcho();
//	TestAuth();
//	TestSelect();
    return;
}


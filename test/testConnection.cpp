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

void TestPing( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		string value;
		bool flag = redis.ping(value);
		cout << flag << endl;
		cout << value << endl;
	} catch( RdException& e )
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

		bool flag = redis.quit();
		cout << flag << endl;
	} catch( RdException& e )
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
		bool flag = redis.echo("hello world", value);
		cout << flag << endl;
		cout << value << endl;
	} catch( RdException& e )
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

		string value;
		bool flag = redis.auth("1234");
		cout << flag << endl;
	} catch( RdException& e )
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

		string value;
		bool flag = redis.select(1);
		cout << flag << endl;
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void Cmain( void )
{
	TestPing();
	TestQuit();
	TestEcho();
	TestAuth();
	TestSelect();
    return;
}


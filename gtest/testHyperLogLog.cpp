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

void TestPfadd( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		CRedisClient::VecString element;
		element.push_back("a");
		element.push_back("b");
		element.push_back("c");
		string key("key1");
		uint64_t count = redis.pfadd(key, element);
		cout << count << endl;
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestPfcount( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		CRedisClient::VecString element;
		element.push_back("a");
		element.push_back("b");
		element.push_back("c");
		string key1("key1");
		redis.pfadd(key1, element);

		CRedisClient::VecString element2;
		element2.push_back("a");
		element2.push_back("d");
		string key2("key2");
		redis.pfadd(key2, element2);

		CRedisClient::VecString keys;
		keys.push_back(key1);
		keys.push_back(key2);
		uint64_t num = redis.pfcount(keys);
		cout << num << endl;
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestPfmerge( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		CRedisClient::VecString element;
		element.push_back("a");
		element.push_back("b");
		element.push_back("c");
		string key1("key1");
		redis.pfadd(key1, element);

		CRedisClient::VecString element2;
		element2.push_back("a");
		element2.push_back("d");
		string key2("key2");
		redis.pfadd(key2, element2);

		CRedisClient::VecString keys;
		keys.push_back(key1);
		keys.push_back(key2);
		bool flag = redis.pfmerge("test", keys);
		cout << flag << endl;
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void Pmain( void )
{
	TestPfadd();
	TestPfcount();
	TestPfmerge();
    return;
}


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
#include "ExceptRedis.h"
#include "CResult.h"

using namespace std;
using namespace Taiji::Redis;
void TestLpush( void )
{
	try
	{

		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "key2";

		CRedisClient::VecString value;
		value.push_back("a");
		value.push_back("f");
		value.push_back("c");
		int count = redis.lpush(mykey, value);
		std::cout << count << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestRpush( void )
{
	try
	{

		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
		string mykey = "key";

		CRedisClient::VecString value;
    //	value.push_back("e");
    //	value.push_back("g");
    //	value.push_back("j");
		int count = redis.rpush(mykey, value);
		std::cout << count << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLpop( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "key";

		std::string value;
        if ( !redis.lpop(mykey, value) )
        {
            std::cout << "lpop failed!" << std::endl;
        }else
        {
            std::cout << "lpop successful!" << std::endl;
            std::cout << value << std::endl;
        }

    } catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestRpop( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
		string mykey = "key";

		std::string value;
        if ( !redis.rpop(mykey, value) )
        {
            std::cout << "rpop failed!!" << std::endl;
        }else
        {
            std::cout << "rpop successful!!" << std::endl;
            std::cout << value << std::endl;
        }
    } catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLindex( void )
{
	try
	{

		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testList";

        std::string value;

        if ( !redis.lindex(mykey, 3, value) )
        {
            std::cout << "lindex failed!!" << std::endl;
        }else
        {
            std::cout << value << std::endl;
        }
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLlen( void )
{
	try
	{

		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testHash";

		std::string value;
		int64_t count = redis.llen(mykey);
		std::cout << count << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLpushx( void )
{
	try
	{

		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testList";
		string value = "x";

		int64_t count = redis.lpushx(mykey, value);
		std::cout << count << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestRpushx( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testHash";
		string value = "y";

		int64_t count = redis.rpushx(mykey, value);
		std::cout << count << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLinsert( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testList";

        int64_t count = redis.linsert(mykey, CRedisClient::BEFORE, "zhongwei", "hello");
		std::cout << count << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLrem( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

        uint64_t num = redis.lrem("testList","yuhaiyang");
        std::cout << "num: " << num << std::endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLtrim( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testList";

        redis.ltrim(mykey, 0, -2);
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLset( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
        string mykey = "testList";

        redis.lset(mykey, 10, "haha");
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestLrange( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

        CRedisClient::VecString value;
        uint64_t count = redis.lrange("testList", 1, -2, value);
		std::cout << count << std::endl;
		CRedisClient::VecString::const_iterator it = value.begin();
		CRedisClient::VecString::const_iterator end = value.end();
		while ( it != end )
		{
			cout << *it << endl;
			++it;
		}
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestRpoplpush( )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

        string value;
        bool flag = redis.rpoplpush("source", "dest", value);
		cout << flag << endl;
        cout << "value:" << value << endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestBlpop( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		string key = "key";
		string key2 = "key2";
		CRedisClient::VecString keyValue;
		keyValue.push_back("a");
		keyValue.push_back("b");
		keyValue.push_back("c");
		CRedisClient::VecString key2Value;
		key2Value.push_back("1");
		key2Value.push_back("2");
		key2Value.push_back("3");

    //	redis.rpush(key, keyValue);
    //	redis.rpush(key2, key2Value);

		CRedisClient::VecString keys;
		keys.push_back("key");
		keys.push_back("key2");

		CRedisClient::VecTuple value;
        uint64_t timeout = 2;

        if ( !redis.blpop(keys, timeout, value) )
        {
            std::cout << "blpop failed" << std::endl;
        }else
        {
            CRedisClient::VecTuple::const_iterator it = value.begin();
            CRedisClient::VecTuple::const_iterator end = value.end();
            while ( it != end )
            {
                std::cout << std::get<0>(*it) << ":" <<std::get<1>(*it) << std::endl;
                ++it;
            }
        }
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestBrpop( void )
{
	try
	{
		CRedisClient redis;
		redis.setTimeout(0,0);
		redis.connect("127.0.0.1", 6379);

		string key = "key";
		string key2 = "key2";
		CRedisClient::VecString keyValue;
		keyValue.push_back("a");
		keyValue.push_back("b");
		keyValue.push_back("c");
		CRedisClient::VecString key2Value;
		key2Value.push_back("1");
		key2Value.push_back("2");
		key2Value.push_back("3");

        redis.rpush(key, keyValue);
        redis.rpush(key2, key2Value);

		CRedisClient::VecString keys;
		keys.push_back("key");
		keys.push_back("key2");

		CRedisClient::VecTuple value;
		uint64_t timeout = 3;
        if ( !redis.brpop(keys, timeout, value) )
        {
            std::cout << "brpop falied" << std::endl;
            return;
        }

		CRedisClient::VecTuple::const_iterator it = value.begin();
		CRedisClient::VecTuple::const_iterator end = value.end();
		while ( it != end )
		{
            std::cout << std::get<0>(*it) << ":" <<std::get<1>(*it) << std::endl;
			++it;
		}
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestBrpoplpush( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);

		string value;
        bool flag=redis.brpoplpush("source", "dest", 1, value);
		cout<<flag<<endl;
		cout<<value<<endl;
	} catch( ExceptRedis& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void TestListMain( void )
{
 // TestLpush();
 // TestRpush();
 // TestLpop();
//  TestRpop();
 // TestLindex();
 // TestLlen();
 // TestLpushx();
//	TestRpushx();
  TestLinsert();
//  TestLrem();
//  TestLtrim();
//  TestLset();
//  TestLrange();
//  TestRpoplpush();
//  TestBlpop();
//  TestBrpop();
//  TestBrpoplpush();
    return;
}


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

void TestLpush( void )
{
	try
	{

		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
		string mykey = "key";

		CRedisClient::VecString value;
		value.push_back("a");
		value.push_back("f");
		value.push_back("c");
		int count = redis.lpush(mykey, value);
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		value.push_back("e");
		value.push_back("g");
		value.push_back("j");
		int count = redis.rpush(mykey, value);
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		bool flag = redis.lpop(mykey, value);
		std::cout << flag << std::endl;
		std::cout << value << std::endl;
	} catch( RdException& e )
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
		bool flag = redis.rpop(mykey, value);
		std::cout << flag << std::endl;
		std::cout << value << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		std::string value;
		bool flag = redis.lindex(mykey, 2, value);
		std::cout << flag << std::endl;
		std::cout << value << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		std::string value;
		int64_t count = redis.llen(mykey);
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";
		string value = "x";

		int64_t count = redis.lpushx(mykey, value);
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";
		string value = "y";

		int64_t count = redis.rpushx(mykey, value);
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		int64_t count = redis.linsert(mykey, "before", "b", "hello");
//		int64_t count=redis.linsert(mykey,"after","b","world");
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		uint64_t num = 3;
		int64_t count = redis.lrem(mykey, num, "hello");
//		int64_t count=redis.linsert(mykey,"after","b","world");
		std::cout << count << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		uint64_t start = 1;
		uint64_t stop = 2;
		bool flag = redis.ltrim(mykey, start, stop);
		std::cout << flag << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		uint64_t index = 1;
		bool flag = redis.lset(mykey, index, "haha");
		std::cout << flag << std::endl;
	} catch( RdException& e )
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
		string mykey = "key";

		CRedisClient::VecString value;
		int64_t start = 0;
		int64_t stop = 33;
		uint64_t count = redis.lrange(mykey, start, stop, value);
		std::cout << count << std::endl;
		CRedisClient::VecString::const_iterator it = value.begin();
		CRedisClient::VecString::const_iterator end = value.end();
		while ( it != end )
		{
			cout << *it << endl;
			++it;
		}
	} catch( RdException& e )
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

		string source = "source";
		string dest = "dest";
//		CRedisClient::VecString srcValue;
//		srcValue.push_back("a");
//		srcValue.push_back("b");
//		srcValue.push_back("c");
//		CRedisClient::VecString destValue;
//		destValue.push_back("1");
//		destValue.push_back("2");
//		destValue.push_back("3");
//
//		redis.rpush(source, srcValue);
//		redis.rpush(dest, destValue);
//		srcValue.clear();
//		destValue.clear();
//
//		cout << "before test rpoplpush......" << endl;
//		cout << "old source.............." << endl;
//		int64_t start = 0;
//		int64_t stop = 2;
//		redis.lrange(source, start, stop, srcValue);
//		for ( CRedisClient::VecString::const_iterator it = srcValue.begin() ; it != srcValue.end() ;
//				it++ )
//		{
//			cout << *it << endl;
//		}
//		redis.lrange(dest, start, stop, destValue);
//		cout << "old dest.........." << endl;
//		for ( CRedisClient::VecString::const_iterator it = destValue.begin() ;
//				it != destValue.end() ; it++ )
//		{
//			cout << *it << endl;
//		}
//		srcValue.clear();
//		destValue.clear();
//
//		cout << "after test rpoplpush........" << endl;
		string value;
		bool flag = redis.rpoplpush(source, dest, value);
		cout << flag << endl;
		cout << "value====================" << value << endl;

//		int64_t start_new = 0;
//		int64_t stop_new = 20;
//		redis.lrange(source, start_new, stop_new, srcValue);
//		redis.lrange(dest, start_new, stop_new, destValue);
//		cout << "new source................" << endl;
//		for ( CRedisClient::VecString::const_iterator it = srcValue.begin() ; it != srcValue.end() ;
//				it++ )
//		{
//			cout << *it << endl;
//		}
//		cout << "new dest..................." << endl;
//		for ( CRedisClient::VecString::const_iterator it = destValue.begin() ;
//				it != destValue.end() ; it++ )
//		{
//			cout << *it << endl;
//		}

	} catch( RdException& e )
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

		redis.rpush(key, keyValue);
		redis.rpush(key2, key2Value);

		CRedisClient::VecString keys;
		keys.push_back("key");
		keys.push_back("key2");

		CRedisClient::MapString value;
		uint64_t timeout = 0;
		redis.blpop(keys, timeout, value);

		CRedisClient::MapString::const_iterator it = value.begin();
		CRedisClient::MapString::const_iterator end = value.end();
		while ( it != end )
		{
			cout << it->first << endl;
			cout << it->second << endl;
			++it;
		}
	} catch( RdException& e )
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

//		redis.rpush(key, keyValue);
//		redis.rpush(key2, key2Value);

		CRedisClient::VecString keys;
		keys.push_back("key");
		keys.push_back("key2");

		CRedisClient::MapString value;
		uint64_t timeout = 3;
		bool flag=redis.brpop(keys, timeout, value);
		cout<<flag<<endl;

		CRedisClient::MapString::const_iterator it = value.begin();
		CRedisClient::MapString::const_iterator end = value.end();
		while ( it != end )
		{
			cout << it->first << endl;
			cout << it->second << endl;
			++it;
		}
	} catch( RdException& e )
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

		string key = "key";
		string key2 = "key2";
//		CRedisClient::VecString keyValue;
//		keyValue.push_back("a");
//		keyValue.push_back("b");
//		keyValue.push_back("c");
//		CRedisClient::VecString key2Value;
//		key2Value.push_back("1");
//		key2Value.push_back("2");
//		key2Value.push_back("3");
//
//		redis.rpush(key, keyValue);
//		redis.rpush(key2, key2Value);

		string value;
		uint64_t timeout = 1;
		bool flag=redis.brpoplpush("key", "key2", timeout, value);
		cout<<flag<<endl;
		cout<<value<<endl;
//		CRedisClient::VecString::const_iterator it=value.begin();
//		CRedisClient::VecString::const_iterator end=value.end();
//		while(it!=end)
//		{
//			cout<<*it<<endl;
//		}
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

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

int main( )
{
	TestLpush();
	TestRpush();
	TestLpop();
	TestRpop();
	TestLindex();
	TestLlen();
	TestLpushx();
	TestRpushx();
	TestLinsert();
	TestLrem();
	TestLtrim();
	TestLset();
	TestLrange();
	TestRpoplpush();
	TestBlpop();
	TestBrpop();
	TestBrpoplpush();
	TestPing();
	TestQuit();
	TestEcho();
	TestAuth();
	TestSelect();
	TestPfadd();
	TestPfcount();
	TestPfmerge();
	return 0;
}


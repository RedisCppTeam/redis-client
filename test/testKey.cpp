/**
 *
 * @file	main.cpp
 * @brief CRedisClient 测试代码。
 *
 * @author: 		Stanley Huang
 * @date: 		2015年8月14日
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

void TestDel( void )
{
	try
	{
		CRedisClient redis;
		CRedisClient::VecString keys;
		redis.connect("127.0.0.1", 6379);
		string val = "{\"a\":\"ok\"\r\n}";
		string mykey = "special";

		//-----------get--------
		if ( redis.get(mykey, val) )
			std::cout << "my val:" << val << std::endl;
		else
			std::cout << "get failed:" << val << std::endl;

		//-----------del--------
		keys.clear();
		keys.push_back("k1");
		keys.push_back("k2");
		uint64_t num = redis.del(keys);
		std::cout << "del num:" << num << std::endl;

	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}
void TestKeys( void )
{
	try
	{
		CRedisClient redis;
		CRedisClient::VecString keys;
		redis.connect("127.0.0.1", 6379);
		string val = "{\"a\":\"ok\"\r\n}";
		string mykey = "special";

		//-----------exists--------
		bool retb = redis.exists(mykey);
		cout << "exists:" << ( retb ? "true" : "false" ) << endl;

		//-----------keys--------
		string pattern = "k*";
		int ret = redis.keys(pattern, keys);
		for ( ret--; ret >= 0 ; ret-- )
		{
			cout << keys[ret] << endl;
		}

	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}
void testTTL( void )
{
	try
	{
		CRedisClient redis;
		CRedisClient::VecString keys;
		redis.connect("127.0.0.1", 6379);
		bool retb;
		string mykey = "myk";
		int ttl = 2;

		//-----------exists--------
		retb = redis.expire(mykey, 5);
		cout << "expire:" << ( retb ? "true" : "false" ) << endl;
		ttl = redis.ttl(mykey);
		while ( ttl > 0 )
		{
			cout << "ttl1:" << ttl << endl << endl;
			ttl = redis.ttl(mykey);
			if ( ttl <= 2 )
			{
				retb = redis.persist(mykey);
				cout << "persist:" << ( retb ? "true" : "false" ) << endl;
			}
			usleep(500 * 1000);
		}
		cout << "ttl1 break*********:" << ttl << endl << endl;
		retb = redis.persist(mykey);
		cout << "persist:" << ( retb ? "true" : "false" ) << endl;

		mykey = "myku";
		retb = redis.expireAt(mykey, 1439268286);
		cout << "expireat:" << ( retb ? "true" : "false" ) << endl;
		ttl = redis.ttl(mykey);
		while ( ttl > 0 )
		{
			cout << "ttlu:" << ttl << endl << endl;
			ttl = redis.ttl(mykey);
			usleep(500 * 1000);
		}
		cout << "ttlu break****:" << ttl << endl << endl;

	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}
void testobj( void )
{
	try
	{
		CRedisClient redis;
		CRedisClient::VecString keys;
		redis.connect("127.0.0.1", 6379);
		//REFCOUNT = 0, ENCODING, IDLETIME
		string mykey = "myhx";
		string ret;

		ret = redis.object(CRedisClient::REFCOUNT, mykey);
		cout << "redis.object REFCOUNT:" << ret << endl;

		ret = redis.object(CRedisClient::ENCODING, mykey);
		cout << "redis.object ENCODING:" << ret << endl;

		ret = redis.object(CRedisClient::IDLETIME, mykey);
		cout << "redis.object IDLETIME:" << ret << endl;

		ret = redis.randomKey();
		cout << "randomKey:" << ret << endl;
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

void testRename( void )
{
	try
	{
		CRedisClient redis;
		redis.connect("127.0.0.1", 6379);
		bool ret;

		ret = redis.rename("mys", "myu");
		if ( ret )
			cout << "redis.rename ok:" << ret << endl;

		else
			cout << "redis.rename failed:" << ret << endl;

		ret = redis.renameNx("myu", "mys");
		if ( ret )
			cout << "redis.nx ok:" << ret << endl;

		else
			cout << "redis.nx failed:" << ret << endl;
	} catch( RdException& e )
	{
		std::cout << "Redis exception:" << e.what() << std::endl;
	}
}
void testSort( void )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	CRedisClient::VecString values;

    redis.sort("today_cost", values);

    for ( int i = values.size() - 1 ; i >= 0 ; i-- )
	{
		cout << i << ":" << values[i] << endl;
	}
}
void testType( void )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;
	string type;

	ret = redis.type("today_cost", type);
	if ( ret )
		cout << "redis.type ok:" << ret << endl;

	else
		cout << "redis.type failed:" << ret << endl;

	cout << "type:" << type << endl;

	ret = redis.type("ki", type);
	if ( ret )
		cout << "redis.type ok:" << ret << endl;

	else
		cout << "redis.type failed:" << ret << endl;

	cout << "type:" << type << endl;
}

void testScan( void )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;
	string type;
	CRedisClient::VecString values;

	ret = redis.scan(values, 0, "k*", 100);
	if ( ret > 0 )
		cout << "redis.scan ok:" << ret << endl;

	else
		cout << "redis.scan failed:" << ret << endl;

	cout << "ret:" << ret << endl;

	for ( int i = values.size() - 1 ; i >= 0 ; i-- )
	{
		cout << i << ":" << values[i] << endl;
	}
}

string testDump( void )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;
	string retStr;

	ret = redis.dump("rrKey", retStr);
	if ( ret )
		cout << "redis.dump ok:" << ret << endl;

	else
		cout << "redis.dump failed:" << ret << endl;

	int n = retStr.length();
	DEBUGOUT("dump", retStr)
	DEBUGOUT("length", n)
	char buf[24] =
	{ 0 };
	memcpy(buf, retStr.c_str(), n);
	for ( int i = 0 ; i < n ; i++ )
	{
		cout << (int) retStr[i] << "-";
	}
	cout << endl;
	return retStr;
}
//
void testRestore( const string& strInput )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;
//	string retStr;
//	char buff[256] =
//	{ 00, 0x01, 0x65, 0x06, 0x00, 0x6c, 0x44, 0x3f, 0xcb, 0x28, 0x4d, 0xe3, 0x8a };
//	string data; // "\x00\x01\x65\x06\x00\x6c\x44\x3f\xcb\x28\x4d\xe3\x8a";
//	data.resize(' ', 13);
//	for ( int i = 0 ; i < 13 ; i++ )
//	{
//		data[i] = buff[i];
//	}
//	data = data.substr(0, 13);

	ret = redis.restore("uuKey", strInput);
	if ( ret )
		cout << "redis.restore ok:" << ret << endl;

	else
		cout << "redis.restore failed:" << ret << endl;

}

void testevalSha( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	CResult result;
	string retString;
    redis.scriptLoad("return 'hello moto'",retString );
    cout << "redis.scriptLoad ok:" << retString << endl;

    redis.evalSha(result, retString);
	cout << "type:" << CResult::getTypeString(result.getType()) << endl;
	cout << "return:" << result << endl;
}

void testScriptExists( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	string retString;
    redis.scriptLoad( "return 'hello moto'",retString );
	cout << "redis.scriptLoad ok:" << retString << endl;

    CRedisClient::VecString result;
    redis.scriptExists(retString,result);
    cout << "script exists ok:" << retString << endl;

    result.clear();
    retString += '2';
    redis.scriptExists(retString,result);
}
void testScriptFlush( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
    redis.scriptFlush();
	cout << "scriptFlush ok!" << endl;
}

//redis-cli:>EVAL "while true do end" 0
void testScriptKill( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
    redis.scriptKill();
	cout << "scriptFlush ok!" << endl;

}
void testMigrate( )
{
	CRedisClient redis;

	redis.connect("127.0.0.1", 6379);
	string retStr;
	if ( !redis.migrate("keyss", "127.0.0.1", 7777) )
	{
		cout << "migrate failed:" << endl;
		return;
	}

	cout << "migrate ok!" << endl;

}

void testMove( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	string key("key");
	int dstDB = 1;
	if ( !redis.move(key, dstDB) )
	{
		cout << "move failed:" << endl;
		return;
	}
	cout << "move ok!" << endl;

}

int kmain( )
{
	try
	{
//		testMove();
//		testMigrate();
//		TestDel();
//		testScriptKill();
//		testScriptFlush();
//		testScriptExists();
//		testevalSha();
//		string dumped = testDump();
//		testRestore(dumped);
//		testScan();
//		testType();
//		testSort();
//		testRename();
//		testobj();
//		testTTL();
//		TestKeys();
	} catch( RdException& e )
	{
		std::cout << endl << "Redis exception !!!!:" << e.what() << std::endl;
		return -1;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}

	cout << "exit main" << endl;
	return 1;
}


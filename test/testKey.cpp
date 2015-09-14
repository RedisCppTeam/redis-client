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

void AddManyKey( void )
{
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );

    string key;
    std::stringstream ss;
    for ( int i = 0; i < 1000; i++ )
     {
         key = "key_";
         ss.str("");

         ss << i;
         key += ss.str();
         redis.set( key, "ok" );
     }
}



void TestDel( void )
{
	try
	{
		CRedisClient redis;
		CRedisClient::VecString keys;
		redis.connect("127.0.0.1", 6379);
        //-----------del--------
		keys.clear();
        keys.push_back("k1");
        keys.push_back("k2");
        uint64_t num = redis.del( keys );
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
    AddManyKey();

	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	CRedisClient::VecString values;

    redis.scan( 0, values, "key_??" );
    while( redis.scan( -1, values,"key_??" ) );

    std::cout << "size:" << values.size() << std::endl;
	for ( int i = values.size() - 1 ; i >= 0 ; i-- )
	{
		cout << i << ":" << values[i] << endl;
	}
}

void testDump( string& dump )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;

    ret = redis.dump("k1", dump );
	if ( ret )
		cout << "redis.dump ok:" << ret << endl;
	else
		cout << "redis.dump failed:" << ret << endl;

    DEBUGOUT("dump", dump )
}
//
void testRestore( const string& strInput )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
    bool ret;
    ret = redis.restore("uuKey", strInput);
	if ( ret )
		cout << "redis.restore ok:" << ret << endl;
	else
		cout << "redis.restore failed:" << ret << endl;

 //   string restoreKey;
 //   redis.get( "uuKey", restoreKey );
 //   std::cout << "data:" << restoreKey << std::endl;
}


void testMigrate( )
{
	CRedisClient redis;

	redis.connect("127.0.0.1", 6379);
    redis.migrate("keyss", "192.168.10.179", 6379 );
}

void testMove( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
    if ( !redis.move("k1", 2) )
	{
		cout << "move failed:" << endl;
		return;
	}
	cout << "move ok!" << endl;
}

void Kmain( void )
{
	try
	{
//      testMove();
//      testMigrate();
//     TestDel();
//        string dumped;
//        testDump( dumped );
//        testRestore(dumped);
        testScan();
//		testType();
//		testSort();
//		testRename();
//		testobj();
//		testTTL();
//		TestKeys();
	} catch( RdException& e )
	{
		std::cout << endl << "Redis exception !!!!:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}


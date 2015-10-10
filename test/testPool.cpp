/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	testPool.cpp		
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 *
 * @author: 		fyt
 * @date: 		2015年10月9日
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
#include "CRedisPool.h"

using namespace std;

void testPoolmain( )
{

	CRedisClient *pRedis1 = NULL;
	CRedisPool redisPool;
	int connNum;
	std::string value;
	//test CRedisClient::init()  ::getConn()   ::pushBackConn()
	try
	{
		redisPool.init("127.0.0.1", 6379, "", 6, 1000, 6);
		for ( int i = 0 ; i < 600 ; i++ )
		{
			sleep(1);
			pRedis1 = redisPool.getConn();
			if ( pRedis1 == NULL )
				return;

			pRedis1->get("two", value);
			std::cout << "getConn():two = " << value << "i = " << i << std::endl;
//		redisPool.pushBackConn(pRedis1);

//test  CRedisClient::getConn( int& )   ::pushBackConn( int& )

			pRedis1 = redisPool.getConn(connNum);
			if ( pRedis1 == NULL )
				return;
			value.clear();
			pRedis1->get("two", value);
			std::cout << "getConn(int&):two = " << value << "ii = " << i << std::endl;
//		redisPool.pushBackConn(connNum);
		}
		redisPool.closeConnPool();
		return;
	} catch( RdException& e )
	{
		redisPool.closeConnPool();
		std::cout << "Redis exception:" << e.what() << std::endl;
	} catch( Poco::Exception& e )
	{
		redisPool.closeConnPool();
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}
}

//// 测试 CRedisPool::pushBackConn(CRedisClient *) 释放连接
//void testPoolmain( )
//{
//	CRedisClient *pRedis1 = NULL;
//	CRedisPool redisPool;
//	std::string value;
//	//test CRedisClient::init()  ::getConn()   ::pushBackConn()
//
//	redisPool.init("127.0.0.1", 6379, "", 6, 100, 6);
//	for ( int i = 0 ; i < 100 ; i++ )
//	{
//		pRedis1 = redisPool.getConn();
//		if ( pRedis1 == NULL )
//			return;
//
//		pRedis1->get("two", value);
//		std::cout << "getConn():two = " << value << "i = " << i << std::endl;
////		redisPool.pushBackConn(pRedis1);
//
//	}
//	redisPool.pushBackConn( pRedis1 );
//	redisPool.pushBackConn( pRedis1 );
//	if ( pRedis1 == NULL )
//		return;
//	value.clear();
//	pRedis1->get("two", value);
//	std::cout << "getConn(int&):two = " << value << std::endl;
//	redisPool.closeConnPool();
//	return;
//
//}

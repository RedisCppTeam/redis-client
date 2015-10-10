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

	//test CRedisClient::init()  ::getConn()   ::pushBackConn()

	redisPool.init("127.0.0.1", 6379, "", 6, 1000, 6);
	for ( int i = 0 ; i < 500 ; i++ )
	{
		pRedis1 = redisPool.getConn();
		std::string value;
		pRedis1->get("two", value);
		std::cout << "getConn():two = " << value << "i = " << i << std::endl;
		redisPool.pushBackConn(pRedis1);

		//test  CRedisClient::getConn( int& )   ::pushBackConn( int& )
		int connNum;
		pRedis1 = redisPool.getConn(connNum);
		if ( NULL != pRedis1 )
		{
			value.clear();
			pRedis1->get("two", value);
			std::cout << "getConn(int&):two = " << value << "ii = " << i<< std::endl;
			redisPool.pushBackConn(connNum);
		}

		pRedis1 = redisPool.getConn(connNum);
		if ( NULL != pRedis1 )
		{
			value.clear();
			pRedis1->get("two", value);
			std::cout << "getConn(int&):two = " << value << "iii = " << i<< std::endl;
			redisPool.pushBackConn(connNum);
		}
	}

	redisPool.closeConnPool();
	return;

}

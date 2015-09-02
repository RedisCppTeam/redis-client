/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	testScript.cpp		
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 *
 * @author: 		stanley
 * @date: 		Sep 2, 2015
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

void testEvalSha( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;
	CResult result;
	string retString;
	if ( !redis.scriptLoad(retString, "return 'hello moto'") )
	{
		cout << "redis.scriptLoad failed:" << endl;
		return;
	}
	cout << "redis.scriptLoad ok:" << retString << endl;

	ret = redis.evalSha(result, retString);
	if ( ret )
		cout << "redis.evalSha ok:" << ret << endl;

	else
		cout << "redis.evalSha failed:" << ret << endl;

	cout << "type:" << CResult::getTypeString(result.getType()) << endl;
	cout << "return:" << result << endl;

}



void testScriptExists( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	bool ret;
	CResult result;
	string retString;
	if ( !redis.scriptLoad(retString, "return 'hello moto'") )
	{
		cout << "redis.scriptLoad failed:" << endl;
		return;
	}
	cout << "redis.scriptLoad ok:" << retString << endl;

	ret = redis.scriptExists(retString);
	if ( ret )
		cout << "script exists ok:" << retString << endl;

	else
		cout << "script nonexists:" << retString << endl;

	retString += '2';
	ret = redis.scriptExists(retString);
	if ( ret )
		cout << "script exists ok:" << retString << endl;

	else
		cout << "script nonexists:" << retString << endl;

}
void testScriptFlush( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	if ( !redis.scriptFlush() )
	{
		cout << "scriptFlush failed:" << endl;
		return;
	}
	cout << "scriptFlush ok!" << endl;

}

//redis-cli:>EVAL "while true do end" 0
void testScriptKill( )
{
	CRedisClient redis;
	redis.connect("127.0.0.1", 6379);
	string retStr;
	if ( !redis.scriptKill() )
	{
		cout << "scriptFlush failed:" << endl;
		return;
	}

	cout << "scriptFlush ok!" << endl;

}



//逐一测试
void testScriptMain( )
{
	try
	{
		testScriptKill();
//		testScriptFlush();
//		testScriptExists();
//		testevalSha();
	} catch( RdException& e )
	{
		std::cout << endl << "Redis exception !!!!:" << e.what() << std::endl;
		return ;
	} catch( Poco::Exception& e )
	{
		std::cout << "Poco_exception:" << e.what() << std::endl;
	}

	cout << "exit main" << endl;
	return ;
}


/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	RedisClientScript.cpp		
 * @brief 实现redis客户端Script指令
 *
 *
 * @author: 		Stanley Huang
 * @date: 		Aug 12, 2015
 *
 * 修订说明:初始版本
 */

#include "Command.h"
#include "CRedisClient.h"

bool CRedisClient::eval( CResult& values , const string& script , const VecString& keysVec ,
		const VecString& argsVec )
{
	Command cmd("EVAL");
	string status;
	int len = keysVec.size();

	cmd << script << len;
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << keysVec[i];
	}

	len = argsVec.size();
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << argsVec[i];
	}

	_socket.clearBuffer();
	_sendCommand(cmd);
	if ( _getReply(values) )
	{
		return true;
	}
	return false;
}

bool CRedisClient::evalSha( CResult& values , const string& sha , const VecString& keysVec ,
		const VecString& argsVec )
{
	Command cmd("EVALSHA");
	string status;
	int len = keysVec.size();

	cmd << sha << len;
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << keysVec[i];
	}

	len = argsVec.size();
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << argsVec[i];
	}

	_socket.clearBuffer();
	_sendCommand(cmd);
	if ( _getReply(values) )
	{
		return true;
	}
	return false;
}

bool CRedisClient::scriptLoad( string& values , const string& script )
{
	Command cmd("SCRIPT");

	cmd << "LOAD" << script;
	return _getString(cmd, values);
}
bool CRedisClient::scriptExists( const string& script )
{
	Command cmd("SCRIPT");
	cmd << "EXISTS" << script;

	CResult rst;
	_getArry(cmd, rst);
	CResult::ListCResult lst = rst.getArry();
	CResult::ListCResult::const_iterator it = lst.begin();
	return it->getInt();
}

bool CRedisClient::scriptFlush( void )
{
	Command cmd("SCRIPT");
	string status;
	cmd << "FLUSH";

	if ( _getStatus(cmd, status) )
	{
		if ( status.find("OK") < 10 || status.find("ok") < 10 )
		{
			return true;
		}
	}
	return false;
}
bool CRedisClient::scriptKill()
{
	Command cmd("SCRIPT");
	cmd << "KILL";

	string retStr;
	if ( _getStatus(cmd, retStr) )
	{

		return true;
	}
	return false;
}

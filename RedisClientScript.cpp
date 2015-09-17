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

void CRedisClient::eval( CResult& result , const string& script , const VecString& keysVec ,
		const VecString& argsVec )
{
	Command cmd("EVAL");
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
    _getResult( cmd, result );
}

void CRedisClient::evalSha( CResult& result , const string& sha , const VecString& keysVec ,
		const VecString& argsVec )
{
	Command cmd("EVALSHA");

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

    _getResult( cmd, result );
}

void CRedisClient::scriptLoad(const string& script , string& values )
{
	Command cmd("SCRIPT");

	cmd << "LOAD" << script;
    _getString(cmd, values);
}

uint64_t CRedisClient::scriptExists( const string& script , VecString& result )
{
	Command cmd("SCRIPT");
	cmd << "EXISTS" << script;

    uint64_t num = 0;
    _getArry( cmd, result, num );
    return num;
}

void CRedisClient::scriptFlush( void )
{
	Command cmd("SCRIPT");
	string status;
	cmd << "FLUSH";
    _getStatus(cmd, status);
    if ( "OK"!=status)
        throw ProtocolErr( "CONFIG RESETSTAT: data recved is not OK" );
}

void CRedisClient::scriptKill()
{
	Command cmd("SCRIPT");
	cmd << "KILL";
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ProtocolErr( "CONFIG RESETSTAT: data recved is not OK" );
}

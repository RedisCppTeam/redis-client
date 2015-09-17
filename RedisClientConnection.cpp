/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	RedisClientCommon.cpp		
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 *
 * @author: 		cj
 * @date: 		Aug 11, 2015
 *
 * 修订说明:初始版本
 */

#include"Command.h"
#include"CRedisClient.h"

bool CRedisClient::ping( string &value )
{
    try
    {
        Command cmd("PING");
        _getStatus(cmd, value);
        return true;
    }catch( ... )
    {
        return false;
    }
}

void CRedisClient::quit( )
{
	Command cmd("QUIT");
	string value;
    _getStatus(cmd, value);
}

void CRedisClient::echo( const string &message , string &value )
{
	Command cmd("ECHO");
	cmd << message;
    _getString(cmd, value);
}

void CRedisClient::auth( const string &password )
{
	Command cmd("AUTH");
	cmd << password;
	string value;
    _getStatus(cmd, value);
}

void CRedisClient::select( uint64_t index )
{
	Command cmd("SELECT");
	cmd << index;
	string value;
    _getStatus(cmd, value);
}




/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	CRedisClientHyperLogLog.cpp		
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 *
 * @author: 		cj
 * @date: 		Aug 13, 2015
 *
 * 修订说明:初始版本
 */

#include"Command.h"
#include"CRedisClient.h"
namespace Taiji {
namespace Redis {



uint64_t CRedisClient::pfadd( const string &key , const VecString &element )
{
	Command cmd("PFADD");
	cmd << key;
    VecString::const_iterator it = element.begin();
    VecString::const_iterator end = element.end();
	while ( it != end )
	{
		cmd << *it;
		++it;
	}

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

uint64_t CRedisClient::pfcount( const VecString &key )
{
	Command cmd("PFCOUNT");
    VecString::const_iterator it = key.begin();
    VecString::const_iterator end = key.end();
	while ( it != end )
	{
		cmd << *it;
		++it;
	}

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::pfmerge( const string &destKey , const VecString &srcKey )
{
	Command cmd("PFMERGE");
	cmd << destKey;
    VecString::const_iterator it = srcKey.begin();
    VecString::const_iterator end = srcKey.end();
	while ( it != end )
	{
		cmd << *it;
		++it;
	}

	string value;
	return _getStatus(cmd, value);
}


}
}

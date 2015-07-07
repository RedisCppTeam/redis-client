/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	CRedisPool.cpp		
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 *
 * @author: 		yp
 * @date: 		Jul 6, 2015
 *
 * 修订说明:初始版本
 */
#include "CRedisPool.h"




CRedisPool::CRedisPool()
{
	_mHost.clear();
	_mPort = 0;
	_mPassword.clear();
	_mTimeout = 0;

	_mMinSize = 5;
	_mMaxSize = 10;
	_mStatus = REDISPOOL_UNCONN;

	_mConnlist.clear();
}

CRedisPool::~CRedisPool()
{

}


CRedisPool* CRedisPool::GetInstance()
{
	return NULL;
}


bool CRedisPool::init(const std::string& host, uint16_t port, const std::string& pass,
		uint32_t timeout, uint16_t minSize, uint16_t  maxSize)
{
	return true;
}


CRedisClient* CRedisPool::popConn(void)
{
	return NULL;
}

bool CRedisPool::pushConn(const CRedisClient* pConn)
{
	return true;
}




//CRedisPool.cpp

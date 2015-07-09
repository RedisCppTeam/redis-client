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
	_host.clear();
	_port = 0;
	_passWord.clear();
	_timeOut = 0;

	_minSize = 5;
	_maxSize = 10;

	_connList.clear();
	_bExitRun = false;
	_scanTime = 60000;
}

CRedisPool::~CRedisPool()
{

}




bool CRedisPool::init(const std::string& host, uint16_t port, const std::string& passWord,
		uint32_t timeout, uint16_t minSize, uint16_t  maxSize)
{
	_host = host;
	_port = port;
	_passWord = passWord;
	_timeOut = timeout;

	if (minSize <= maxSize)
	{
		_maxSize = maxSize;
		_minSize = minSize;
	}
	else
	{
		DEBUGOUT("CRedisPool::init:------Error:---minSize > maxSize---", "error");
		CL_LOG.error("CRedisPool::init:------Error:---minSize > maxSize---error");
		return false;
	}

	return true;
}


CRedisClient* CRedisPool::getConn(void)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();


	if (__getIdleCount() == 0 && _connList.size() >= _maxSize)
	{
		_cond.wait(_mutex);
	}
	else if(__getIdleCount() == 0)
	{
		SRedisConn* redisConn = new SRedisConn;

		try {
			redisConn->pConn.connect(_host, _port);
		} catch (Poco::Exception& e) {
			DEBUGOUT("CRedisPool::getConn:------Error:---", e.message());
			CL_LOG.error("CRedisPool::getConn:------Error:---", e.message());
			if(redisConn) {
				delete redisConn;
				redisConn = NULL;
			}
		}

		if(redisConn) {
			redisConn->idle = false;
			_connList.push_back(redisConn);
			return &redisConn->pConn;
		}

		return NULL;
	}


	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if(redisConn && redisConn->idle)
		{
			redisConn->idle = false;
			return &redisConn->pConn;
		}
	}

	return NULL;
}


void CRedisPool::pushBackConn(const CRedisClient* pConn)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if (redisConn && (&redisConn->pConn == pConn))
		{
			redisConn->idle = true;
			break;
		}
	}

	if(__getIdleCount() == 1)
		_cond.signal();
}


void CRedisPool::closeConnPool(void)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if (redisConn)
		{
			redisConn->pConn.closeConnect();
			delete redisConn;
		}
	}

	_connList.clear();
	_bExitRun = true;
}


void CRedisPool::keepAlive(void)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if (redisConn)
		{
			if(!redisConn->pConn.ping())
				redisConn->pConn.reconnect();
		}
	}

}


void CRedisPool::run()
{
	while(!_bExitRun)
	{
		DEBUGOUT("CRedisPool::run:------", "is Running");
		::usleep(_scanTime*1000);
		keepAlive();
	}
}


int CRedisPool::__getIdleCount(void)
{
	int idleCount = 0;

	RedisConnIter iter = _connList.begin();
	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if(redisConn && redisConn->idle)
		{
			idleCount ++;
		}
	}

	return idleCount;
}




//CRedisPool.cpp

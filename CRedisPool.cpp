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
	_password.clear();
	_timeout = 0;

	_minSize = 5;
	_maxSize = 10;

	_connList.clear();
	_bExitRun = false;
	_scanTime = 60;
	_idleTime = 60;
}

CRedisPool::~CRedisPool()
{

}




bool CRedisPool::init(const std::string& host, uint16_t port, const std::string& password, uint32_t timeout,
		uint16_t minSize, uint16_t  maxSize, uint32_t scanTime, uint32_t idleTime)
{
	_host = host;
	_port = port;
	_password = password;
	_timeout = timeout;
	_scanTime = scanTime;
	_idleTime = idleTime;

	if (minSize <= maxSize)
	{
		_maxSize = maxSize;
		_minSize = minSize;
	}
	else
	{
		DEBUGOUT("CRedisPool::init:------Error:---minSize > maxSize---", "error");
		return false;
	}

	return true;
}


CRedisClient* CRedisPool::getConn(void)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	int idleCount = __getIdleCount();

	if(!idleCount)
	{
		if (_connList.size() >= _maxSize)
		{
			_cond.wait(_mutex);
		}
		else
		{
			SRedisConn* redisConn = new SRedisConn;

			try {
				redisConn->pConn.connect(_host, _port);
			} catch (Poco::Exception& e) {
				DEBUGOUT("CRedisPool::getConn:------Error:---", e.message());
				if(redisConn) {
					delete redisConn;
					redisConn = NULL;
				}
				throw ConnectErr("This connection is timeout");
			}

			if(redisConn) {
				redisConn->idle = false;
				_connList.push_back(redisConn);
				return &redisConn->pConn;
			}

			return NULL;
		}
	}

	RedisConnIter iter = _connList.begin();
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


void CRedisPool::pushBackConn(CRedisClient* & pConn)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if (redisConn && (&redisConn->pConn == pConn))
		{
			redisConn->idle = true;
			redisConn->oldTime = time(0);
			pConn = NULL;
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

	int leftSize = _connList.size() - _minSize;
	for( ; iter != _connList.end(); )
	{
		SRedisConn* redisConn = *iter;
		if (redisConn)
		{
			if((leftSize > 0) && redisConn->idle && (time(0) - redisConn->oldTime) > 60)
			{
				delete redisConn;
				_connList.erase(iter);
				leftSize --;
			}
			else
			{
				if(!redisConn->pConn.ping())
					redisConn->pConn.reconnect();
				iter++;
			}
		}
		else
			iter++;
	}

}


void CRedisPool::run()
{
	while(!_bExitRun)
	{
		::sleep(_scanTime);
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

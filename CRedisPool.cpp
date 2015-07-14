/**
 * Copyright (c) 2015, iwifi (Copyright statement)
 *
 * @file	CRedisPool.cpp		
 * @brief CRedisPool class is to create and manage redis connections in the buffer pool.
 * These connections are ready to be used by any thread that needs them.
 * @author: 		yp
 * @date: 		Jul 6, 2015
 *
 * Revision Description: initial version
 */
#include "CRedisPool.h"




void OnRunCallBack(void* pVoid)
{
	CRedisPool* pRedisPool = (CRedisPool*)pVoid;
	if(pRedisPool)
	{
		//The state of the connection pool is not close.
		while(pRedisPool->status != CRedisPool::REDIS_POOL_DEAD)
		{
			::sleep(pRedisPool->scanTime);
			pRedisPool->keepAlive();	//keep connecting
		}
	}
}


CRedisPool::CRedisPool()
{
	status = REDIS_POOL_UNCONN;
	scanTime = 60;

	_host.clear();
	_port = 0;
	_password.clear();
	_timeout = 0;
	_minSize = 5;
	_maxSize = 10;
	_connList.clear();
	_idleTime = 60;
}

CRedisPool::~CRedisPool()
{

}




bool CRedisPool::init(const std::string& host, uint16_t port, const std::string& password, uint32_t timeout,
		uint16_t minSize, uint16_t  maxSize, uint32_t nScanTime, uint32_t idleTime)
{
	scanTime = nScanTime;

	_host = host;
	_port = port;
	_password = password;
	_timeout = timeout;
	_idleTime = idleTime;

	//the minSize value is less than the maxSize value
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

	_scanThread.start(OnRunCallBack, this);	//Starting scan thread

	return true;
}


CRedisClient* CRedisPool::getConn(void)
{
	if(status == REDIS_POOL_DEAD)	//If you closed the connection pool, return NULL
		return NULL;

	Poco::Mutex::ScopedLock lock(_mutex);
	int idleCount = __getIdleCount();	//get the count of idle connections in the pool

	if(idleCount == 0)
	{
		if ( _connList.size() >= _maxSize)
		{
			_cond.wait(_mutex);	//There is no idle connection in the pool, waiting...
		}
		else	//The count of connected pool is not up to the maximum.
		{
			SRedisConn* pRedisConn = new SRedisConn;

			try {
				pRedisConn->conn.connect(_host, _port);
			} catch (Poco::Exception& e) {	//The connection timeout, throw exception
				DEBUGOUT("CRedisPool::getConn:------Error:---", e.message());
				if(pRedisConn) {
					delete pRedisConn;
					pRedisConn = NULL;
				}
				throw ConnectErr("This connection is timeout");
			}

			if(pRedisConn) {
				pRedisConn->idle = false;
				_connList.push_back(pRedisConn); //Create success, push into the connection pool
				status = REDIS_POOL_WORKING;
				return &pRedisConn->conn;
			} else {
				return NULL;
			}
		}
	}

	//traverse connection pool, return idle connection
	RedisConnIter iter = _connList.begin();
	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* redisConn = *iter;
		if(redisConn && redisConn->idle)
		{
			redisConn->idle = false;
			return &redisConn->conn;
		}
	}

	return NULL;
}


void CRedisPool::pushBackConn(CRedisClient* & pConn)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

	int idleCount = __getIdleCount();
	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* pRedisConn = *iter;
		if (pRedisConn && (&pRedisConn->conn == pConn)) //find pConn
		{
			pRedisConn->idle = true;
			pRedisConn->oldTime = time(0); //Record the time stamp
			pConn = NULL;

			if(status == REDIS_POOL_DEAD)
				delete pRedisConn;//If you closed the connection pool, delete pConn
			else {
				if(idleCount == 0)
					_cond.signal();//wake up waiting
			}
			break;
		}
	}

}


void CRedisPool::closeConnPool(void)
{
	_mutex.lock();
	status = REDIS_POOL_DEAD;
	_cond.broadcast();	//Wake up all the wait

	RedisConnIter iter = _connList.begin();
	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* pRedisConn = *iter;
		if (pRedisConn && pRedisConn->idle)
			delete pRedisConn;	//Free the idle connection
	}
	_mutex.unlock();
	_scanThread.join();	//Waiting for the thread to end
}


void CRedisPool::keepAlive(void)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

	int leftSize = _connList.size() - _minSize; //Keep the minimum count of connections in the pool
	for( ; iter != _connList.end(); )
	{
		SRedisConn* pRedisConn = *iter;
		if (pRedisConn)
		{
			//The idle time for idle connections over the maximum of the idle time
			if((leftSize > 0) && pRedisConn->idle && (time(0) - pRedisConn->oldTime) > _idleTime)
			{
				delete pRedisConn;
				_connList.erase(iter);
				leftSize --;
			}
			else // if not idle and disconnected will be reconnect
			{
				if(!pRedisConn->idle && !pRedisConn->conn.ping())
					pRedisConn->conn.reconnect();
				iter++;
			}
		}
		else
			iter++;
	}

}


int CRedisPool::__getIdleCount(void)
{
	int idleCount = 0;

	RedisConnIter iter = _connList.begin();
	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* pRedisConn = *iter;
		if(pRedisConn && pRedisConn->idle)
		{
			idleCount ++;
		}
	}

	return idleCount;
}




//CRedisPool.cpp

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




void CRedisPool::__onRunCallBack(void* pVoid)
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
    _host.clear();
    _port = 0;
    _password.clear();
    _timeout = 0;
    _minSize = 5;
    _maxSize = 10;

    status = REDIS_POOL_UNCONN;
	scanTime = 60;
    _idleTime = 60;
    _connList.clear();
}

CRedisPool::~CRedisPool()
{

}


bool CRedisPool::init(const std::string& host, uint16_t port, const std::string& password, uint32_t timeout,
		uint16_t minSize, uint16_t  maxSize, uint32_t nScanTime, uint32_t idleTime)
{
    _host = host;
    _port = port;
    _password = password;
    _timeout = timeout;
    //the minSize value is less than the maxSize value
    if (minSize <= maxSize)
    {
        _maxSize = maxSize;
        _minSize = minSize;
    }
    else
    {
        return false;
    }

    scanTime = nScanTime;
    _idleTime = idleTime;

    scanThread.start(__onRunCallBack, this);	//Starting scan thread

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
                pRedisConn->_conn.connect(_host, _port);
			} catch (Poco::Exception& e) {	//The connection timeout, throw exception
				DEBUGOUT("CRedisPool::getConn:------Error:---", e.message());
				if(pRedisConn) {
					delete pRedisConn;
					pRedisConn = NULL;
				}
				throw ConnectErr("This connection is timeout");
			}

			if(pRedisConn) {
                pRedisConn->_idle = false;
				_connList.push_back(pRedisConn); //Create success, push into the connection pool
				status = REDIS_POOL_WORKING;
                return &pRedisConn->_conn;
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
        if(redisConn && redisConn->_idle)
		{
            redisConn->_idle = false;
            return &redisConn->_conn;
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
        if (pRedisConn && (&pRedisConn->_conn == pConn)) //find pConn
		{
            pRedisConn->_idle = true;
            pRedisConn->_oldTime = time(0); //Record the time stamp
			pConn = NULL;

			if(status == REDIS_POOL_DEAD)
			{
				delete pRedisConn;//If you closed the connection pool, delete pConn
				iter = _connList.erase(iter);
			}
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
	for( ; iter != _connList.end(); )
	{
		SRedisConn* pRedisConn = *iter;
        if (pRedisConn && pRedisConn->_idle)
		{
			delete pRedisConn;	//Free the idle connection
			iter = _connList.erase(iter);
		}
		else
			iter++;
	}
	_mutex.unlock();

	scanThread.join();	//Waiting for the thread to end
}


void CRedisPool::keepAlive(void)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	RedisConnIter iter = _connList.begin();

    for( ; iter != _connList.end(); )
    {
        SRedisConn* pRedisConn = *iter;
        // if the pointer's value is NULL delete it.
        if ( !pRedisConn )
        {
            iter = _connList.erase( iter );
            continue;
        }

        //if it is idle
        if ( pRedisConn->_idle )
        {
            if ( pRedisConn->getIdleTime()>_idleTime && _connList.size() > _minSize )
            {
                DEBUGOUT( "time out", "delete it");
                delete pRedisConn;
                iter = _connList.erase( iter );
            }
            else
            {
                if ( !pRedisConn->_conn.ping() )
                {
                    pRedisConn->_conn.reconnect();
                    DEBUGOUT( "Ping", "reconnect()");
                }else
                {
                    DEBUGOUT("Ping ok","" );
                }
                iter++;
            }
        }else
        {
            //if other thhread is using it.
            iter++;
        }
    }

}


int CRedisPool::__getIdleCount(void)
{
	int idleCount = 0;

	RedisConnIter iter = _connList.begin();
	for( ; iter != _connList.end(); iter++)
	{
		SRedisConn* pRedisConn = *iter;
        if(pRedisConn && pRedisConn->_idle)
		{
			idleCount ++;
		}
	}

	return idleCount;
}




//CRedisPool.cpp

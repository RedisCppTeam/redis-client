/**
 * Copyright (c) 2015, (Copyright statement)
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
#include "CRedisClient.h"
#include <stdlib.h>
#include <time.h>
using namespace std;

CRedisPool::CRedisPool( )
{
    _status = REDIS_POOL_UNCONN;
    _scanTime = 60;
    _host.clear();
    _port = 0;
    _password.clear();
    _timeout = 0;
    _poolSize = DEFALUT_SIZE;
    _connList.clear();
    _connectedNumber=0;
    srand(time(NULL));
}

CRedisPool::~CRedisPool( )
{
    closeConnPool();
}

bool CRedisPool::init( const std::string& host , uint16_t port , const std::string& password ,
                       uint32_t timeout , int32_t poolSize , uint32_t nScanTime )
{
    _scanTime = nScanTime;
    _host = host;
    _port = port;
    _password = password;
    _timeout = timeout;
    _poolSize = poolSize;
    _connList.resize(_poolSize, NULL);

    int32_t i;
    for ( i = 0; i < _poolSize ; i++ )
    {
        SRedisConn* pRedisConn = new SRedisConn;
        pRedisConn->idle = true;
        pRedisConn->connStatus = true;
        try
        {
            pRedisConn->conn.connect(_host, _port);
        } catch( Poco::Exception& e )
        {
            DEBUGOUT("Poco::Exception", e.displayText());
            if ( pRedisConn )
            {
                delete pRedisConn;
                pRedisConn = NULL;
            }
            return false;
        }
        _connList[i] = pRedisConn;
    }
    _status = REDIS_POOL_WORKING;
    _scanThread.start(__onRunCallBack, this);
    return true;
}


CRedisClient* CRedisPool::_getConn( int32_t& connNum,long millisecond )
{
    if ( _status != REDIS_POOL_WORKING )
        return NULL;
    Poco::Mutex::ScopedLock lock(_mutex);
    connNum = __getConn();
    if ( connNum < 0 )
    {
        DEBUGOUT( "getConn():", "waitting for a idle connection" );
        _cond.wait(_mutex,millisecond);
        connNum = __getConn();
    }
    if ( connNum >= 0 )
    {
        _connectedNumber++;
        return &( _connList[connNum]->conn );
    }

    return NULL;
}


void CRedisPool::_putBackConn( int32_t& connNum )
{
    if ( connNum < 0 || connNum > _poolSize  )
    {
        return;
    }
    if ( _status != REDIS_POOL_WORKING )
        return;
    Poco::Mutex::ScopedLock lock(_mutex);
    if (!_connList[connNum]->idle)
        _connectedNumber--;
    _connList[connNum]->idle = true;
    connNum = -1;
    _cond.signal();
}

std::shared_ptr<CRedisClient> CRedisPool::getConn(long millisecond)
{
    int32_t connNum;
    CRedisClient* pRedis = _getConn(connNum,millisecond);
    int32_t * pConnNum = new int32_t(connNum);
    auto deleter = [pConnNum,this](CRedisClient * pConn)
    {
        int32_t n = *pConnNum;
        delete pConnNum;
        if(pConn)
            this->_putBackConn(n);

    };
    std::shared_ptr<CRedisClient> pConn =std::shared_ptr<CRedisClient>(pRedis,deleter);
    return pConn;
}

void CRedisPool::closeConnPool( void )
{
    if ( _status == REDIS_POOL_DEAD )
        return;
    _mutex.lock();
    _status = REDIS_POOL_DEAD;
    int32_t i;
    SRedisConn* pRedisConn;
    for ( i = 0; i < _poolSize ; i++ )
    {
        pRedisConn = _connList[i];
        if ( pRedisConn )
        {
            _connList[i] = NULL;
            delete pRedisConn;
        }
    }

    _mutex.unlock();
    _scanThread.join();	//Waiting for the thread to end
}

int32_t CRedisPool::__getConn( )
{
    int32_t i, tRand, tmp;
    tRand = rand() % _poolSize;
    for ( i = 0; i < _poolSize ; i++ )
    {
        tmp = ( tRand + i ) % _poolSize;
        if ( ( _connList[tmp]->idle ) && ( _connList[tmp]->connStatus ) )
        {
            _connList[tmp]->idle = false;
            return tmp;
        }
    }
    return -1;
}

void CRedisPool::__keepAlive( void )
{
    Poco::Mutex::ScopedLock lock(_mutex);
    if ( _status == REDIS_POOL_DEAD )
        return;

    int32_t i;
    SRedisConn* pRedisConn;
    string value;
    for ( i = 0; i < _poolSize ; i++ )
    {
        pRedisConn = _connList[i];
        if ( !pRedisConn->idle )
            continue;
        pRedisConn->connStatus = true;
        if ( !pRedisConn->conn.ping(value) )
        {
            try
            {
                pRedisConn->conn.reconnect();
            } catch( Poco::Exception& e )
            {
                pRedisConn->idle = true;
                pRedisConn->connStatus = false;
                DEBUGOUT("CRedisPool::keepAlive:------reconnect--Error:---", e.message());
                DEBUGOUT("the connect number ", i);
            }
        }
    }
}
int32_t CRedisPool::connectedNumber() const
{
    return _connectedNumber;
}


void CRedisPool::__onRunCallBack( void* pVoid )
{
    CRedisPool* pRedisPool = (CRedisPool*) pVoid;
    if ( pRedisPool )
    {
        while ( pRedisPool->_status == CRedisPool::REDIS_POOL_WORKING )
        {
            ::sleep(pRedisPool->_scanTime);
            pRedisPool->__keepAlive();
        }
    }
}

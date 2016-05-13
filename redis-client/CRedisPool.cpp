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
namespace Taiji {
namespace Redis {



CRedisPool::CRedisPool( )
{
    _status = REDIS_POOL_UNCONN;
    _scanTime = 60;
    _host.clear();
    _port = 0;
    _password.clear();
    _timeout = 0;
    _poolSize = 0;
    _connList.clear();
    _havePassword = false;
    srand(time(NULL));
}

CRedisPool::~CRedisPool( )
{
    closeConnPool();
}

bool CRedisPool::init(const string &host, uint16_t port, const string &password, uint32_t timeout, int32_t poolSize, uint32_t nScanTime)
{
    _scanTime = nScanTime;
    _host = host;
    _port = port;
    _havePassword = true;
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
        pRedisConn->conn.connect(_host, _port);
        pRedisConn->conn.auth(password);
        _connList[i] = pRedisConn;
    }
    _status = REDIS_POOL_WORKING;
    _scanThread.start(__onRunCallBack, this);
    return true;
}

bool CRedisPool::init(const std::string& host , uint16_t port ,
                      uint32_t timeout , int32_t poolSize , uint32_t nScanTime )
{
    _scanTime = nScanTime;
    _host = host;
    _port = port;
    _timeout = timeout;
    _poolSize = poolSize;
    _connList.resize(_poolSize, NULL);

    int32_t i;
    for ( i = 0; i < _poolSize ; i++ )
    {
        SRedisConn* pRedisConn = new SRedisConn;
        pRedisConn->idle = true;
        pRedisConn->connStatus = true;
        pRedisConn->conn.connect(_host, _port);
        _connList[i] = pRedisConn;
    }
    _status = REDIS_POOL_WORKING;
    _scanThread.start(__onRunCallBack, this);
    return true;
}

CRedisClient *CRedisPool::getConn(long millisecond)
{
    if ( _status != REDIS_POOL_WORKING )
    {
        throw ExceptHandle("lack handle");
    }
    Poco::Mutex::ScopedLock lock(_mutex);

    int32_t connNum = _getConn();
    if ( connNum < 0 )
    {
        REDIS_DEBUGOUT( "getConn()", "waitting for a idle connection" );
        _cond.wait(_mutex, millisecond );
        connNum = _getConn();
    }
    if ( connNum >= 0 )
    {
        return &( _connList[connNum]->conn );
    }
    throw ExceptHandle("lack handle");
    return NULL;
}

CRedisClient* CRedisPool::getConn( int32_t& connNum,long millisecond )
{
    if ( _status != REDIS_POOL_WORKING )
    {
        throw ExceptHandle("lack handle");
    }
    Poco::Mutex::ScopedLock lock(_mutex);
    connNum = _getConn();
    if ( connNum < 0 )
    {
        REDIS_DEBUGOUT( "getConn():", "waitting for a idle connection" );
        _cond.wait(_mutex,millisecond);
        connNum = _getConn();
    }
    if ( connNum >= 0 )
        return &( _connList[connNum]->conn );
    throw ExceptHandle("lack handle");
    return NULL;
}

void CRedisPool::pushBackConn( CRedisClient *&pConn )
{
    if ( _status != REDIS_POOL_WORKING )
        return;
    Poco::Mutex::ScopedLock lock(_mutex);
    int32_t i;
    for ( i = 0; i < _poolSize ; i++ )
    {
        if ( _connList[i] != NULL )
        {
            if ( &( _connList[i]->conn ) == pConn )
                _connList[i]->idle = true;
        }
    }
    _cond.signal();
}

void CRedisPool::pushBackConn( int32_t connNum )
{
    if ( _status != REDIS_POOL_WORKING )
        return;
    Poco::Mutex::ScopedLock lock(_mutex);
    _connList[connNum]->idle = true;
    _cond.signal();
}

CRedisPool::Handle CRedisPool::getRedis(long millisecond)
{
    int32_t place = -1;
    CRedisClient* predis = getConn( place,millisecond );
    if (NULL == predis)
    {
        throw ExceptHandle("lack handle");
    }
    //智能指针的析构器。把 redis 对象 putBack() 回去。
    auto deleter = [ place,this ]( CRedisClient * pConn )
    {
        if( place >= 0 && nullptr != pConn  )
        {
            this->pushBackConn( place );
        }
    };
    return CRedisPool::Handle( predis,deleter );
}

void CRedisPool::closeConnPool( void )
{
    if ( _status != REDIS_POOL_WORKING )
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

int32_t CRedisPool::_getConn( )
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

void CRedisPool::_keepAlive( void )
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
                if (_havePassword)
                    pRedisConn->conn.auth(_password);
            } catch( Poco::Exception& e )
            {
                pRedisConn->idle = true;
                pRedisConn->connStatus = false;
                REDIS_DEBUGOUT("CRedisPool::keepAlive:------reconnect--Error:---", e.message());
                REDIS_DEBUGOUT("the connect number ", i);
            }
        }
    }
}

void CRedisPool::__onRunCallBack( void* pVoid )
{
    CRedisPool* pRedisPool = (CRedisPool*) pVoid;
    if ( pRedisPool )
    {
        while ( pRedisPool->_status == CRedisPool::REDIS_POOL_WORKING )
        {
            ::sleep(pRedisPool->_scanTime);
            pRedisPool->_keepAlive();
        }
    }
}



}
}

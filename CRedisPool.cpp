/**
 * Copyright (c) 2016, iwifi (Copyright statement)
 * @file	CRedisPool.h
 * @brief          redis固定连接数量的连接池
 * @author: 		zhangxinhao
 * @date: 	2016-01-12
 * Revision Description: initial version
 */
#include "CRedisPool.h"
#include <time.h>

bool CRedisPool::init(const std::string &host, const uint16_t port, const std::string &password,uint32_t timeout,
                      int32_t poolSize, uint32_t scanSecond)
{
    _scanTime = scanSecond;
    srand(time(nullptr));
    if (password.size()){}
    _poolSize = poolSize;
    _connList.resize(_poolSize,SRedisConn{nullptr,true,true});
    _redisPoolIsExistConn = true;
    try
    {
        for (auto& it: _connList)
        {
            it.pConn = new CRedisClient;
            it.pConn->connect(host, port);
            it.pConn->setTimeout(timeout,0);
        }
    }
    catch( std::exception& e )
    {
        REDISPOOL_DEBUG("redis pool init fail","");
        return false;
    }
    _scanThread = std::thread(__timer,this);
    _isThreadStart = true;
    REDISPOOL_DEBUG("redis pool init ok","");
    return true;
}

void CRedisPool::closeConnPool()
{
    if ( !_redisPoolIsExistConn )
        return;
    _mutex.lock();
    _redisPoolIsExistConn = false;
    for (auto& it: _connList)
    {
        delete it.pConn;
        it.pConn = nullptr;
    }
    _mutex.unlock();
    if (_isThreadStart)
        _scanThread.join();
    REDISPOOL_DEBUG("redis pool close ok","");
}

CRedisPool::Handle CRedisPool::getConn(long millisecond)
{
    int32_t connNum = -1;
    if (!_getConnNum(connNum,millisecond))
        return nullptr;
    auto deleter = [connNum,this](CRedisClient * pConn)
    {
        int connN = connNum;
        if(pConn)
            this->_putBackConnNum(connN);
    };
    return CRedisPool::Handle(_connList[connNum].pConn,deleter);
}

bool CRedisPool::_getConnNum(int32_t &connNum, long millisecond)
{
    auto getConnNum = [this]()
    {
        int32_t i, tRand, tmp;
        tRand = rand() % (this->_poolSize);
        for ( i = 0; i < (this->_poolSize) ; i++ )
        {
            tmp = ( tRand + i ) % (this->_poolSize);
            if ( ( this->_connList[tmp].idle ) && ( this->_connList[tmp].connStatus ) )
            {
                this->_connList[tmp].idle = false;
                return tmp;
            }
        }
        return -1;
    };

    if (!_redisPoolIsExistConn)
        return false;
    std::unique_lock<std::mutex> lock(_mutex);
    connNum = getConnNum();
    if ( connNum < 0 )
    {
        if ((bool)_cond.wait_for(lock,std::chrono::milliseconds(millisecond)))
        {
            REDISPOOL_DEBUG("getConn wait time out","");
            return false;
        }
        connNum = getConnNum();
    }
    if ( connNum >= 0 )
    {
        _connectedNumber++;
        REDISPOOL_DEBUG("getConn",connNum);
        REDISPOOL_DEBUG("getConnAddr",(int*) _connList[connNum].pConn);
    }
    return true;
}

bool CRedisPool::_putBackConnNum(int32_t &connNum)
{
    if ( connNum < 0 || connNum > _poolSize  )
        return false;
    if (!_redisPoolIsExistConn)
        return false;
    std::unique_lock<std::mutex> lock(_mutex);
    if (!_connList[connNum].idle)
    {
        REDISPOOL_DEBUG("putConn",connNum);
        REDISPOOL_DEBUG("putConnAddr",(int*) _connList[connNum].pConn);
        _connectedNumber--;
    }
    _connList[connNum].idle = true;
    connNum = -1;
    _cond.notify_one();
    return true;
}

void CRedisPool::__timer(CRedisPool *pRedisPool)
{
    if ( pRedisPool )
    {
        while ( pRedisPool->_redisPoolIsExistConn )
        {
            REDISPOOL_DEBUG("timer","");
            std::this_thread::sleep_for(std::chrono::seconds(pRedisPool->_scanTime));
            pRedisPool->__keepAlive();
        }
    }
}

void CRedisPool::__keepAlive()
{
    std::unique_lock<std::mutex> lock(_mutex);
    if (!_redisPoolIsExistConn)
        return;
    std::string value;
    for (auto & it: _connList)
    {
        if (!it.idle)
            continue;
        it.connStatus = true;
        if ( !it.pConn->ping(value) )
        {
            try{
                it.pConn->reconnect();
            }catch(...){
                it.idle = true;
                it.connStatus = false;
                REDISPOOL_DEBUG("redispool keepAlive error","");
            }
        }
    }
}

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
#include <stdlib.h>
#include <time.h>
using namespace std;

CRedisPool::CRedisPool()
{
    status = REDIS_POOL_UNCONN;
    scanTime = 60;
    _host.clear();
    _port = 0;
    _password.clear();
    _timeout = 0;
    _poolSize=DEFALUT_SIZE;
    _connList.clear();
    srand(time(NULL));
}

CRedisPool::~CRedisPool()
{

}

bool CRedisPool::init(const std::string& host, uint16_t port, const std::string& password, uint32_t timeout,
                      int32_t poolSize, uint32_t nScanTime)
{
    scanTime = nScanTime;
    _host = host;
    _port = port;
    _password = password;
    _timeout = timeout;
    _poolSize= poolSize;
    _connList.resize(_poolSize,NULL);

    int32_t i;
    for(i=0;i<_poolSize;i++)
    {
        cout<<"init:"<<i<<endl;
        SRedisConn* pRedisConn = new SRedisConn;
        pRedisConn->idle=true;
        pRedisConn->connStatus=true;
        try {
            pRedisConn->conn.connect(_host, _port);
        } catch (Poco::Exception& e) {
            DEBUGOUT("CRedisPool::getConn:------Error:---", e.message());
            if(pRedisConn) {
                delete pRedisConn;
                pRedisConn = NULL;
            }
            //throw ConnectErr("This connection is timeout");
            return false;
        }
        _connList[i]=pRedisConn;
    }
    status = REDIS_POOL_WORKING;
    scanThread.start(__onRunCallBack, this);
    return true;
}

CRedisClient *CRedisPool::getConn()
{
    if(status != REDIS_POOL_WORKING)
        return NULL;
    Poco::Mutex::ScopedLock lock(_mutex);
    int32_t  connNum=_getConn();
    if (connNum<0)
    {
        _cond.wait(_mutex);
        connNum=_getConn();
    }
    if(connNum>=0)
        return   &( _connList[connNum]->conn);
    return NULL;
}

CRedisClient* CRedisPool::getConn(int32_t& connNum)
{
    if(status != REDIS_POOL_WORKING)
        return NULL;
    Poco::Mutex::ScopedLock lock(_mutex);
    connNum=_getConn();
    if (connNum<0)
    {
        _cond.wait(_mutex);
        connNum=_getConn();
    }
    if(connNum>=0)
        return   &( _connList[connNum]->conn);
    return NULL;
}

void CRedisPool::pushBackConn(CRedisClient *&pConn)
{
    if(status!=REDIS_POOL_WORKING)
        return;
    Poco::Mutex::ScopedLock lock(_mutex);
    int32_t i;
    for(i=0;i<_poolSize;i++)
    {
        if (_connList[i]!=NULL)
        {
            if (&(_connList[i]->conn)==pConn)
                _connList[i]->idle=true;
        }
    }
    _cond.signal();
}

void CRedisPool::pushBackConn(int32_t connNum)
{
    if(status!=REDIS_POOL_WORKING)
        return;
    Poco::Mutex::ScopedLock lock(_mutex);
    _connList[connNum]->idle=true;
    _cond.signal();
}

void CRedisPool::closeConnPool(void)
{
    if(status==REDIS_POOL_DEAD)
        return;
    _mutex.lock();
    status = REDIS_POOL_DEAD;
    int32_t i;
    SRedisConn* pRedisConn;
    for(i=0;i<_poolSize;i++)
    {
        pRedisConn=_connList[i];
        if (pRedisConn)
        {
            delete pRedisConn;
        }
    }
    _mutex.unlock();
    scanThread.join();	//Waiting for the thread to end
}

int32_t CRedisPool::_getConn()
{
    int32_t i,tRand,tmp;
    tRand=rand()%_poolSize;
    for(i=0;i<_poolSize;i++)
    {
        tmp=(tRand+i)%_poolSize;
        if ( (_connList[tmp]->idle)&&(_connList[tmp]->connStatus))
        {
            _connList[tmp]->idle=false;
            return tmp;
        }
    }
    return -1;
}

void CRedisPool::keepAlive(void)
{
    Poco::Mutex::ScopedLock lock(_mutex);
    int32_t i;
    SRedisConn* pRedisConn;
    string value;
    for(i=0;i<_poolSize;i++)
    {
        pRedisConn=_connList[i];
        if(!pRedisConn->conn.ping(value))
        {
            try {
                pRedisConn->conn.reconnect();
            }catch (Poco::Exception& e) {
                pRedisConn->idle=true;
                pRedisConn->connStatus=false;
                DEBUGOUT("CRedisPool::keepAlive:------reconnect--Error:---", e.message());
                DEBUGOUT("the connect number ",i);
            }
        }
    }
}

void CRedisPool::__onRunCallBack(void* pVoid)
{
    CRedisPool* pRedisPool = (CRedisPool*)pVoid;
    if(pRedisPool)
    {
        while(pRedisPool->status == CRedisPool::REDIS_POOL_WORKING)
        {
            ::sleep(pRedisPool->scanTime);
            pRedisPool->keepAlive();
        }
    }
}
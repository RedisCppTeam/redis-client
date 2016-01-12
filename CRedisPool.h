/**
 * Copyright (c) 2016, iwifi (Copyright statement)
 * @file	CRedisPool.h
 * @brief          redis固定连接数量的连接池
 * @author: 		zhangxinhao
 * @date: 	2016-01-12
 * Revision Description: initial version
 */
#ifndef CREDISPOOL_H
#define CREDISPOOL_H

#include "CRedisClient.h"
#include <thread>
#include <memory>
#include <condition_variable>
// 测试debug
#define REDISPOOL_DEBUG(x,y)   //std::cout<<(x)<<":"<<(y)<<std::endl;
class CRedisPool
{
public:
    typedef std::shared_ptr <CRedisClient> Handle ;

    CRedisPool() = default;

    CRedisPool(const CRedisPool&) = delete;

    /**
     * @brief init
     * 初始化连接池
     * @param host[in] redis地址
     * @param port[in] redis端口
     * @param password[in] redis密码(目前没用)
     * @param timeout[in] redis send recv超时时间(单位秒)
     * @param poolSize[in] redisPool连接数量
     * @param scanSecond[in] redisPool 保活间隔时间
     * @return 成功返回true，失败返回false。
     */
    bool init(const std::string& host, const uint16_t port, const std::string& password,uint32_t timeout=5,
              int32_t  poolSize=10, uint32_t scanSecond = 30);

    /**
     * @brief closeConnPool
     * 关闭连接池
     */
    void closeConnPool();

    /**
     * @brief getConn
     * 获取连接
     * @param millisecond[in] 获取连接超时时间，单位毫秒
     * @return redisclient 的智能指针
     */
    Handle getConn(long  millisecond ) ;

    /**
     * @brief connectedNumber
     * 获取当前已用连接
     * @return 连接数
     */
    int32_t connectedNumber() const  {
        return _connectedNumber;
    }

protected:

    /**
     * @brief _getConnNum
     * 获取连接的下标
     * @param connNum[out] 获取的下标
     * @param millisecond[in] 获取的超时时间
     * @return 成功返回true，失败返回false。
     */
    bool _getConnNum(int32_t& connNum,long millisecond);

    /**
     * @brief _putBackConnNum
     * 放回连接
     * @param connNum[in][out] 放回下标，置为-1
     * @return 成功返回true，失败返回false。
     */
    bool _putBackConnNum(int32_t& connNum);

private:
    typedef struct
    {
        CRedisClient *pConn;      ///<redis客户端
        bool idle;                      ///<是否空闲
        bool connStatus;         ///<连接是否可用
    } SRedisConn;                  ///<连接属性

    bool _redisPoolIsExistConn = false;      ///<是否存在了连接(连接池是否可用)
    int32_t _poolSize;                   ///<连接池的数量
    uint32_t _scanTime = 60;		///< 保活(扫描)线程间隔时间，单位秒
    int32_t _connectedNumber = 0;  ///<已获得的连接数
    bool _isThreadStart = false;  ///保活线程是否开启
    std::thread _scanThread;	///< 扫描线程
    std::vector<SRedisConn> _connList;   ///<redis连接表

    std::mutex _mutex;
    std::condition_variable _cond;

    //定时器
    static void __timer(CRedisPool* pRedisPool);

    //已取的连接将不再保活，可以放回保活
    void __keepAlive();
};

#endif // CREDISPOOL_H

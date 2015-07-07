/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	CRedisPool.h		
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 *
 * @author: 		yp
 * @date: 		Jul 6, 2015
 *
 * 修订说明:初始版本
 */
#ifndef CREDISCLIENT_CREDISPOOL_H_
#define CREDISCLIENT_CREDISPOOL_H_

#include "CRedisClient.h"



#define MAX_REDIS_CONN_POOLSIZE     32      // 每个DB最大连接数



typedef struct
{
	CRedisClient* pConn;
	bool idle;
} SRedisConn;


///< 连接链表
typedef std::list<SRedisConn*> RedisConnList;
typedef std::list<SRedisConn*>::iterator RedisConnIter;


typedef enum
{
    REDISPOOL_UNCONN = 0,
    REDISPOOL_WORKING,
    REDISPOOL_DEAD
}REDIS_POOL_STATE;




class CRedisPool
{
public:
	CRedisPool();
	~CRedisPool();

	static CRedisPool* GetInstance();


	bool init(const std::string& host, uint16_t port, const std::string& pass,
			uint32_t timeout=0, uint16_t minSize=5, uint16_t  maxSize=10);


	CRedisClient* popConn(void);

	bool pushConn(const CRedisClient* pConn);


private:
	std::string _mHost;
	uint16_t _mPort;
	std::string _mPassword;
	uint32_t _mTimeout;
	uint16_t _mMinSize;
	uint16_t _mMaxSize;

	RedisConnList _mConnlist;
	REDIS_POOL_STATE _mStatus;

	Poco::Mutex _mutex;

};




#endif /* CREDISCLIENT_CREDISPOOL_H_ */

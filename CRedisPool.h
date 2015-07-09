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
#include <Poco/Condition.h>


typedef struct
{
	CRedisClient pConn;
	bool idle;
} SRedisConn;


///< 连接链表
typedef std::list<SRedisConn*> RedisConnList;
typedef std::list<SRedisConn*>::iterator RedisConnIter;
//typedef std::map<std::string, RedisConnList*> RedisConnListMap;




class CRedisPool : public Poco::Runnable
{
public:
	CRedisPool();
	~CRedisPool();


	bool init(const std::string& host, uint16_t port, const std::string& passWord,
			uint32_t timeout=0, uint16_t minSize=5, uint16_t  maxSize=10);

	CRedisClient* getConn(void);

	void pushBackConn(const CRedisClient* pConn);

	void keepAlive(void);

	void closeConnPool(void);

	virtual void run();

private:
	std::string _host;
	uint16_t _port;
	std::string _passWord;
	uint32_t _timeOut;
	uint16_t _minSize;
	uint16_t _maxSize;//192.168.10.179//---

	RedisConnList _connList;
	//RedisConnListMap _connListMap;

	bool _bExitRun;
	uint32_t _scanTime;	///< 单位:毫秒
	Poco::Mutex _mutex;
	Poco::Condition _cond;

	DISALLOW_COPY_AND_ASSIGN(CRedisPool);
	int __getIdleCount(void);

};




#endif /* CREDISCLIENT_CREDISPOOL_H_ */

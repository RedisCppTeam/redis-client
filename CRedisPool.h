/**
 * Copyright (c) 2015, iwifi (Copyright statement)
 *
 * @file	CRedisPool.h		
 * @brief CRedisPool class is to create and manage redis connections in the buffer pool.
 * These connections are ready to be used by any thread that needs them.
 * @author: 		yp
 * @date: 		Jul 6, 2015
 *
 * Revision Description: initial version
 */
#ifndef CREDISCLIENT_CREDISPOOL_H_
#define CREDISCLIENT_CREDISPOOL_H_


#include "CRedisClient.h"
#include <Poco/Condition.h>




class CRedisPool
{
public:
	///< single connection
	typedef struct
	{
		CRedisClient conn;	///< connection
		time_t oldTime;		///< idle time stamp
		bool idle;		///< idle state
	} SRedisConn;


	///< redis pool state
	typedef enum
	{
	    REDIS_POOL_UNCONN = 0,	///< no connection
	    REDIS_POOL_WORKING,		///< connecting
	    REDIS_POOL_DEAD		///< disconnect
	}REDIS_POOL_STATE;


	///< linked list
	typedef std::list<SRedisConn*> RedisConnList;
	typedef std::list<SRedisConn*>::iterator RedisConnIter;


	CRedisPool();
	~CRedisPool();


	/**
	* @brief initial connection pool, starting scan thread
	* @param host [in] host ip
	* @param port [in] host port
	* @param password [in] host password
	* @param timeout [in] timeout period, default 0
	* @param minSize [in] minimum value of connections, default 5
	* @param maxSize [in] maximum value of connections, default 10
	* @param nScanTime [in] thread scan time, default 60
	* @param idleTime [in] idle time, default 60
	* @return if success return true else return false
	* @warning If the maximum value is less than the minimum value will initialize the failure.
	*/
	bool init(const std::string& host, uint16_t port, const std::string& password, uint32_t timeout=0,
			uint16_t minSize=5, uint16_t  maxSize=10, uint32_t nScanTime = 60, uint32_t idleTime = 60);

	/**
	* @brief get a single connection in the pool
	* @return return a connection, if busy will wait
	* @warning The connection timeout will throw exception.
	*/
	CRedisClient* getConn(void);

	/**
	* @brief put back a connection to the pool
	* @param pConn [in and out] a connection reference
	* @warning pConn will be set free.If you close the connection pool, not pushBackConn will cause the memory leak.
	*/
	void pushBackConn(CRedisClient* & pConn);

	/**
	* @brief traverse connection pool, if disconnected will be reconnect
	* @warning If the idle time is reached, the connection will be released.
	*/
	void keepAlive(void);

	/**
	* @brief close connection pool
	* @warning Free idle connection, waiting for the scan thread to end.
	*/
	void closeConnPool(void);


	uint32_t scanTime;		///< thread scan time, unit: Second
	REDIS_POOL_STATE status;	///< redis pool state
	Poco::Thread scanThread;	///< scan thread, if disconnected will be reconnect


private:
	std::string _host;		///< host ip
	uint16_t _port;			///< host port
	std::string _password;		///< host password
	uint32_t _timeout;		///< timeout period, default 0
	uint16_t _minSize;		///< minimum value of connections, default 5
	uint16_t _maxSize;		///< maximum value of connections, default 10

	RedisConnList _connList;	///< the list of redis connection pool
	uint32_t _idleTime;		///< connection idle time, unit: Second

	Poco::Mutex _mutex;
	Poco::Condition _cond;
	DISALLOW_COPY_AND_ASSIGN(CRedisPool);

	/**
	* @brief get the count of idle connections in the pool
	* @return return the count of idle connections
	*/
	int __getIdleCount(void);


};




#endif /* CREDISCLIENT_CREDISPOOL_H_ */

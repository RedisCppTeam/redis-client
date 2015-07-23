/**
 * @file	CRedisClient.h
 * @brief CRedisClient 类的方法声明。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */
#ifndef CREDIS_CLIENT_H
#define CREDIS_CLIENT_H

#include <stdint.h>
#include <vector>
#include <Poco/Net/StreamSocket.h>
#include "Command.h"
#include "common.h"
#include "RdException.hpp"
#include "CRedisSocket.h"

#include "CResult.h"

using namespace Poco;

/**
 *@brief CRedisClient redis client
 */
class CRedisClient
{
public:

    typedef  	vector<std::string> VecString;

    typedef 	vector<CResult> VecResult;

    CRedisClient();
    ~CRedisClient();

    /**
     * @brief setAddress set redis-server ipaddress.
     * @param ip	redis-server ip.
     * @param port redis-server port.
     */
    void setAddress( const string& ip, UInt16 port );

    /**
     * @brief getAddrip
     * @return get redis-server ip
     */
    string getAddrip( void );

    /**
     * @brief getAddrPort
     * @return get redis-server port
     */
    UInt16 getAddrPort( void );

    /**
     * @brief getAddr
     * @return return the addr for redis-server
     */
    string getAddr();


    /**
     * @brief setTimeOut  		 Sets the connect timeout,send timeout,recv timeout for the socket.
     * @param seconds
     * @param microseconds
     */
    void setTimeout( long seconds, long microseconds  );

    /**
     * @brief connect to redis-server
     * @param ip [in] host ip
     * @param port [in] host port
     * @warning Will throw an exception when the connection fails.
     */
    void connect( const string& ip, UInt16 port=6379 );

    /**
     * @brief connect to redis-server.
     * @warning Must set address before.
     */
    void connect();

    /**
     * @brief reconnect redis-server
     */
    void reconnect();

    void closeConnect();
    //---------------------------------common----------------------------------------

    bool ping( void );


    //-----------------------------------key---------------------------------------------
    /**
     * @brief keys get all keys matching pattern. 
     * @param pattern [in] The conditions of the matching. 
     * @param keys [out] vector of keys maching pattern 
     * @return The number of keys returned.
     */
    uint64_t keys( const string& pattern, VecResult& keys );

    uint64_t del( VecString& keys );

    
    //-----------------------------string method--------------------------------------
    /**
     * @brief set set a string type key = value
     * @param key
     * @param value
     * @warning  could throw Poco::Exception and ProtocolErr exception
     */
    void	set( const string& key, const string& value );

    void get(const string& key, CResult &value );
    //------------------------------list method--------------------------------------
    uint64_t lpush( const string& key ,const string&value );

    uint64_t lpush( const string& key, const VecString& value );

    //------------------------------hash method-----------------------------------
    /**
     * @brief hset  insert into a value to hash name is key field is field .
     * @param key  hash name
     * @param field hash field
     * @param value to insert data
     * @return return 1, a new data insert into hash.return 0, a old data update.
     */
    uint8_t hset( const string& key, const string& field,const string& value );


    void hget(const string& key, const string& field, CResult &value );

protected:
     /**
     * @brief sendCommand. send a Command to redis-server
     * @param cmd [in]  command will be send.
     */
    void _sendCommand(  const string& cmd );

    int64_t _getNum( const char prefix );

    int64_t _getBulkNum( void );

    int64_t _getMutilBulkNum( void );

    string _replyStatus( void );

    /**
     * @brief _replyOk
     * @return if recv "+OK\r\n" return true,else return false.
     */
    void _replyOk( void );

    int64_t _replyInt( void );

    /**
     * @brief _replyBulk
     * @return
     */
    void _replyBulk(CResult &value );

    bool _replyMultiBulk(VecResult &keys );

    /**
     * @brief _flushRecvBuff  Clear receiving buffer of raw socket.
     */
    void _flushSocketRecvBuff( void );

private:
    DISALLOW_COPY_AND_ASSIGN( CRedisClient );

    CRedisSocket _socket;			///< redis net work class.
    Net::SocketAddress _addr;		///< redis server ip address.
    Timespan _timeout;					///< time out.

    enum
    {
        MAX_LINE_SIZE = 2048,
        MAX_RECV_SIZE = 1024*1024		///< The max number of recved data.( 1M  )
    };
    static const char PREFIX_REPLY_STATUS;
    static const char PREFIX_REPLY_ERR;
    static const char PREFIX_REPLY_INT;
    static const char PREFIX_BULK_REPLY;
    static const char PREFIX_MULTI_BULK_REPLY;
};

#endif // REDIS_H

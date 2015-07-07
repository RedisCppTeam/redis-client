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

using namespace Poco;

/**
 *@brief CRedisClient redis client
 */
class CRedisClient
{
public:

    typedef     vector<std::string> VecString;

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

    bool ping( void );


    //-----------------------------------key---------------------------------------------
    /**
     * @brief keys get all keys matching pattern. 
     * @param pattern [in] The conditions of the matching. 
     * @param keys [out] vector of keys maching pattern 
     * @return The number of keys returned.
     */
    uint64_t keys( const string& pattern, VecString& keys );

    uint64_t del( VecString& keys );

    
    //-----------------------------string method--------------------------------------
    /**
     * @brief set set a string type key = value
     * @param key
     * @param value
     * @warning  could throw Poco::Exception and ProtocolErr exception
     */
    void	set( const string& key, const string& value );

    void get( const string& key, string& value );
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


protected:
     /**
     * @brief sendCommand. send a Command to redis-server
     * @param cmd [in]  command will be send.
     */
    void _sendCommand(  const string& cmd );

    /**
     * @brief _replyLine. Recv a single line  data from redis-server and should be recv "+xx\r\n"
     * "-xx\r\n" ":100\r\n"
     * @return return the line was read, not including "\r\n".
     * @example +xxxx , -xx , :100
     */
    string _replyLine( void );

    uint64_t _getNum( const char prefix );

    uint64_t _getBulkNum( void );

    uint64_t _getMutilBulkNum( void );

    string _replyStatus( void );

    /**
     * @brief _replyOk
     * @return if recv "+OK\r\n" return true,else return false.
     */
    void _replyOk( void );

    uint64_t _replyInt( void );

    /**
     * @brief _replyBulk
     * @return
     */
    string _replyBulk( void );

    uint64_t _replyMultiBulk( VecString& keys );

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
    static const char PREFIX_STATUS_VALUE;
    static const char PREFIX_STATUS_ERR;
    static const char PREFIX_REPLY_INT;
    static const char PREFIX_BULK_REPLY;
    static const char PREFIX_MULTI_BULK_REPLY;
};

#endif // REDIS_H

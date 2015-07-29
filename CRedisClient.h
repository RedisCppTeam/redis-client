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

typedef enum
{
    DEFAULT,	///< -- A default option.
    NX,				///< -- Only set the key if it does not already exist.
    XX				///< -- Only set the key if it already exist.
} SET_OPTION;

/**
 *@brief CRedisClient redis client
 */
class CRedisClient
{
public:

    typedef  	vector<std::string> VecString;

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

    void keys(const std::string &pattern, CResult& result);
    /**
     * @brief keys get all keys matching pattern. 
     * @param pattern [in] The conditions of the matching. 
     * @param keys [out] vector of keys maching pattern 
     * @return The number of keys returned.
     */
    int64_t keys(const string& pattern, VecString &value );


    void del(VecString& keys , CResult &result);

    int64_t del( VecString &keys );
    //-----------------------------string method--------------------------------------


    /**
     * @brief set set a string type key = value
     * @param key
     * @param value
     * @warning  could throw Poco::Exception and ProtocolErr exception
     */
    void set(const string& key, const string& value );



    bool setEX( const string& key, const string& value, long time, SET_OPTION opt=DEFAULT );
    bool setPX( const string& key, const string& value, long time, SET_OPTION opt=DEFAULT );

    bool setNX( const string& key, const string& value );
    bool setXX( const string& key, const string& value );

    void get(const string& key, CResult &result );

    /**
     * @brief get
     * @param key
     * @param value
     * @return true: get value successful, false: key is not exist.
     */
    bool get(const string& key, string &value );
    //------------------------------list method--------------------------------------

    void lpush(const string& key, const VecString& value , CResult &result);

    int64_t lpush(const string& key, const VecString& value );

    /**
     * @brief lpop
     * @param key
     * @param value
     * @return true: successful. false: key is not exit.
     */
    void lpop(const std::string &key, CResult &result);

    bool lpop(const std::string &key, string &value);

    //------------------------------hash method-----------------------------------

    void hset( const string& key, const string& field,const string& value, CResult& result );

    /**
     * @brief hset  insert into a value to hash name is key field is field .
     * @param key  hash name
     * @param field hash field
     * @param value to insert data
     * @return return 1, a new data insert into hash.return 0, a old data update.
     */
    uint8_t hset( const string& key, const string& field, const string&value );



    void hget(const string& key, const string& field, CResult &result );

    /**
     * @brief hget
     * @param key
     * @param field
     * @param value
     * @return true : get value successful,false get value failed.
     */
    bool hget(const string& key, const string& field, std::string &value );

    //--------------------------transtraction method------------------------------

    void watch( const VecString& keys );

    void unwatch( void );

    void multi( void );

    void discard( void );

    bool transactionCmd( const string& cmd, VecString& params );

    void exec(CResult &result );

protected:
     /**
     * @brief sendCommand. send a Command to redis-server
     * @param cmd [in]  command will be send.
     */
    void _sendCommand(  const string& cmd );

    bool _getReply(CResult& result );

    /**
     * @brief _replyBulk
     * @param result
     * @param line
     * @return true: successful false :not exists
     */
    bool _replyBulk(CResult &result, const string& line );

    uint64_t _replyMultiBulk(CResult &result , const std::string &line);

    template <typename T>
    T _valueFromString( const string& data )
    {
        T value ;
        std::istringstream istr( data );
        istr >> value;
        if ( istr.fail() )
        {
            throw ConvertErr( "convert from string to other type value falied" );
        }

        return value;
    }
    /**
     * @brief set
     * @param key
     * @param value
     * @param result
     * @param suffix		"EX","PX"
     * @param time		expire time.
     * @param suffix2	"NX","XX"
     */
    void	set(const string& key, const string& value, CResult& result,const string& suffix="",long time=0,const string suffix2="" ) ;
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

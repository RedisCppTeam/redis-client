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
#include <map>
#include <Poco/Net/StreamSocket.h>
#include "Command.h"
#include "common.h"
#include "RdException.hpp"
#include "CRedisSocket.h"

#include "CResult.h"

using namespace Poco;

typedef enum
{
    DEFAULT,		///< -- A default option.
    NX,				///< -- Only set the key if it does not already exist.
    XX				///< -- Only set the key if it already exist.
} SET_OPTION;



/**
 *@brief CRedisClient redis client
 */
class CRedisClient
{
public:

    typedef  	std::vector<std::string> VecString;
    typedef 	std::map<string,string> MapString;

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
    int64_t keys(const string& pattern, VecString &values );


    void del(VecString& keys , CResult &result);

    int64_t del( VecString &keys );
    //-----------------------------string method--------------------------------------


    /**
     * @brief set set a string type key = value
     * @param key
     * @param value
     * @warning  could throw Poco::Exception and ProtocolErr exception
     */
    void _set(const string& key, const string& value );



    bool setEX( const string& key, const string& value, long time, SET_OPTION opt=DEFAULT ) ;
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
    /**
     * @brief hset  insert into a value to hash name is key field is field .
     * @param key  hash name
     * @param field hash field
     * @param value to insert data
     * @return return 1, a new data insert into hash.return 0, a old data update.
     */
    uint8_t hset( const string& key, const string& field, const string&value );


    /**
     * @brief hget
     * @param key
     * @param field
     * @param value
     * @return true : get value successful,false get value failed.
     */
    bool hget(const string& key, const string& field, std::string &value );


    uint64_t hdel (const string& key, const VecString& fields );


    bool hexists( const string& key, const string& field );


    uint64_t hgetall( const string& key, MapString& pairs );


    void hincrby( const string& key, const string& field, uint64_t increment,CResult& result );

    uint64_t hincrby( const string& key, const string& field ,uint64_t increment );

    float hincrbyfloat( const string& key, const string& field, float increment );


    void hkeys( const string& key, CResult& result );

    uint64_t hkeys( const string& key, VecString& values );


    uint64_t hlen( const string& key );


    void hmget( const string& key, const VecString& fields, CResult& result );


    void hmset( const string& key, const MapString& pairs, CResult& result );

    void hmset( const string& key, const MapString& pairs );


    bool hsetnx( const string& key, const string& field, const string& value );

    uint64_t hvals( const string& key, VecString& values );

    void hscan( const string& key, int64_t cursor, const string& match, uint64_t count, CResult& result );

    /**
     * @brief hscan
     * @param key [in]
     * @param cursor [in] 0: get value from the first. >=1 : get value from the cursor. <0 get value from last time call hscan.
     * @param values [out] value returned.
     * @param match [in] It is possible to only iterate elements matching a given glob-style pattern
     * @param count	[in] Basically with COUNT the user specified the amount of work that should be done at every call in order to retrieve elements from the collection.
     * @return true:There are some value you don't scan.  false: you have scaned all value.
     */
    bool hscan( const string& key, int64_t cursor, MapString& values, const string& match="", uint64_t count=0 );

    //---------------------------Set---------------------------------------------------
    void sadd( const string& key, const VecString& members, CResult& result );

    uint64_t sadd( const string& key, const VecString& members );


    void scard( const string& key, CResult& result );

    uint64_t scard( const string& key );


    void sdiff( const VecString& keys, CResult& result );

    uint64_t sdiff(const VecString& keys, VecString& values );


    void sdiffstore( const string& newKey,const VecString& keys,CResult& result );

    uint64_t sdiffstore(const string& newKey, const VecString& keys);


    void sinter( const VecString& keys, CResult& result );

    uint64_t sinter( const VecString& keys, VecString& values  );


    //---------------------------SortedSet-------------------------------------------


    //--------------------------transtraction method------------------------------

    void watch( const VecString& keys );

    void unwatch( void );

    void multi( void );

    void discard( void );

    bool transactionCmd( const string& cmd, VecString& params );

    void exec(CResult &result );

    //----------------------------HyperLogLog-------------------------------------------


    //----------------------------pub/sub--------------------------------------------------


    //-----------------------------Script-----------------------------------------------------

    //-----------------------------connection----------------------------------------------


    //-----------------------------Server---------------------------------------------------

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

    void _getStringVecFromArry( const CResult::ListCResult& arry, VecString& values );

    void _getStringMapFromArry( const CResult::ListCResult& arry, MapString& pairs );

    /**
     * @brief set
     * @param key
     * @param value
     * @param result
     * @param suffix		"EX","PX"
     * @param time		expire time.
     * @param suffix2	"NX","XX"
     */
    void	_set(const string& key, const string& value, CResult& result,const string& suffix="",long time=0,const string suffix2="" );

    /**
     * @brief _getStatus
     * @param cmd [in] Command you want send.
     * @param data [out] recved from server.
     * @return true:recv data successful. false: recv empty object.
     */
    bool _getStatus(Command &cmd, string &status);
    bool _getInt(Command &cmd, int64_t &number);
    bool _getString(Command &cmd, string &value);
    /**
     * @brief _getArry
     * @param cmd
     * @param result [out] it must arry.
     * @return
     */
    bool _getArry(Command& cmd, CResult& result );
    bool _getArry(Command &cmd, VecString& values );
    bool _getArry(Command &cmd, MapString& pairs );


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

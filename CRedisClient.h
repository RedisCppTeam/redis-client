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
    uint64_t append( const string& key, const string& value );

    uint64_t bitcount( const string& key, int64_t start = 0, int64_t end = -1 ) ;

    uint64_t bitop( const string& operation, const string& destkey, VecString& keys );

    int64_t decr( const string& key );

    int64_t decrby( const string& key, int64_t decrement );

    /**
     * @brief get
     * @param key
     * @param value
     * @return true: get value successful, false: key is not exist.
     */
    bool get(const string& key, string &value );

    uint8_t getbit( const string& key, uint32_t offset );

    void getrange( const string& key, int64_t start, int64_t end, string &value );

    bool getset(const string& key, const string &value, string &oldvalue );

    int64_t incr( const string& key );

    int64_t incrby( const string& key, int64_t increment );

    float incrbyfloat( const string& key, float increment );

    void mget( VecString& keys, CResult& result );

    void mset( CRedisClient::MapString &value );

    uint8_t msetnx( CRedisClient::MapString &value );

    /**
     * @brief set set a string type key = value
     * @param key
     * @param value
     * @warning  could throw Poco::Exception and ProtocolErr exception
     */
    void set(const string& key, const string& value );

    bool setEX( const string& key, const string& value, long time, SET_OPTION opt=DEFAULT ) ;
    bool setPX( const string& key, const string& value, long time, SET_OPTION opt=DEFAULT );

    bool setNX( const string& key, const string& value );
    bool setXX( const string& key, const string& value );

    uint8_t setbit( const string& key, uint32_t offset, const string& value );

    uint64_t setrange( const string& key, uint32_t offset, const string& value );

    uint64_t strlen( const string& key );

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

    /**
     * @brief hscan
     * @param key [in]
     * @param cursor [in] 0: get value from the first. >=1 : get value from the cursor. <0 get value from last time call hscan.
     * @param values [out] value returned.
     * @param match [in] It is possible to only iterate elements matching a given glob-style pattern
     * @param count	[in] Basically with COUNT the user specified the amount of work that should be done at every call in order to retrieve elements from the collection.
     * @return true:There are some value you don't scan.  false: you have scaned all value.
     *
     * eg: get all key between pair_100 and pair_199
     *       	redis.hscan( "testHash", 0, hscanPairs,"pair_1??" );
     *			while ( redis.hscan( "testHash", -1, hscanPairs ,"pair_1??") );
     */
    bool hscan( const string& key, int64_t cursor, MapString& values, const string& match="", uint64_t count=0 );

    //---------------------------Set---------------------------------------------------
    uint64_t sadd( const string& key, const VecString& members );

    uint64_t scard( const string& key );

    uint64_t sdiff(const VecString& keys, VecString& values );

    uint64_t sdiffstore(const string& destKey, const VecString& keys);

    uint64_t sinter( const VecString& keys, VecString& values  );

    uint64_t sinterstore(const string& destKey , const VecString& keys);

    /**
     * @brief sismember
     * @param key
     * @param member
     * @return true : member   ∈ key.  false member ∉ key.
     */
    bool sismember( const string& key, const string& member );

    uint64_t smembers( const string& key,VecString& members );

    /**
     * @brief smove
     * @param source
     * @param dest
     * @param member
     * @return true: move members ok false: move member failed.
     */
    bool smove( const string& source,const string& dest, const string& member );

    bool spop( const string& key, string& member );

    /**
     * @brief srandmember
     * @param key
     * @param member
     * @return true: get a member random is successful. false: key is empty
     */
    bool srandmember( const string& key, string& member );

    /**
     * @brief srandmember
     * @param key
     * @param count		如果 count 为正数，且小于集合基数，那么命令返回一个包含 count 个元素的数组，数组中的元素各不相同。
     * 								如果 count 大于等于集合基数，那么返回整个集合。
     *								如果 count 为负数，那么命令返回一个数组，数组中的元素可能会重复出现多次，而数组的长度为 count 的绝对值。
     * @param members
     * @return the number of fetched members.
     */
    uint64_t srandmember( const string& key, int count,VecString& members );

    uint64_t srem( const string& key, VecString& members );

    uint64_t sunion( const VecString& keys, VecString& members );

    uint64_t sunionstroe( const string& dest,const VecString& keys );

    bool sscan( const string& key, int64_t cursor, VecString& values, const string& match="", uint64_t count=0  );
    //---------------------------SortedSet-------------------------------------------

    uint64_t  zadd(const string& key,const MapString& pairs);

    uint64_t zcard(const string& key);

    uint64_t zcount(const string& key,const string& min,const string& max);

    string zincrby(const string& key,float increment,const string& member);

    uint64_t zrange(const string& key,const int64_t start,const int64_t stop,VecString& reply);
    uint64_t zrange(const string& key,const int64_t start,const int64_t stop,MapString& reply);

    uint64_t zrangebyscore(const string& key,const string& min,const string& max,VecString& reply,int64_t offset=0,int64_t count=0);
    uint64_t zrangebyscore(const string& key,const string& min,const string& max,MapString& reply,int64_t offset=0,int64_t count=0);

    bool zrank(const string& key,const string& member,int64_t& reply);//cunzai nil

    uint64_t zrem(const string& key,VecString& members);

    uint64_t zremrangebyrank(const string& key,const int64_t start,const int64_t stop);

    uint64_t zremrangebyscore(const string& key,const string& min,const string& max);

    uint64_t zrevrange(const string& key,const int64_t start,const int64_t stop,VecString& reply);
    uint64_t zrevrange(const string& key,const int64_t start,const int64_t stop,MapString& reply);

    uint64_t zrevrangebyscore(const string& key,const string& max,const string& min,VecString& reply,int64_t offset=0,int64_t count=0);
    uint64_t zrevrangebyscore(const string& key,const string& max,const string& min,MapString& reply,int64_t offset=0,int64_t count=0);

    uint64_t zrevrank(const string& key,const string& member);

    string zscore(const string& key,const string& member);

    void addAggregate(Command& cmd,int aggregate);
    uint64_t zunionstore (const string& destination,const uint64_t numkeys,const VecString& keys,const VecString& weigets,int aggregate=0);
    uint64_t zunionstore (const string& destination,const uint64_t numkeys,const VecString& keys,int aggregate=0);

    uint64_t zinterstore (const string& destination,const uint64_t numkeys,const VecString& keys,const VecString& weigets,int aggregate=0);
    uint64_t zinterstore (const string& destination,const uint64_t numkeys,const VecString& keys,int aggregate=0);

    bool zscan( const string& key, int64_t cursor, MapString& reply, const string& match="", uint64_t count=0 );

    uint64_t zrangebylex (const string& key,const string& min,const string& max,VecString& reply,int64_t offset=0,int64_t count=0);

    uint64_t zlexcount (const string& key,const string& min,const string& max);

    uint64_t zremrangebylex (const string& key,const string& min,const string& max);
    //--------------------------transtraction method------------------------------

    void watch( const VecString& keys );

    void unwatch( void );

    void multi( void );

    void discard( void );

    bool transactionCmd( const string& cmd, VecString& params );

    void exec(CResult &result );

    //----------------------------HyperLogLog-------------------------------------------


    //----------------------------pub/sub--------------------------------------------------


    //uint64_t psubscribe( VecString& pattern, VecString& value );


    uint64_t publish( const string& channel, const string& message );


    void psubchannels( VecString& pattern, VecString& value );


    void psubnumsub( VecString& channel, CRedisClient::MapString& value );


    uint64_t psubnumpat();


    void punsubscribe( VecString& pattern, CResult& result );

    //void subscribe( VecString& channel, CResult& result );



    void unsubscribe( VecString& channel, CResult& result );


    //-----------------------------Script-----------------------------------------------------

    //-----------------------------connection----------------------------------------------


    //-----------------------------Server---------------------------------------------------

    string bgrewriteaof();

    string bgsave();

    string  clientGetname();

    bool clientKill(const string& ip,const uint32_t port);

    void clientList(CResult& result);

    bool clientSetname (const string& connectionName);

    uint64_t configGet(const string& parameter,VecString& reply);

    void configResetstat();

    bool configRewrite();

    bool configSet(const string& parameter,const string& value);

    uint64_t dbsize();

    string debugObject(const string& key);

    void debugSegfault();

    void flushall();

    void flushdb();

    uint64_t info(VecString& reply);

    uint64_t lastsave();

    void monitor(void* input=NULL,void* output=NULL,void (*p)(string& str,void* in,void* out)=NULL);

    bool save();

    string shutdown();

    void slaveof(const string& host,const string& port);

    void slowlog(const VecString&  subcommand ,CResult& reply);

    void time(string& currentseconds,string& microseconds);

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
    bool _replyBulk(CResult &result, const string& len );

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
    uint64_t _getArry(Command &cmd, VecString& values );
    uint64_t _getArry(Command &cmd, MapString& pairs );


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

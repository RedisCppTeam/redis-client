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
#define REDIS_NIL "nil"
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
	//---------------------------------connection----------------------------------------

	/**
	 * @brief 使用客户端向 Redis 服务器发送一个 PING ，如果服务器运作正常的话，会返回一个 PONG
	 * @param value [out]返回信息 成功为pong
	 * @return 成功返回true，失败抛异常
	 */
	bool ping( string &value );

	/**
	 * @brief 请求服务器关闭与当前客户端的连接。
	 * @return 成功返回true，失败抛异常
	 */
	bool quit( );

	/**
	 * @brief 打印一个特定的信息 message ，测试时使用
	 * @param message [in]打印信息
	 * @param value [out]
	 * @return 成功返回true，失败抛异常
	 */
	bool echo( const string &message , string &value );

	/**
	 * @brief 如果开启了密码保护的话，在每次连接 Redis 服务器之后，就要使用 AUTH 命令解锁，解锁之后才能使用其他 Redis 命令
	 * @param password[in]认证密码
	 * @return 密码匹配时返回true,失败抛异常
	 */
	bool auth( const string &password );

	/**
	 * @brief 切换到指定的数据库，数据库索引号 index 用数字值指定，以 0 作为起始索引值。
	 * @param index [in]数据库索引号
	 * @return 成功返回true，失败抛异常
	 */
	bool select( uint64_t index );

	//---------------------------------hyperloglog----------------------------------------

	/**
	 * @brief 将任意数量的元素添加到指定的 HyperLogLog 里面
	 * @param key[in] hyperlog名
	 * @param element[in] 要添加的元素列表
	 * @return 整数回复：如果HyperLogLog的内部储存被修改了。那么返回1，否则返回 0
	 */
	uint64_t pfadd( const string &key , const VecString &element );

	/**
	 * @brief 当 PFCOUNT 命令作用于单个键时，返回储存在给定键的HyperLogLog的近似基数，当PFCOUNT命令作用于多个键时，返回所有给定 HyperLogLog 的并集的近似基数
	 * @param key[in] list结构，里面每个元素都是hyperlog类型的key
	 * @return 整数回复：给定HyperLogLog 包含的唯一元素的近似数量
	 */
	uint64_t pfcount( const VecString &key );

	/**
	 * @brief 将多个 HyperLogLog 合并（merge）为一个 HyperLogLog ， 合并后的 HyperLogLog 的基数接近于所有输入 HyperLogLog 的可见集合（observed set）的并集。
	 * @param destKey[in] 合并到这个目标结构
	 * @param srcKey[in]	list结构，里面每个元素都是hyperlog类型的key
	 * @return 成功返回true，失败抛异常
	 */
	bool pfmerge( const string &destKey , const VecString &srcKey );
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

	/**
	 * @brief insert a value into a list top
	 * @param key[in]  name of list
	 * @param value[out] to insert data
	 * @return length of list
	 */
	uint64_t lpush( const string& key , const VecString& value );

	/**
	 * @brief insert a value into a list tail
	 * @param key[in]  name of list
	 * @param value[out] to insert data
	 * @return length of list
	 */
	uint64_t rpush( const string& key , const VecString& value );

	/**
	 * @brief 将值 value 插入到列表 key 的表尾，当且仅当 key 存在并且是一个列表。和 RPUSH 命令相反，当 key 不存在时， RPUSHX 命令什么也不做。
	 * @param key[in]  list name
	 * @param value[out] to insert data
	 * @return length of list,if failed then return 0
	 */
	uint64_t rpushx( const string& key , const string& value );

	/**
	 * @brief 将值 value 插入到列表 key 的表头，当且仅当 key 存在并且是一个列表。和 LPUSH 命令相反，当 key 不存在时， LPUSHX 命令什么也不做。
	 * @param key[in] name of list
	 * @param value[out] to insert data
	 * @return length of list,if failed then return 0
	 */
	uint64_t lpushx( const string& key , const string& value );

	/**
	 * @brief remove a data from a list top
	 * @param key[in] name of list
	 * @param value[out] to remove data,if key is not found then value is ""
	 * @return if value is not "" then true else false
	 */
	bool lpop( const string &key , string &value );

	/**
	 * @brief remove a data from a list tail
	 * @param key[in] name of list
	 * @param value[out] to remove data,if key is not found then value is ""
	 * @return if value is not "" then true else false
	 */
	bool rpop( const string &key , string &value );

	/**
	 * @brief 返回列表 key 中，下标为 index 的元素
	 * @param key[in] name of list
	 * @param index[in] index of list
	 * @param value[out] an element of index of list,if key is not found then value is ""
	 * @return if value is not "" then true else false
	 */
	bool lindex( const string &key , uint64_t index , string &value );

	/**
	 * @brief return length of a list
	 * @param key[in] name of list
	 * @return length of list if list is exists,else 0
	 */
	uint64_t llen( const string& key );

	/**
	 * @brief 将值 value 插入到列表 key 当中，位于值 pivot 之前或之后
	 * @param key[in] name of list
	 * @param token[in] before or after
	 * @param value[in] a data to be inserted
	 * @return 如果命令执行成功，返回插入操作完成之后，列表的长度。
	 * @如果没有找到 pivot ，返回 -1 。
	 * @如果 key 不存在或为空列表，返回 0 。
	 */
	uint64_t linsert( const string& key , const string &token , const string &pivot ,
			const string &value );

	/**
	 * @brief 根据参数 count 的值，移除列表中与参数 value 相等的元素。
	 * @param key[in] name of list
	 * @param count[in] 移除元素个数，正数代表从前往后，负数则相反
	 * @param value[in] remove some data that equal to value
	 * @return 被移除元素的数量。因为不存在的 key 被视作空表(empty list)，所以当 key 不存在时， LREM 命令总是返回 0
	 */
	uint64_t lrem( const string &key , uint64_t &count , const string &value );

	/**
	 * @brief 对一个列表进行修剪(trim)，就是说，让列表只保留指定区间内的元素，不在指定区间之内的元素都将被删除。
	 * @param key[in] name of list
	 * @param start[in] start position
	 * @param stop[in] stop position
	 * @return true if successful，else false
	 */
	bool ltrim( const string &key , uint64_t &start , uint64_t &stop );

	/**
	 * @brief 将列表 key 下标为 index 的元素的值设置为 value 。
	 * @param key[in] name of list
	 * @param index[in] index of list
	 * @param value[in] a data to be set
	 * @return true if successful ,else false
	 */
	bool lset( const string &key , uint64_t &index , const string &value );

	/**
	 * @brief 将列表 source 中的最后一个元素(尾元素)弹出，并返回给客户端。将 source 弹出的元素插入到列表 destination ，作为 destination 列表的的头元素。
	 * @param source[in] 原列表名
	 * @param dest[in]   目标列表
	 * @param value[out]  a pop data from source to dest
	 * @return true if successful ,else false.
	 */
	bool rpoplpush( string &source , string &dest , string &value );

	/**
	 * @brief 返回列表 key 中指定区间内的元素，区间以偏移量 start 和 stop 指定。
	 * @param key[in] name of list
	 * @param start[in] start position
	 * @param stop[in]	stop position
	 * @param value[out]	返回区间内的元素列表
	 * @return	列表的长度
	 */
	uint64_t lrange( const string &key , int64_t &start , int64_t &stop , VecString &value );

	/**
	 * @brief lpop的阻塞版本，当给定列表内没有元素弹出的时候，将阻塞，直到等待超时或发现可弹出元素为止。
	 * @param 当给定多个 key 参数时，按参数 key 的先后顺序依次检查各个列表，弹出第一个非空列表的头元素
	 * @param key[in] name of list
	 * @param timeout[in] 超时时间
	 * @param value[out]弹出的元素，map类型，包含列表名和元素值
	 * @return 没元素弹出返回false，否则返回true
	 */
	bool blpop( const VecString &key , uint64_t &timeout , MapString &value );

	/**
	 * @brief rpop的阻塞版本，当给定列表内没有元素弹出的时候，将阻塞，直到等待超时或发现可弹出元素为止。
	 * @param 当给定多个 key 参数时，按参数 key 的先后顺序依次检查各个列表，弹出第一个非空列表的尾部元素。
	 * @param key[in] name of list
	 * @param timeout[in] 超时时间
	 * @param value[out]弹出的元素，map类型，包含列表名和元素值
	 * @return 没元素弹出返回false，否则返回true
	 */
	bool brpop( const VecString &key , uint64_t &timeout , MapString &value );

	/**
	 * @brief BRPOPLPUSH 是 RPOPLPUSH 的阻塞版本，当给定列表 source 不为空时， BRPOPLPUSH 的表现和 RPOPLPUSH 一样。
	 *当列表 source 为空时， BRPOPLPUSH 命令将阻塞连接，直到等待超时，或有另一个客户端对 source 执行 LPUSH 或 RPUSH 命令为止
	 * @param source[in]原始列表
	 * @param dest[in]目标列表
	 * @param timeout[in]超时时间
	 * @param value[out]弹出元素
	 * @return 没元素弹出返回false，否则返回true
	 */
	bool brpoplpush( const string &source , const string &dest , uint64_t &timeout ,
			string &value );

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
      typedef void (*psubscribecallback)(CResult& result, void* pData);

      void psubscribe( VecString& pattern, psubscribecallback callback, void* pData );


      uint64_t publish( const string& channel, const string& message );


      void psubchannels( VecString& pattern, VecString& value );


      void psubnumsub( VecString& channel, CRedisClient::MapString& value );


      uint64_t psubnumpat();


      void punsubscribe( VecString& pattern, CResult& result );

      void subscribe( VecString& channel, psubscribecallback callback, void* pData );



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

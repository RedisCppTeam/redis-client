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

typedef enum
{
    SUM=0,
    MIN,
    MAX

} SORTEDSET_OPTION;


/**
 *@brief CRedisClient redis client
 */
class CRedisClient
{
public:

	typedef std::vector<std::string> VecString;
	typedef std::map<string , string> MapString;

	CRedisClient( );
	~CRedisClient( );

	/**
	 * @brief setAddress set redis-server ipaddress.
	 * @param ip	redis-server ip.
	 * @param port redis-server port.
	 */
	void setAddress( const string& ip , UInt16 port );

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
	string getAddr( );

	/**
	 * @brief setTimeOut  		 Sets the connect timeout,send timeout,recv timeout for the socket.
	 * @param seconds
	 * @param microseconds
	 */
	void setTimeout( long seconds , long microseconds );

	/**
	 * @brief connect to redis-server
	 * @param ip [in] host ip
	 * @param port [in] host port
	 * @warning Will throw an exception when the connection fails.
	 */
	void connect( const string& ip , UInt16 port = 6379 );

	/**
	 * @brief connect to redis-server.
	 * @warning Must set address before.
	 */
	void connect( );

	/**
	 * @brief reconnect redis-server
	 */
	void reconnect( );

	void closeConnect( );
	//---------------------------------common----------------------------------------

	bool ping( void );

	//-----------------------------------key---------------------------------------------

	/**
	 * @brief keys get all keys matching pattern.
	 * @param pattern [in] The conditions of the matching.
	 * @param keys [out] vector of keys maching pattern
	 * @return The number of keys returned.
	 */
	int64_t keys( const string& pattern , VecString &values );

	int64_t del( VecString &keys );
	bool exists( const string& key );

	/**
	 * @brief 作用和 EXPIRE 类似，都用于为 key 设置生存时间。
	 * 不同在于 EXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)。
	 * @param keys [in] name of key
	 * @param timestamp [in] the key will be destroy after this timestamp
	 * @return true for success,当 key 不存在或没办法设置生存时间，返回 false
	 * Example:
	 * redis> SET cache www.google.com
	 * OK
	 * redis> EXPIREAT cache 1355292000     # 这个 key 将在 2012.12.12 过期
	 * (integer) 1
	 * redis> TTL cache
	 * (integer) 45081860
	 */
	bool expireAt( const string& key , const uint64_t& timestamp );

	/**
	 * @brief 作用和 EXPIREAT 类似，都用于为 key 设置生存时间,以毫秒为单位。
	 * 不同在于 pEXPIREAT 命令接受的时间参数是 UNIX 时间戳(unix timestamp)毫秒为单位。
	 * @param keys [in] name of key
	 * @param timestamp [in] the key will be destroy after this timestamp
	 * @return true for success,当 key 不存在或没办法设置生存时间，返回 false
	 */
	bool pExpireAt( const string& key , const uint64_t& timestamp );

	/**
	 * @brief 以秒为单位设定,更新生存时间
	 * @param keys [in] name of key
	 * @param seconds [in] the key will be destroy after this second
	 * @return The true for set or reset success,false for failed.
	 */
	bool expire( const string& key , const uint64_t& seconds );

	/**
	 * @brief 以毫秒为单位设定,更新生存时间
	 * @param keys [in] name of key
	 * @param msec [in] the key will be destroy after this millisecond
	 * @return The true for set or reset success,false for failed.
	 */
	bool pExpire( const string& key , const uint64_t& msec );

	/**
	 * @brief 以秒为单位取得key的剩余生存周期
	 * @param keys [in] The name of key
	 * @return The number of sec before destroyed,-2 no such key,-1 key persist(alive for ever),
	 */
	int64_t ttl( const string& key );

	/**
	 * @brief 以毫秒为单位取得key的剩余生存周期
	 * @param keys [in] The name of key
	 * @return The number of msec before destroyed,-2 no such key,-1 key persist(alive for ever),
	 */
	int64_t pttl( const string& key );

	/**
	 * @brief 移除 key 的生存时间，让 key 重新成为一个『持久的』(persistent) key
	 * @param key [in] the key
	 * @return The true for success,fail for key isn't exists or the key is persist already.
	 */
	bool persist( const string& key );

	/**
	 * @brief move key from current DB to dstDBIndex
	 * @param key [in] the key
	 * @param dstDBIndex [in]  destination DB inex
	 * @return true for success,fals for failed if no such key or both DB have this key already.
	 */
	bool move( const string& key , const int& dstDBIndex );

	enum EobjSubCommand
	{
		REFCOUNT = 0, ENCODING, IDLETIME
	};
	/**
	 * @brief 命令允许从内部察看给定 key 的 Redis 对象
	 * @param key [in] the key
	 * @param subcommand [in]  子命令
	 * 		REFCOUNT <key> 返回给定 key 引用所储存的值的次数。此命令主要用于除错。
	 * 		ENCODING <key> 返回给定 key 锁储存的值所使用的内部表示(representation)。
	 *		IDLETIME <key> 返回给定 key 自储存以来的空闲时间(idle， 没有被读取也没有被写入)，以秒为单位。
	 * @return "nil" for no such key, other string for response from redis-server,int type will be convert to string
	 * @warning Throw ArgmentErr exception when input argument error.
	 */
	string object( const EobjSubCommand& subcommand , const string& key );

	/**
	 * @brief 从当前数据库中随机返回(不删除)一个 key 。
	 * @param  n/a
	 * @return "nil" for DB is empty, a random key was return;
	 */
	string randomKey( );

	/**
	 * @brief rename key as newkey
	 * @param  key [in] old key name
	 * @param  newKey [in] new key name
	 * @return true for success,false for failed;
	 * @warning throwing exception of 'ReplyErr' if no such key
	 */
	bool rename( const string& key , const string& newKey );

	/**
	 * @brief 当newKey不存在时,将key改名为newkey
	 * @param  key [in] old key name
	 * @param  newKey [in] new key name
	 * @return true for success,false for  newkey exists already or rename failed;
	 * @warning throwing exception of 'ReplyErr' if no such key
	 */
	bool renameNx( const string& key , const string& newKey );

	/**
	 * @brief 返回键值排序的结果。
	 * @param flag   [in] 0 小到大排序, 1 从大到小排序
	 * @param  key [in] key name
	 * @param  retVec [out] returned values arry
	 * @return true for success,false for  key is empty or nonexistent;
	 * @warning throwing exception of 'ReplyErr', if /key type isn't list nor set/one of the values isn't digital
	 */
	bool sort( const string& key , VecString& values , const bool& desc = false );

	/**
	 * @brief 返回 key 所储存的值的类型
	 * @param  key [in] key name
	 * @param type [out] key 所储存的值的类型
	 * 	none (key不存在)
	 * 	string (字符串)
	 * 	list (列表)
	 * 	set (集合)
	 * 	zset (有序集)
	 * 	hash (哈希表)
	 *
	 * @return true for success,false for failed
	 * @warning throwing exception of 'ReplyErr',
	 */
	bool type( const string& key , string& type );

	/**
	 * @brief 将 key 原子性地从当前redis-server移动到时目标redis-server指定的数据库上
	 * @param  key [in] key name
	 * @param  host [in] host of server
	 * @param  port [in] port of server
	 * @param  db [in] db No
	 * @param  timeout [in] timeout
	 * @return true for success,false for failed
	 * @warning throwing exception of 'ReplyErr',
	 */
	bool migrate( const string& key , const string& host , const uint16_t& port = 6379 ,
			const uint16_t& db = 0 , const uint16_t& timeout = 3 );

	/**
	 * @brief 基于游标的迭代器（cursor based iterator）： SCAN 命令每次被调用之后， 都会向用户返回一个新的游标， 用户在下次迭代时需要使用这个新游标作为 SCAN 命令的游标参数， 以此来延续之前的迭代过程。
	 * @param values [out] 得到的键名
	 * @param index [in] 迭代开始的位置，第一次为0
	 * @param pattern [in] 键名匹配限制
	 * @param count [in] 每次迭代出来的元素个数
	 * @return >0 下次迭代开始的位置，为0时完成本次遍历，<0失败
	 * @warning 不保证每次执行都返回某个给定数量的元素。
	 */
	int scan( VecString& values , const int& index , const string& pattern = "" ,
			const int& count = 10 );

	/**
	 * @brief 序列化给定 key ，并返回被序列化的值，使用 RESTORE 命令可以将这个值反序列化为 Redis 键
	 * @param key [in] the key
	 * @param retStr [out] return server response data
	 * @return true for success,false for failed(no such key)
	 * @warning the return data stored in retStr is binary data!
	 */
	bool dump( const string& key , string& retStr );

	/**
	 * @brief 序列化给定 key ，并返回被序列化的值，使用 RESTORE 命令可以将这个值反序列化为 Redis 键
	 * @param key [in] the key
	 * @param buf [in] return server response data
	 * @param ttl [in] time to alive for this key
	 * @return true for success,false for failed(no such key)
	 * @warning Throw ReplyErr exception when the key already exists
	 */
	bool restore( const string& key , const string& buf , const int& ttl = 0 );
	//-----------------------------script method--------------------------------------
	/**
	 * @brief 通过内置的 Lua 解释器，对 Lua 脚本进行求值。
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param keysVec [in] 表示在脚本中所用到的那些 Redis 键(key)，这些键名参数可以在 Lua 中通过全局变量 KEYS 数组，用 1 为基址的形式访问( KEYS[1] ， KEYS[2] ，以此类推)。
	 * @param argsVec [in] 附加参数 arg [arg ...] ，可以在 Lua 中通过全局变量 ARGV 数组访问，访问的形式和 KEYS 变量类似( ARGV[1] 、 ARGV[2] ，诸如此类)。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool eval( CResult& values , const string& script , const VecString& keysVec = VecString() ,
			const VecString& argsVec = VecString() );

	/**
	 * @brief 对缓存在服务器中的脚本进行求值
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param keysVec [in] 表示在脚本中所用到的那些 Redis 键(key)，这些键名参数可以在 Lua 中通过全局变量 KEYS 数组，用 1 为基址的形式访问( KEYS[1] ， KEYS[2] ，以此类推)。
	 * @param argsVec [in] 附加参数 arg [arg ...] ，可以在 Lua 中通过全局变量 ARGV 数组访问，访问的形式和 KEYS 变量类似( ARGV[1] 、 ARGV[2] ，诸如此类)。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool evalSha( CResult& values , const string& script , const VecString& keysVec =
			VecString() , const VecString& argsVec = VecString() );

	/**
	 * @brief 将脚本 script 添加到脚本缓存中，但并不立即执行这个脚本
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param values [out] 返回脚本的序列号(校验和)
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool scriptLoad( string& values , const string& script );

	/**
	 * @brief 表示校验和所指定的脚本是否已经被保存在缓存当中
	 * @param script [in] 脚本的序列号(校验和)
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool scriptExists( const string& script );

	/**
	 * @brief 清除所有 Lua 脚本缓存。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
	bool scriptFlush( void );

	/**
	 * @brief kill  scripts currently executing
	 * @return true for success,false for failed(command failed)
	 * @warning Throw ReplyErr exception when No scripts in executing or the script is writing database.
	 */
	bool scriptKill( void );
	//-----------------------------string method--------------------------------------
	uint64_t append( const string& key , const string& value );

	uint64_t bitcount( const string& key , int64_t start = 0 , int64_t end = -1 );

	uint64_t bitop( const string& operation , const string& destkey , VecString& keys );

	int64_t decr( const string& key );

	int64_t decrby( const string& key , int64_t decrement );

	/**
	 * @brief get
	 * @param key
	 * @param value
	 * @return true: get value successful, false: key is not exist.
	 */
	bool get( const string& key , string &value );

	uint8_t getbit( const string& key , uint32_t offset );

	bool getrange( const string& key , int64_t start , int64_t end , string &value );

	bool getset( const string& key , const string &value , string &oldvalue );

	int64_t incr( const string& key );

	int64_t incrby( const string& key , int64_t increment );

	bool incrbyfloat( const string& key , float increment, float& value );

	void mget( VecString& keys , CResult& result );

	void mset( CRedisClient::MapString &value );

	uint8_t msetnx( CRedisClient::MapString &value );

	/**
	 * @brief set set a string type key = value
	 * @param key
	 * @param value
	 * @warning  could throw Poco::Exception and ProtocolErr exception
	 */
	void set( const string& key , const string& value );

	bool setEX( const string& key , const string& value , long time ,
			SET_OPTION opt = DEFAULT );
	bool setPX( const string& key , const string& value , long time ,
			SET_OPTION opt = DEFAULT );

	bool setNX( const string& key , const string& value );
	bool setXX( const string& key , const string& value );

	uint8_t setbit( const string& key , uint32_t offset , const string& value );

	uint64_t setrange( const string& key , uint32_t offset , const string& value );

	uint64_t strlen( const string& key );

	//------------------------------list method--------------------------------------

	void lpush( const string& key , const VecString& value , CResult &result );

	int64_t lpush( const string& key , const VecString& value );

	/**
	 * @brief lpop
	 * @param key
	 * @param value
	 * @return true: successful. false: key is not exit.
	 */
	void lpop( const std::string &key , CResult &result );

	bool lpop( const std::string &key , string &value );

	//------------------------------hash method-----------------------------------
	/**
	 * @brief hset  insert into a value to hash name is key field is field .
	 * @param key  hash name
	 * @param field hash field
	 * @param value to insert data
	 * @return return 1, a new data insert into hash.return 0, a old data update.
	 */
	uint8_t hset( const string& key , const string& field , const string&value );

	/**
	 * @brief hget
	 * @param key
	 * @param field
	 * @param value
	 * @return true : get value successful,false get value failed.
	 */
	bool hget( const string& key , const string& field , std::string &value );

	uint64_t hdel( const string& key , const VecString& fields );

	bool hexists( const string& key , const string& field );

	uint64_t hgetall( const string& key , MapString& pairs );

	void hincrby( const string& key , const string& field , uint64_t increment ,
			CResult& result );

	uint64_t hincrby( const string& key , const string& field , uint64_t increment );

	float hincrbyfloat( const string& key , const string& field , float increment );

	uint64_t hkeys( const string& key , VecString& values );

	uint64_t hlen( const string& key );

	void hmget( const string& key , const VecString& fields , CResult& result );

	void hmset( const string& key , const MapString& pairs , CResult& result );

	void hmset( const string& key , const MapString& pairs );

	bool hsetnx( const string& key , const string& field , const string& value );

	uint64_t hvals( const string& key , VecString& values );

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
	bool hscan( const string& key , int64_t cursor , MapString& values , const string& match =
			"" , uint64_t count = 0 );

	//---------------------------Set---------------------------------------------------
	uint64_t sadd( const string& key , const VecString& members );

	uint64_t scard( const string& key );

	uint64_t sdiff( const VecString& keys , VecString& values );

	uint64_t sdiffstore( const string& destKey , const VecString& keys );

	uint64_t sinter( const VecString& keys , VecString& values );

	uint64_t sinterstore( const string& destKey , const VecString& keys );

	/**
	 * @brief sismember
	 * @param key
	 * @param member
	 * @return true : member   ∈ key.  false member ∉ key.
	 */
	bool sismember( const string& key , const string& member );

	uint64_t smembers( const string& key , VecString& members );

	/**
	 * @brief smove
	 * @param source
	 * @param dest
	 * @param member
	 * @return true: move members ok false: move member failed.
	 */
	bool smove( const string& source , const string& dest , const string& member );

	bool spop( const string& key , string& member );

	/**
	 * @brief srandmember
	 * @param key
	 * @param member
	 * @return true: get a member random is successful. false: key is empty
	 */
	bool srandmember( const string& key , string& member );

	/**
	 * @brief srandmember
	 * @param key
	 * @param count		如果 count 为正数，且小于集合基数，那么命令返回一个包含 count 个元素的数组，数组中的元素各不相同。
	 * 								如果 count 大于等于集合基数，那么返回整个集合。
	 *								如果 count 为负数，那么命令返回一个数组，数组中的元素可能会重复出现多次，而数组的长度为 count 的绝对值。
	 * @param members
	 * @return the number of fetched members.
	 */
	uint64_t srandmember( const string& key , int count , VecString& members );

	uint64_t srem( const string& key , VecString& members );

	uint64_t sunion( const VecString& keys , VecString& members );

	uint64_t sunionstroe( const string& dest , const VecString& keys );

	bool sscan( const string& key , int64_t cursor , VecString& values , const string& match =
			"" , uint64_t count = 0 );
	//---------------------------SortedSet-------------------------------------------


    /**
     * @brief zadd Add one or more members to a sorted set, or update its score if it already exists
     * @param key[in]
     * @param map[in]
     * @return:  The number of elements added to the sorted sets, not including elements already existing for which the score was updated.
     */
    uint64_t  zadd(const string& key,const MapString& map);
    /**
     * @brief zcard Get the number of members in a sorted set
     * @param key[in]
     * @return: number of members in the key.
     */
    uint64_t zcard(const string& key);
    /**
     * @brief zcount Count the members in a sorted set with scores within the given values
     * @param key[in]
     * @param min[in]
     * @param max[in]
     * @return:  the number of elements in the specified score range.
     */
    uint64_t zcount(const string& key,const string& min,const string& max);
    /**
     * @brief zincrby Increment the score of a member in a sorted set
     * @param key[in]
     * @param increment[in]
     * @param member[in]
     * @return: the new score of member (a double precision floating point number).
     */
    double zincrby(const string& key,double increment,const string& member);
    /**
     * @brief zrange Return a range of members in a sorted set, by index.
     * @param key [in]
     * @param start [in]
     * @param stop [in]
     * @param reply  [out] list of elements in the specified range (optionally with their scores, in case the WITHSCORES option is given).
     * @return: size of reply list.
     */
    uint64_t zrange(const string& key,const int64_t start,const int64_t stop,VecString& reply);
    uint64_t zrangeWithscore(const string& key,const int64_t start,const int64_t stop,MapString& reply);
    /**
     * @brief zrangebyscore  Return a range of members in a sorted set, by score
     * @param key [in]
     * @param min [in]
     * @param max [in]
     * @param reply [out] list of elements in the specified score range (optionally with their scores, in case the WITHSCORES option is given).
     * @param offset [in] The optional LIMIT argument can be used to only get a range of the matching elements (similar to SELECT LIMIT offset, count in SQL).
     * @param count [in]
     * @return: size of reply list.
     */
    uint64_t zrangebyscore(const string& key,const string& min,const string& max,VecString& reply,int64_t offset=0,int64_t count=0);
    uint64_t zrangebyscoreWithscore(const string& key,const string& min,const string& max,MapString& reply,int64_t offset=0,int64_t count=0);
    /**
     * @brief zrank  Determine the index of a member in a sorted set
     * @param key[in]
     * @param member[in]
     * @param reply[out]   the rank of member
     * @return: If member does not exist in the sorted set or key does not exist, return false,else return true.

     */
    bool zrank(const string& key,const string& member,int64_t& reply);
    /**
     * @brief zrem Remove one or more members from a sorted set
     * @param key[in]
     * @param members[in]
     * @return:  The number of members removed from the sorted set, not including non existing members.
     */
    uint64_t zrem(const string& key,const VecString& members);
    /**
     * @brief zremrangebyrank Remove all members in a sorted set within the given indexes.
     * @param key[in]
     * @param start[in]
     * @param stop[in]
     * @return: the number of elements removed.
     */
    uint64_t zremrangebyrank(const string& key,const int64_t start,const int64_t stop);
    /**
     * @brief zremrangebyscore Remove all members in a sorted set within the given scores
     * @param key[in]
     * @param min[in]
     * @param max[in]
     * @return: the number of elements removed.
     */
    uint64_t zremrangebyscore(const string& key,const string& min,const string& max);
    /**
     * @brief zrevrange Return a range of members in a sorted set, by index, with scores ordered from high to low.
     * @param key[in]
     * @param start[in]
     * @param stop[in]
     * @param reply[out] list of elements in the specified range (optionally with their scores).
     * @return: size of reply list.
     */
    uint64_t zrevrange(const string& key,const int64_t start,const int64_t stop,VecString& reply);
    uint64_t zrevrangeWithscore(const string& key,const int64_t start,const int64_t stop,MapString& reply);
    /**
     * @brief zrevrangebyscore Return a range of members in a sorted set, by score, with scores ordered from high to low.
     * @param key[in]
     * @param max[in]
     * @param min[in]
     * @param reply[out]  list of elements in the specified score range (optionally with their scores).
     * @param offset[in] The optional LIMIT argument can be used to only get a range of the matching elements (similar to SELECT LIMIT offset, count in SQL).
     * @param count[in]
     * @return: size of reply list.
     */
    uint64_t zrevrangebyscore(const string& key,const string& max,const string& min,VecString& reply,int64_t offset=0,int64_t count=0);
    uint64_t zrevrangebyscoreWithscore(const string& key,const string& max,const string& min,MapString& reply,int64_t offset=0,int64_t count=0);
    /**
     * @brief zrevrank Determine the index of a member in a sorted set, with scores ordered from high to low.
     * @param key[in]
     * @param member[in]
     * @param reply[out]  the rank of member.
     * @return: f member does not exist in the sorted set or key does not exist, return false,else return true.
     */
    bool zrevrank(const string& key,const string& member,int64_t& reply);
    /**
     * @brief zscore  Get the score associated with the given member in a sorted set
     * @param key[in]
     * @param member[in]
     * @param reply[out]  the score of member (a double precision floating point number), represented as string.
     * @return: If member does not exist in the sorted set or key does not exist, return false,else return true.
     */
    bool zscore(const string& key,const string& member,string& reply);

    void addAggregate(Command& cmd,SORTEDSET_OPTION  aggregate);
    /**
     * @brief zunionstore Add multiple sorted sets and store the resulting sorted set in a new key
     * @param destination [in] union of numkeys sorted sets given by the specified keys, and stores the result in destination.
     * @param keys[in]
     * @param weigets [in] it is possible to specify a multiplication factor for each input sorted set.
     * @param aggregate[in] it is possible to specify how the results of the union are aggregated.
     *      SUM:where the score of an element is summed across the inputs where it exists.
     *      MIN: the resulting set will contain the maximum score of an element across the inputs where it exists.
     *      MAX: the resulting set will contain the minimum score of an element across the inputs where it exists.
     * @return:  the number of elements in the resulting sorted set at destination.
     */
    uint64_t zunionstore (const string& destination,const VecString& keys,const VecString& weigets,SORTEDSET_OPTION aggregate=SUM);
    uint64_t zunionstore (const string& destination,const VecString& keys,SORTEDSET_OPTION  aggregate=SUM);
    /**
     * @brief zinterstore Intersect multiple sorted sets and store the resulting sorted set in a new key.
     * @param destination [in] union of numkeys sorted sets given by the specified keys, and stores the result in destination.
     * @param keys[in]
     * @param weigets [in] it is possible to specify a multiplication factor for each input sorted set.
     * @param aggregate[in] it is possible to specify how the results of the union are aggregated.
     *      SUM:where the score of an element is summed across the inputs where it exists.
     *      MIN: the resulting set will contain the maximum score of an element across the inputs where it exists.
     *      MAX: the resulting set will contain the minimum score of an element across the inputs where it exists.
     * @return:  the number of elements in the resulting sorted set at destination.
     */
    uint64_t zinterstore (const string& destination,const VecString& keys,const VecString& weigets,SORTEDSET_OPTION aggregate=SUM);
    uint64_t zinterstore (const string& destination,const VecString& keys,SORTEDSET_OPTION  aggregate=SUM);
    /**
     * @brief zscan Incrementally iterate sorted sets elements and associated scores.
     * @param key [in]
     * @param cursor [in] 0: get value from the first. >=1 : get value from the cursor. <0 get value from last time call hscan.
     * @param reply [out] value returned.
     * @param match [in] It is possible to only iterate elements matching a given glob-style pattern
     * @param count	[in] Basically with COUNT the user specified the amount of work that should be done at every call in order to retrieve elements from the collection.
     * @return true:There are some value you don't scan.  false: you have scaned all value.
     *
     * eg: get all key between pair_100 and pair_199
     *       	redis.zscan( "test", 0, zscanPairs,"pair_1??" );
     *			while ( redis.zscan( "testHash", -1, zscanPairs ,"pair_1??") );
     */
    bool zscan( const string& key, int64_t cursor, MapString& reply, const string& match="", uint64_t count=0 );
    /**
     * @brief zrangebylex Return a range of members in a sorted set, by lexicographical range
     * @param key[in]
     * @param min[in]
     * @param max[in]
     * @param reply[out] list of elements in the specified score range.
     * @param offset[in]
     * @param count[in]
     * @return: size of reply list.
     */
    uint64_t zrangebylex (const string& key,const string& min,const string& max,VecString& reply,int64_t offset=0,int64_t count=0);
    /**
     * @brief zlexcount Count the number of members in a sorted set between a given lexicographical range.
     * @param key[in]
     * @param min[in]
     * @param max[in]
     * @return: the number of elements in the specified score range.
     */
    uint64_t zlexcount (const string& key,const string& min,const string& max);
    /**
     * @brief zremrangebylex Remove all members in a sorted set between the given lexicographical range
     * @param key[in]
     * @param min[in]
     * @param max[in]
     * @return: the number of elements removed.
     */
    uint64_t zremrangebylex (const string& key,const string& min,const string& max);
	//--------------------------transtraction method------------------------------

	void watch( const VecString& keys );

	void unwatch( void );

	void multi( void );

	void discard( void );

	bool transactionCmd( const string& cmd , VecString& params );

	void exec( CResult &result );

	//----------------------------HyperLogLog-------------------------------------------

	//----------------------------pub/sub--------------------------------------------------

	void psubscribe( VecString& pattern , CResult& result );

	uint64_t publish( const string& channel , const string& message );

	uint64_t psubchannels( VecString& value, const VecString& pattern = VecString() );

	uint64_t psubnumsub( CRedisClient::MapString& value, const VecString& channel = VecString() );

	uint64_t psubnumpat( );

	void punsubscribe( CResult& result, const VecString& pattern = VecString() );

	void subscribe( VecString& channel , CResult& result );

	void unsubscribe( CResult& result, const VecString& channel = VecString() );

	//-----------------------------Script-----------------------------------------------------

	//-----------------------------connection----------------------------------------------

	//-----------------------------Server---------------------------------------------------

    uint64_t stringToVecString(string& str,VecString& vec);
    /**
     * @brief bgrewriteaof Instruct Redis to start an Append Only File rewrite process.
     * @return: Feedback information.
     */
    string bgrewriteaof();
    /**
     * @brief bgsave    APPEND key value Append a value to a key.
     * @return: Feedback information.
     */
    string bgsave();
    /**
     * @brief clientGetname Get the current connection name.
     * @return: The connection name, or a null bulk reply if no name is set.
     */
    string  clientGetname();
    /**
     * @brief clientKill Kill a connection of a client.
     * @param ip[in]
     * @param port[in]
     * @return:  if the connection exists and has been closed   return true.else return false.
     */
    bool clientKill(const string& ip,const UInt16 port);
    /**
     * @brief clientList Get the list of client connections.
     * @param[in] reply[out] list of client connections.
     * @return: size of reply list.
     */
    uint64_t clientList(VecString & reply);
    /**
     * @brief clientSetname Set the current connection name.
     * @param[in] connectionName
     * @return: true:success;false:fail.
     */
    bool clientSetname (const string& connectionName);
    /**
     * @brief configGet Get the value of a configuration parameter.
     * @param parameter[in]
     * @param reply[out] command list.
     * @return: size of reply list.
     */
    uint64_t configGet(const string& parameter,VecString& reply);
    /**
     * @brief configResetstat Reset the stats returned by INFO.
     */
    void configResetstat();
    /**
     * @brief configRewrite Rewrite the configuration file with the in memory configuration.
     * @return: true:success;false:fail.
     */
    bool configRewrite();
    /**
     * @brief configSet Set a configuration parameter to the given value.
     * @param parameter[in]
     * @param value[in]
     * @return: true:success;false:fail.
     */
    bool configSet(const string& parameter,const string& value);
    /**
     * @brief dbsize Return the number of keys in the selected database.
     * @return: the number of keys in the selected database.
     */
    uint64_t dbsize();
    /**
     * @brief debugObject Get debugging information about a key.
     * @param key[in]
     * @return: Feedback information.
     */
    string  debugObject(const string& key);
    /**
     * @brief debugSegfault Make the server crash.
     */
    void debugSegfault();
    /**
     * @brief flushall Remove all keys from all databases.
     */
    void flushall();
    /**
     * @brief flushdb Remove all keys from the current database.
     */
    void flushdb();
    /**
     * @brief info Get information and statistics about the server.
     * @param reply[out] as a collection of text lines.
     * @param section[in]
     *    server: General information about the Redis server
     *    clients: Client connections section
     *    memory: Memory consumption related information
     *    persistence: RDB and AOF related information
     *    stats: General statistics
     *    replication: Master/slave replication information
     *    cpu: CPU consumption statistics
     *    commandstats: Redis command statistics
     *    cluster: Redis Cluster section
     *    keyspace: Database related statistics
     *    all: Return all sections
     *    default: Return only the default set of sections
     */
    void info(VecString& reply,const string& section="");
    /**
     * @brief lastsave Get the UNIX time stamp of the last successful save to disk.
     * @return: n UNIX time stamp.
     */
    uint64_t lastsave();
    /**
     * @brief monitor Listen for all requests received by the server in real time.
     * @param timeout[in]
     * @param reply[out] received commands in an infinite flow.
     */
    void monitor(uint64_t timeout, string& reply);
    /**
     * @brief save Synchronously save the dataset to disk.
     * @return: true:success;false:fail.
     */
    bool save();
    /**
     * @brief shutdown Synchronously save the dataset to disk and then shut down the server.
     * @return On success nothing is returned since the server quits and the connection is closed.
     */
    string shutdown();
    /**
     * @brief slaveof Make the server a slave of another instance, or promote it as master.
     * @param host[in]
     * @param port[in]
     */
    void slaveof(const string& host,const UInt16 port);
    /**
     * @brief slowlog Manages the Redis slow queries log.
     * @param subcommand[in]
     * @param reply[out]: Feedback information.
     */
    void slowlog(const VecString&  subcommand ,CResult& reply);
    /**
     * @brief time Return the current server time.
     * @param currentseconds[out]
     * @param microseconds[out]
     */
    void time(string& currentseconds,string& microseconds);

protected:
	/**
	 * @brief sendCommand. send a Command to redis-server
	 * @param cmd [in]  command will be send.
	 */
	void _sendCommand( const string& cmd );

	bool _getReply( CResult& result );

	/**
	 * @brief _replyBulk
	 * @param result
	 * @param line
	 * @return true: successful false :not exists
	 */
	bool _replyBulk( CResult &result , const string& len );

	uint64_t _replyMultiBulk( CResult &result , const std::string &line );

	template< typename T >
	T _valueFromString( const string& data )
	{
		T value;
		std::istringstream istr(data);
		istr >> value;
		if ( istr.fail() )
		{
			throw ConvertErr("convert from string to other type value falied");
		}

		return value;
	}

	void _getStringVecFromArry( const CResult::ListCResult& arry , VecString& values );

	void _getStringMapFromArry( const CResult::ListCResult& arry , MapString& pairs );

	/**
	 * @brief set
	 * @param key
	 * @param value
	 * @param result
	 * @param suffix		"EX","PX"
	 * @param time		expire time.
	 * @param suffix2	"NX","XX"
	 */
	void _set( const string& key , const string& value , CResult& result ,
			const string& suffix = "" , long time = 0 , const string suffix2 = "" );

	/**
	 * @brief _getStatus
	 * @param cmd [in] Command you want send.
	 * @param data [out] recved from server.
	 * @return true:recv data successful. false: recv empty object.
	 */
	bool _getStatus( Command &cmd , string &status );
	bool _getInt( Command &cmd , int64_t &number );
	bool _getString( Command &cmd , string &value );
	/**
	 * @brief _getArry
	 * @param cmd
	 * @param result [out] it must arry.
	 * @return
	 */
	bool _getArry( Command& cmd , CResult& result );
	uint64_t _getArry( Command &cmd , VecString& values );
	uint64_t _getArry( Command &cmd , MapString& pairs );

private:
	DISALLOW_COPY_AND_ASSIGN( CRedisClient );

	CRedisSocket _socket;			///< redis net work class.
	Net::SocketAddress _addr;		///< redis server ip address.
	Timespan _timeout;					///< time out.

	enum
	{
		MAX_LINE_SIZE = 2048, MAX_RECV_SIZE = 1024 * 1024///< The max number of recved data.( 1M  )
	};
	static const char PREFIX_REPLY_STATUS;
	static const char PREFIX_REPLY_ERR;
	static const char PREFIX_REPLY_INT;
	static const char PREFIX_BULK_REPLY;
	static const char PREFIX_MULTI_BULK_REPLY;
};

#endif // REDIS_H

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


    //---------------------------------Hyperloglog----------------------------------------
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
    void sort( const string& key , VecString& values ,bool desc = false );

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
     * @return None
	 * @warning Throw ReplyErr exception
	 */
     void eval(CResult& result , const string& script , const VecString& keysVec = VecString() ,
            const VecString& argsVec = VecString() );

	/**
	 * @brief 对缓存在服务器中的脚本进行求值
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param keysVec [in] 表示在脚本中所用到的那些 Redis 键(key)，这些键名参数可以在 Lua 中通过全局变量 KEYS 数组，用 1 为基址的形式访问( KEYS[1] ， KEYS[2] ，以此类推)。
	 * @param argsVec [in] 附加参数 arg [arg ...] ，可以在 Lua 中通过全局变量 ARGV 数组访问，访问的形式和 KEYS 变量类似( ARGV[1] 、 ARGV[2] ，诸如此类)。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
    void evalSha(CResult& result , const string& script , const VecString& keysVec =
            VecString() , const VecString& argsVec = VecString() );

	/**
	 * @brief 将脚本 script 添加到脚本缓存中，但并不立即执行这个脚本
	 * @param script [in] 一段 Lua 5.1 脚本程序，它会被运行在 Redis 服务器上下文中，这段脚本不必(也不应该)定义为一个 Lua 函数
	 * @param values [out] 返回脚本的序列号(校验和)
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
    void scriptLoad( const string& script, string& values );

	/**
	 * @brief 表示校验和所指定的脚本是否已经被保存在缓存当中
	 * @param script [in] 脚本的序列号(校验和)
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
    uint64_t scriptExists(const string& script , VecString &result);

	/**
	 * @brief 清除所有 Lua 脚本缓存。
	 * @return true for success,false for failed
	 * @warning Throw ReplyErr exception
	 */
    void scriptFlush( void );

	/**
	 * @brief kill  scripts currently executing
	 * @return true for success,false for failed(command failed)
	 * @warning Throw ReplyErr exception when No scripts in executing or the script is writing database.
	 */
    void scriptKill( void );


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
     * @param where[in] before / after
	 * @param value[in] a data to be inserted
	 * @return 如果命令执行成功，返回插入操作完成之后，列表的长度。
	 * @如果没有找到 pivot ，返回 -1 。
	 * @如果 key 不存在或为空列表，返回 0 。
	 */
    int64_t linsert( const string& key , const string &where , const string &pivot ,
			const string &value );

	/**
	 * @brief 根据参数 count 的值，移除列表中与参数 value 相等的元素。
	 * @param key[in] name of list
     * @param count[in]
     *  The ``num`` argument influences the operation in the following ways:
            num > 0: Remove elements equal to value moving from head to tail.
            num < 0: Remove elements equal to value moving from tail to head.
            num = 0: Remove all elements equal to value.
	 * @param value[in] remove some data that equal to value
	 * @return 被移除元素的数量。因为不存在的 key 被视作空表(empty list)，所以当 key 不存在时， LREM 命令总是返回 0
	 */
    uint64_t lrem(const string &key , const string &value , int64_t count=0 );

	/**
     * @brief  Trim the list ``key``, removing all values not within the slice between ``start`` and ``end`。
	 * @param key[in] name of list
     * @param start[in] start position.The first ele is 0, The last ele is -1,The last but not one is -2 ...
     * @param stop[in] stop position. The first ele is 0, The last ele is -1,The last but not one is -2 ...
     * @return return None
     */
    void ltrim(const string &key , int64_t start , int64_t stop );

	/**
     * @brief   "Set ``position`` of list ``key`` to ``value``"
	 * @param key[in] name of list
	 * @param index[in] index of list
	 * @param value[in] a data to be set
     * @return None
	 */
    void lset(const string &key , uint64_t index , const string &value );

	/**
     * @brief    	RPOP a value off of the ``src`` list and atomically LPUSH it
                        on to the ``dst`` list.  Returns the value.
	 * @param source[in] 原列表名
	 * @param dest[in]   目标列表
	 * @param value[out]  a pop data from source to dest
     * @return true successful ,false: source is not exists .
	 */
    bool rpoplpush( const string &source , const string &dest , string &value );

	/**
     * @brief Return a slice of the list ``key`` between position ``start`` and ``end``。
	 * @param key[in] name of list
     * @param start[in] start position. The first ele is 0, The last ele is -1,The last but not one is -2 ...
     * @param stop[in]	stop position. The first ele is 0, The last ele is -1,The last but not one is -2 ...
     * @param value[out]	the slice
     * @return the length of the slice
	 */
    uint64_t lrange(const string &key , int64_t start , int64_t stop , VecString &value );

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
    bool brpoplpush( const string &source , const string &dest , uint64_t timeout ,
			string &value );

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

    int64_t hincrby( const string& key , const string& field , int64_t increment );

    double hincrbyfloat( const string& key , const string& field , float increment );

	uint64_t hkeys( const string& key , VecString& values );

	uint64_t hlen( const string& key );

    void hmget(const string &key, const CRedisClient::VecString &fields, CResult &result);

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

    //---------------------------Set----------------------------------
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

    uint64_t srem( const string& key , const VecString& members );

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

    void transactionCmd(const string& cmd );

    void transactionCmd( const string& cmd , VecString& params );

    /**
     * @brief exec execute transaction.
     * @param result [out] the result of transaction.
     * @return true:transaction excute successful.
     */
    bool exec( CResult &result );

	//----------------------------pub/sub--------------------------------------------------

	void psubscribe( VecString& pattern , CResult& result );

	uint64_t publish( const string& channel , const string& message );

	uint64_t psubchannels( VecString& value, const VecString& pattern = VecString() );

    uint64_t psubnumsub( CRedisClient::MapString& value,const VecString& channel = VecString() );

	uint64_t psubnumpat( );

	void punsubscribe( CResult& result, const VecString& pattern = VecString() );

	void subscribe( VecString& channel , CResult& result );

	void unsubscribe( CResult& result, const VecString& channel = VecString() );

	//-----------------------------Server---------------------------------------------------
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
     * @param: clientName [out] client name.
     * @return: true:client name is existent ,false:client name is inexistent .
     */
   bool   clientGetname(string& clientName);
    /**
     * @brief clientKill Kill a connection of a client.
     * @param ip[in]
     * @param port[in]
     * @return:  if the connection exists and has been closed   return true.else return false.
     */
    void clientKill(const string& ip,const UInt16 port);
    /**
     * @brief clientList Get the list of client connections.
     * @param[in] reply[out] list of client connections.
     * @return: size of reply list.
     */
    uint64_t clientList(VecString & reply);
    /**
     * @brief clientSetname Set the current connection name.
     * @param[in] connectionName
     * @return: None
     */
    void clientSetname (const string& connectionName);
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
     * @return: None
     */
    void configRewrite();
    /**
     * @brief configSet Set a configuration parameter to the given value.
     * @param parameter[in]
     * @param value[in]
     * @return: None
     */
    void configSet(const string& parameter,const string& value);
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
    void monitorStart( void );
    /**
     * @brief save Synchronously save the dataset to disk.
     * @return: true:success;false:fail.
     */

    bool monitorRead( string& value, uint64_t timeout );
    void monitorStop(void);
    void save();
    /**
     * @brief shutdown Synchronously save the dataset to disk and then shut down the server.
     * @return true: successful, false: status is information about fail reason
     */
    bool shutdown(std::string &status);
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

    void _getReply( CResult& result );

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

    void _getResult(Command &cmd, CResult &result);

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
    bool _getArry( Command &cmd , VecString& values, uint64_t& num );
    bool _getArry(Command &cmd , MapString& pairs, uint64_t &num  );

    uint64_t stringToVecString(string& str,VecString& vec);
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

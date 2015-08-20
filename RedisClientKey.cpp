/**
 *
 * @file	RedisClientKey.cpp
 * @brief CRedisClient key 操作的代码实现。
 *
 *
 * @author: 		Stanley Huang
 * @date: 		2015年8月14日
 *
 */

#include "Command.h"
#include "CRedisClient.h"

int64_t CRedisClient::keys( const std::string &pattern , VecString &values )
{
	Command cmd("KEYS");
	cmd << pattern;

	_getArry(cmd, values);
	return values.size();
}

int64_t CRedisClient::del( CRedisClient::VecString &keys )
{

	Command cmd("DEL");

	VecString::const_iterator it = keys.begin();
	VecString::const_iterator end = keys.end();
	for ( ; it != end ; ++it )
	{
		cmd << *it;
	}

	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::exists( const string& key )
{
	Command cmd("EXISTS");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;

}

bool CRedisClient::expireAt( const string& key , const uint64_t& timestamp )
{
	Command cmd("EXPIREAT");
	cmd << key << timestamp;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::pExpireAt( const string& key , const uint64_t& msec_timestamp )
{
	Command cmd("PEXPIREAT");
	cmd << key << msec_timestamp;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::expire( const string& key , const uint64_t& seconds )
{
	Command cmd("EXPIRE");
	cmd << key << seconds;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::pExpire( const string& key , const uint64_t& msec )
{
	Command cmd("PEXPIRE");
	cmd << key << msec;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

int64_t CRedisClient::ttl( const string& key )
{
	Command cmd("TTL");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

int64_t CRedisClient::pttl( const string& key )
{
	Command cmd("PTTL");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::persist( const string& key )
{
	Command cmd("PERSIST");
	cmd << key;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::move( const string& key , const int& dstDBIndex )
{
	Command cmd("MOVE");
	cmd << key << dstDBIndex;
	int64_t num = 0;
	_getInt(cmd, num);
	return num;
}

bool CRedisClient::object( const EobjSubCommand& subcommand , const string& key , string& retStr )
{
	Command cmd("OBJECT");
	int64_t num = 0;
//	string retStr;
	std::stringstream ss;
	switch ( subcommand )
	{
	case REFCOUNT :
		cmd << "REFCOUNT" << key;
		if ( _getInt(cmd, num) )
		{
			ss << num;
			retStr = ss.str();
			return true;
		}
		break;

	case IDLETIME :
		cmd << "IDLETIME" << key;
		if ( _getInt(cmd, num) )
		{
			ss << num;
			retStr = ss.str();
			return true;
		}
		break;

	case ENCODING :
		cmd << "ENCODING" << key;

		if ( _getString(cmd, retStr) )
		{
			return true;
		}
		break;

	}

	return false;
}

bool CRedisClient::randomKey( string& retKey )
{
	Command cmd("RANDOMKEY");

	if ( _getString(cmd, retKey) )
		return true;

	return false;
}

bool CRedisClient::rename( const string& key , const string& newKey )
{
	Command cmd("RENAME");
	string status;
	cmd << key << newKey;

	return _getStatus(cmd, status);
}

bool CRedisClient::renameNx( const string& key , const string& newKey )
{
	Command cmd("RENAMENX");
	int64_t num = 0;
	cmd << key << newKey;

	if ( _getInt(cmd, num) )
	{
		return num;
	}
	return false;
}

bool CRedisClient::sort( const string& key , VecString& values , const bool& desc )
{
	Command cmd("SORT");
	cmd << key;

	if ( desc )
		cmd << "DESC";

	if ( _getArry(cmd, values) )
	{
		return values.size();
	}
	return false;
}

bool CRedisClient::type( const string& key , string& type )
{
	Command cmd("TYPE");
	cmd << key;

	return _getStatus(cmd, type);
}

int CRedisClient::scan( VecString& values , const int& index , const string& pattern ,
		const int& count )
{
	Command cmd("SCAN");
	string val;
	int64_t nextNo;
	CResult arry;

	cmd << index;

	if ( !pattern.empty() )
	{
		DEBUGOUT("PATTERN:", pattern)
		cmd << "MATCH" << pattern;
	}

	if ( count > 0 && count != 10 )
	{
		DEBUGOUT("PATTERN:", pattern)
		cmd << "COUNT" << count;
	}

	if ( !_getArry(cmd, arry) )
		return -1;

	CResult::ListCResult arrList = arry.getArry();
	if ( arrList.size() != 2 )
		return -2;

	CResult::ListCResult::const_iterator it = arrList.begin();

	val = it->getString(); //throw TypeErr
	std::istringstream istr(val);
	istr >> nextNo;

	if ( istr.fail() )
	{
		throw ProtocolErr(val + ": data received is unexpected");
	}
	DEBUGOUT("nextNo", nextNo)

	++it;
	CResult::ListCResult::const_iterator itKeybgein = it->getArry().begin();
	CResult::ListCResult::const_iterator itKeyend = it->getArry().end();

	values.clear();
	while ( itKeybgein != itKeyend )
	{
		val = itKeybgein->getString();
		values.push_back(val);
		itKeybgein++;
	}

	return nextNo;
}

bool CRedisClient::dump( const string& key , string& retStr )
{
	Command cmd("DUMP");
	cmd << key;
	return _getString(cmd, retStr);
}

bool CRedisClient::restore( const string& key , const string& buf , const int& ttl )
{
	Command cmd("RESTORE");
	string status;
	cmd << key << ttl << buf;

	return _getStatus(cmd, status);
}

bool CRedisClient::migrate( const string& key , const string& host , const uint16_t& port ,
		const uint16_t& db , const uint16_t& timeout )
{
	CResult result;
	Command cmd("MIGRATE");
	cmd << host << port << key << db << timeout;

	_socket.clearBuffer();
	_sendCommand(cmd);
	_getReply(result);

	ReplyType type = result.getType();
	if ( REDIS_REPLY_STATUS == type )
	{
		//"+NOKEY" may returned
		if ( result.compare(0, 2, "OK") == 0 || result.compare(0, 2, "ok") == 0 )
			return true;
	}

	throw ReplyErr(result);
	return false;
}

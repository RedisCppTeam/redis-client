/**
 *
 * @file	RdException.hpp
 * @brief CRedisClient 抛出的异常。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */

#ifndef EXCEPTREDIS_H
#define EXCEPTREDIS_H

#include "redisCommon.h"
#include "Exception/Except.h"

namespace Taiji {
namespace Redis {



TAIJI_NEW_EXCEPTION(ExceptRedis, Except)

///< All exception base class is RdException

///<  Do not conform to the requirements of the redis  protocol.
TAIJI_NEW_EXCEPTION(ExceptProtocol, ExceptRedis)
///< Get unexcepted reply

TAIJI_NEW_EXCEPTION(ExceptReply, ExceptRedis)
///< Connection Exception

TAIJI_NEW_EXCEPTION(ExceptConnect, ExceptRedis)
///< Data received exceeded Maximum limit.

TAIJI_NEW_EXCEPTION(ExceptMaximum, ExceptRedis)
///< Convert from string to other type value failed.

TAIJI_NEW_EXCEPTION(ExceptConvert, ExceptRedis)
///<

TAIJI_NEW_EXCEPTION(ExceptType, ExceptRedis)

TAIJI_NEW_EXCEPTION(ExceptHandle, ExceptRedis)



}
}

#endif // EXCEPTREDIS_H






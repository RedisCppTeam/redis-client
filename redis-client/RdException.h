/**
 *
 * @file	RdException.hpp
 * @brief CRedisClient 抛出的异常。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */

#ifndef RDEXCEPTION_H
#define RDEXCEPTION_H

#include "redisCommon.h"
#include "Exception/Exception.h"

namespace Taiji {
namespace Redis {



TAIJI_NEW_EXCEPTION_INCLUDE(ExceptRedis, Exception, 800)

///< All exception base class is RdException

///<  Do not conform to the requirements of the redis  protocol.
TAIJI_NEW_EXCEPTION_INCLUDE(ExceptProtocol, ExceptRedis, 10)
///< Get unexcepted reply

TAIJI_NEW_EXCEPTION_INCLUDE(ExceptReply, ExceptRedis, 20)
///< Connection Exception

TAIJI_NEW_EXCEPTION_INCLUDE(ExceptConnect, ExceptRedis, 30)
///< Data received exceeded Maximum limit.

TAIJI_NEW_EXCEPTION_INCLUDE(ExceptMaximum, ExceptRedis, 40)
///< Convert from string to other type value failed.

TAIJI_NEW_EXCEPTION_INCLUDE(ExceptConvert, ExceptRedis, 50)
///<

TAIJI_NEW_EXCEPTION_INCLUDE(ExceptType, ExceptRedis, 60)

TAIJI_NEW_EXCEPTION_INCLUDE(ExceptHandle, ExceptRedis, 70)



}
}

#endif // RDEXCEPTION_H






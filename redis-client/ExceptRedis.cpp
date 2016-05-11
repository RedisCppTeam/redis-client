#include "ExceptRedis.h"
namespace Taiji {
namespace Redis {



TAIJI_NEW_EXCEPTION_CPP(ExceptRedis, Exception, 800)

///< All exception base class is RdException

///<  Do not conform to the requirements of the redis  protocol.
TAIJI_NEW_EXCEPTION_CPP(ExceptProtocol, ExceptRedis, 10)
///< Get unexcepted reply

TAIJI_NEW_EXCEPTION_CPP(ExceptReply, ExceptRedis, 20)
///< Connection Exception

TAIJI_NEW_EXCEPTION_CPP(ExceptConnect, ExceptRedis, 30)
///< Data received exceeded Maximum limit.

TAIJI_NEW_EXCEPTION_CPP(ExceptMaximum, ExceptRedis, 40)
///< Convert from string to other type value failed.

TAIJI_NEW_EXCEPTION_CPP(ExceptConvert, ExceptRedis, 50)
///<

TAIJI_NEW_EXCEPTION_CPP(ExceptType, ExceptRedis, 60)

TAIJI_NEW_EXCEPTION_CPP(ExceptHandle, ExceptRedis, 70)



}
}

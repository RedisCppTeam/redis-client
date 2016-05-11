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

class ExceptRedis :public std::exception
{
public:
    ExceptRedis( const string& rhs ):
        _err( rhs )
    {
    }

    virtual const char* what( void ) const throw()
    {
        return _err.c_str();
    }
    ~ExceptRedis() throw()
    {

    }

private:
    string _err;
};


#define NEW_EXCEPTION( name ) \
    class name : public ExceptRedis \
    { \
        public: 	\
            name( const string& rhs) \
            : ExceptRedis( rhs ) \
            { \
                    \
            }	\
            \
            ~name( ) throw()\
            { \
                \
            }\
    };

///< All exception base class is RdException

///<  Do not conform to the requirements of the redis  protocol.
NEW_EXCEPTION( ExceptProtocol )
///< Get unexcepted reply
NEW_EXCEPTION( ExceptReply )
///< Connection Exception
NEW_EXCEPTION( ExceptConnect )
///< Data received exceeded Maximum limit.
NEW_EXCEPTION( ExceptMaximum )
///< Convert from string to other type value failed.
NEW_EXCEPTION( ExceptConvert )
///<
NEW_EXCEPTION( ExceptType )

NEW_EXCEPTION( ExceptHandle )
#endif // RDEXCEPTION_H






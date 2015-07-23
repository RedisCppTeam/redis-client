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

#include "common.h"

class RdException :public std::exception
{
public:
    RdException( const string& rhs ):
        _err( rhs )
    {
    }

    virtual const char* what( void ) const throw()
    {
        return _err.c_str();
    }
    ~RdException() throw()
    {

    }

private:
    string _err;
};


#define NEW_EXCEPTION( name ) \
    class name : public RdException \
    { \
        public: 	\
            name( const string& rhs) \
            : RdException( rhs ) \
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
NEW_EXCEPTION( ProtocolErr )
///< Get unexcepted reply
NEW_EXCEPTION( ReplyErr )
///< Connection Exception
NEW_EXCEPTION( ConnectErr )
///< Data received exceeded Maximum limit.
NEW_EXCEPTION( MaximumErr )
///< Convert from string to other type value failed.
NEW_EXCEPTION( ConvertErr )
///<
NEW_EXCEPTION( TypeErr )

#endif // RDEXCEPTION_H






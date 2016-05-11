/**
 *
 * @file	RedisClientPSub.cpp		
 * @brief the Pub/Sub method of the CRedisClient
 * @author: 		yp
 * @date: 		Aug 6, 2015
 * Revision Description: initial version
 */
#include "Command.h"
#include "CRedisClient.h"

namespace Taiji {
namespace Redis {





 void CRedisClient::psubscribe( VecString& pattern, CResult& result)
 {
	Command cmd( "PSUBSCRIBE" );
	VecString::const_iterator it = pattern.begin();
	for ( ; it != pattern.end(); ++it )
	{
		cmd << *it;
	}
	_socket.setReceiveTimeout(0);
	result.clear();
	_getArry( cmd, result );
	while(true)
	{
		result.clear();
		_getReply( result );
	}
 }



 uint64_t CRedisClient::publish( const string& channel, const string& message )
 {
	Command cmd( "PUBLISH" );
	cmd << channel << message;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
 }




 uint64_t CRedisClient::psubchannels( VecString& value, const VecString& pattern )
 {
	Command cmd( "PUBSUB" );
	cmd << "CHANNELS";

	if ( pattern.size() != 0 )
	{
		VecString::const_iterator it = pattern.begin();
		for ( ; it != pattern.end(); ++it )
		{
			cmd << *it ;
		}
	}

    uint64_t num = 0;
    _getArry( cmd, value, num );
    return num;
 }

 uint64_t CRedisClient::psubnumsub(CRedisClient::VecTuple& value ,  const VecString& channel )
 {
	Command cmd( "PUBSUB" );
	cmd << "NUMSUB";
	if ( channel.size() != 0 )
	{
		VecString::const_iterator it = channel.begin();
		for ( ; it != channel.end(); ++it )
		{
			cmd << *it ;
		}
	}

    uint64_t num = 0;
     _getArry( cmd, value,num );
     return num;
 }

 uint64_t CRedisClient::psubnumpat()
 {
	Command cmd( "PUBSUB" );
	cmd << "NUMPAT";
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
 }


 void CRedisClient::punsubscribe( CResult& result, const VecString& pattern )
 {
	_socket.clearBuffer();

	Command cmd( "PUNSUBSCRIBE" );
	if ( pattern.size() != 0 )
	{
		VecString::const_iterator it = pattern.begin();
		for ( ; it != pattern.end(); ++it )
		{
			cmd << *it ;
		}
	}
	_sendCommand( cmd );

	_getReply( result );
 }


 void CRedisClient::subscribe( VecString& channel, CResult& result )
 {
	Command cmd( "SUBSCRIBE" );
	VecString::const_iterator it = channel.begin();
	for ( ; it != channel.end(); ++it )
	{
		cmd << *it ;
	}
	_socket.setReceiveTimeout(0);
	result.clear();
	_getArry( cmd, result );
	while(true)
	{
		result.clear();
		_getReply( result );
	}
 }


 void CRedisClient::unsubscribe( CResult& result, const VecString& channel )
 {
	_socket.clearBuffer();

	Command cmd( "UNSUBSCRIBE" );
	if ( channel.size() != 0 )
	{
		VecString::const_iterator it = channel.begin();
		for ( ; it != channel.end(); ++it )
		{
			cmd << *it ;
		}
	}
	_sendCommand( cmd );

    _getResult( cmd, result );
 }







}
}





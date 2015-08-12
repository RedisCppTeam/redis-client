
#include "Command.h"
#include "CRedisClient.h"

void CRedisClient::watch(const CRedisClient::VecString &keys)
{
    Command cmd( "WATCH" );

    VecString::const_iterator it = keys.begin();
    for ( ; it != keys.end(); ++it )
    {
        cmd << *it ;
    }

    string status;
    _getStatus( cmd, status );

    if ( "OK" != status )
    {
        ProtocolErr( "WATCH recv unexpected data: " + status );
    }
}

void CRedisClient::unwatch()
{
    Command cmd( "UNWATCH" );
    string status;
    _getStatus( cmd, status );

   if ( "OK" != status )
   {
       ProtocolErr( "UNWATCH recv unexpected data: " + status );
   }
 }


void CRedisClient::multi( void )
{
    Command cmd("MULTI");
    string status;
    _getStatus( cmd, status );

    if ( "OK" != status )
    {
        ProtocolErr( "MULTI recv unexpected data: " + status );
    }
}

bool CRedisClient::transactionCmd(const std::string &cmmand,VecString &params)
{
    CResult result;
    _socket.clearBuffer();
    Command cmd( cmmand );

    VecString::const_iterator it = params.begin();
    VecString::const_iterator end = params.end();

    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _sendCommand( cmd );
    _getReply( result );
   if ( result.getType() != REDIS_REPLY_STATUS || result != "QUEUED" )
   {
       return false;
   }else
   {
       return true;
   }
}

//void CRedisClient::discard( void )
//{
//    _socket.clearBuffer();;
//
//    Command cmd( "DISCARD" );
//
//    _sendCommand( cmd );
//    if ( _replyStatus( ) == "OK" )
//    {
//           return;
//    }else
//    {
//            throw ProtocolErr("DISCARD: data recved is not OK");
//    }
//}
//

void CRedisClient::exec( CResult &result )
{
    _socket.clearBuffer();;

    Command cmd( "EXEC" );

    _sendCommand( cmd );
    _getReply( result );
    if ( result.getType() != REDIS_REPLY_ARRAY )
    {
        throw ProtocolErr( "EXEC received uncecepted data" );
    }
}






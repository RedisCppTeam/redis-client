
#include "Command.h"
#include "CRedisClient.h"

//void CRedisClient::watch(const CRedisClient::VecString &keys)
//{
//    _socket.clearBuffer();;
//
//    Command cmd( "WATCH" );
//
//    VecString::const_iterator it = keys.begin();
//    for ( ; it != keys.end(); it++ )
//    {
//        cmd << *it ;
//    }
//
//    _sendCommand( cmd );
//
//    if ( _replyStatus( ) == "OK" )
//    {
//        return;
//    }else
//    {
//        throw ProtocolErr("WATCH: data recved is not OK");
//    }
//}
//
//void CRedisClient::unwatch()
//{
//    _socket.clearBuffer();;
//
//    Command cmd( "UNWATCH" );
//
//    _sendCommand( cmd );
//    if ( _replyStatus( ) == "OK" )
//    {
//           return;
//    }else
//    {
//            throw ProtocolErr("UNWATCH: data recved is not OK");
//    }
//}
//
//
void CRedisClient::multi( void )
{
    CResult result;
    _socket.clearBuffer();

    Command cmd("MULTI");

    _sendCommand( cmd );
    _getReply( result );

    if ( result.getType() == REDIS_REPLY_STATUS && result=="OK" )
    {
           return;
    }else
    {
            throw ProtocolErr("MULTI: data recved is not OK");
    }
}

bool CRedisClient::transactionCmd(const std::string &cmmand,VecString &params)
{
    CResult result;
    _socket.clearBuffer();
    Command cmd( cmmand );

    VecString::const_iterator it = params.begin();

    for ( ; it != params.end(); it++ )
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







#include "Command.h"
#include "CRedisClient.h"
namespace Taiji {
namespace Redis {



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
        throw ExceptProtocol( "WATCH recv unexpected data: " + status );
    }
}

void CRedisClient::unwatch()
{
    Command cmd( "UNWATCH" );
    string status;
    _getStatus( cmd, status );

   if ( "OK" != status )
   {
       throw ExceptProtocol( "UNWATCH recv unexpected data: " + status );
   }
 }


void CRedisClient::multi( void )
{
    Command cmd("MULTI");
    string status;
    _getStatus( cmd, status );

    if ( "OK" != status )
    {
        throw ExceptProtocol( "MULTI recv unexpected data: " + status );
    }
}

void CRedisClient::transactionCmd(const std::string &cmmand )
{
    Command cmd( cmmand );

    string status;
    _getStatus( cmd, status );

    if ( "QUEUED" != status )
    {
        throw ExceptProtocol( "TRANSACTION recv unexpected data:" + status );
    }
}


void CRedisClient::transactionCmd(const std::string &cmmand,  VecString& params )
{
    Command cmd( cmmand );

    VecString::const_iterator it = params.begin();
    VecString::const_iterator end = params.end();

    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    string status;
    _getStatus( cmd, status );

    if ( "QUEUED" != status )
    {
        throw ExceptProtocol( "TRANSACTION recv unexpected data:" + status );
    }
}

void CRedisClient::discard( void )
{
    Command cmd( "DISCARD" );
    string status;
    _getStatus( cmd,status );

    if ( "OK" != status )
    {
            throw ExceptProtocol("DISCARD: data recved is not OK");
    }
}


bool CRedisClient::exec( CResult &result )
{
    Command cmd( "EXEC" );
    return _getArry( cmd, result );
}


}
}



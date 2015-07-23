/**
 *
 * @file	CRedisClient.cpp
 * @brief CRedisClient 类的公共　方法。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */


#include "CRedisClient.h"
#include "Poco/Types.h"


const char CRedisClient:: PREFIX_REPLY_STATUS = '+';
const char CRedisClient:: PREFIX_REPLY_ERR = '-';
const char CRedisClient:: PREFIX_REPLY_INT = ':';
const char CRedisClient:: PREFIX_BULK_REPLY = '$';
const char CRedisClient:: PREFIX_MULTI_BULK_REPLY = '*';

//==============================based method====================================
CRedisClient::CRedisClient()
{
    Timespan timeout( 5 ,0 );
    _timeout = timeout;
}

CRedisClient::~CRedisClient()
{

}

void CRedisClient::setAddress(const string &ip, UInt16 port)
{
    Net::SocketAddress addr( ip, port );
    _addr =  addr;
    return;
}

string CRedisClient::getAddrip()
{
    return _addr.host().toString();
}

string CRedisClient::getAddr()
{
    return _addr.toString();
}



void CRedisClient::setTimeout(long seconds, long microseconds)
{
    Timespan timeout( seconds, microseconds );
    _timeout =  timeout;
}


void CRedisClient::connect( const string &ip, UInt16 port )
{
    setAddress( ip, port );
    _socket.connect( _addr, _timeout );
    _socket.setSendTimeout( _timeout );
    _socket.setReceiveTimeout( _timeout );
}

void CRedisClient::connect()
{
    _socket.connect( _addr,_timeout );
}

void CRedisClient::reconnect()
{
    _socket.shutdown();
    connect();
}

void CRedisClient::closeConnect()
{
	_socket.shutdown();
}

bool CRedisClient::ping()
{
    try
    {
        _socket.clearBuffer();

        Command cmd( "PING" );
        _sendCommand( cmd );
        DEBUGOUT( "send", string( cmd ) );

        string value = _replyStatus();
        if ( value != "PONG" )
        {
            return false;
        }else
        {
            return true;
        }
    }catch ( std::exception& e )
    {
        DEBUGOUT( "Ping catch exception:", e.what() );
       return false;
    }
}



//==============================Core method=======================================

void CRedisClient::_sendCommand( const string &cmd )
{
    const char* sdData = cmd.data();
    size_t sdLen = cmd.length();

    size_t sded = 0;
    int sd = 0;
    do{
        sd = _socket.sendBytes( sdData, sdLen-sded );
        if ( sd < 0 )
        {
            throw ConnectErr("sendByte failed!");
        }
        sded += sd;
        sdData += sd;
    }while( sded < sdLen );
    return ;
}

string CRedisClient::_replyStatus(void)
{
    string ret;
    _socket.readLine( ret );
    DEBUGOUT( "ret", ret );

    if ( ret[0] == PREFIX_REPLY_STATUS )
    {
        return ret.substr(1);
    } else if( ret[0] == PREFIX_REPLY_ERR )
    {    // error
        throw ReplyErr( ret.substr(1) );
    }else
    {
        throw ProtocolErr("unexpected prefix for status reply");
    }
}


void CRedisClient::_replyOk()
{
    string ret = _replyStatus();
    if ( ret != "OK" )
    {
      throw ProtocolErr("excepted \"OK\" response");
    }else
    {
        return;
    }
 }


template <typename T>
T _valueFromString( const string& data )
{
    T value ;
    std::istringstream istr( data );
    istr >> value;
    if ( istr.fail() )
    {
        throw ConvertErr( "convert from string to other type value falied" );
    }

    return value;
}

int64_t CRedisClient::_replyInt()
{
    int64_t num = 0 ;
    string ret;
    _socket.readLine( ret );
    DEBUGOUT( "ret ", ret );
    //successful
    if ( ret[0] == PREFIX_REPLY_INT )
    {
        num = _valueFromString<int64_t>( ret.substr(1) );
    }else if ( ret[0] == PREFIX_REPLY_ERR )
    {
        // error
        throw ReplyErr( ret.substr(1) );
    }else
    {
        throw ProtocolErr("unexpected prefix for status reply");
    }
    return num;
}

int64_t CRedisClient::_getNum( const char prefix )
{
    string num;
    if ( !_socket.readLine( num ) )
    {
        throw ProtocolErr( "readLine error" );
    }

    DEBUGOUT( "num", num );

    if ( num[0] == prefix )
    {
        return _valueFromString<uint64_t>( num.substr(1) );
    }else if ( num[0] == PREFIX_REPLY_ERR )
    {
        throw ReplyErr( num.substr(1) );
    }else
    {
        throw ProtocolErr("unexpected prefix for status reply");
    }
}

int64_t CRedisClient::_getBulkNum()
{
    return _getNum(PREFIX_BULK_REPLY );
}

int64_t CRedisClient::_getMutilBulkNum()
{
    return _getNum( PREFIX_MULTI_BULK_REPLY );
}




uint8_t CRedisClient::_replyBulk( CResult& value )
{
    value.clear();
    int64_t len = _getBulkNum();
    DEBUGOUT( "getBulkNum", len );
    if ( len == -1 )
    {
        return 0;
    }

    _socket.readLine( value );

    if ( value.length() == ( uint32_t)len )
    {
        value.setType( REDIS_REPLY_STRING );
        return 1;
    }else
    {
        value.clear();
        throw ProtocolErr( "invalid bulk reply data; data of unexpected length" );
    }
}




bool CRedisClient::_replyMultiBulk( VecResult& keys )
{
    keys.clear();
    // get the number of rows of data received .
   int64_t num = _getMutilBulkNum();
   
   if ( num < 0 )
   {
       throw ProtocolErr( "keys command, reply -1 is unknow error" );
   }
   CResult key;
   int64_t count = 0;

   for ( int64_t i = 0; i < num; i++ )
   {
       // get the length of  data to the next line.
       count = _getBulkNum();
       if ( count == -1 )
       {
           key.setType( REDIS_REPLY_NIL );
           continue;
       }else
       {
            key.setType( REDIS_REPLY_STRING );
       }
       _socket.readLine( key );
       if ( (uint32_t)count != key.length() )
       {
           throw ProtocolErr( "the length of recveived data is error!" );
       }
       keys.push_back( key );
   }
   return true;
}

//void CRedisClient::_flushSocketRecvBuff(void)
//{
//    struct timeval timeout;
//    timeout.tv_sec = 0;
//    timeout.tv_usec = 0;
//
//    int fd = _socket.impl()->sockfd();
//
//    fd_set         fds;
//    FD_ZERO(&fds);
//    FD_SET( fd, &fds );
//
//    int nRet;
//    char tmp[1024];
//
//    memset(tmp, 0, sizeof(tmp));
//
//    while(1)
//    {           nRet= select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
//               if(nRet== 0)
//                   break;
//               int ret = recv(fd, tmp, 1024,0);
//				if ( ret <=0 )
//					break;
//
//    }
//}






#ifndef CREDISSOCKET_H
#define CREDISSOCKET_H

#include "common.h"
#include <Poco/Net/StreamSocket.h>

using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Net::Socket;

class CRedisSocket: public StreamSocket
{
public:
	CRedisSocket( );
	explicit CRedisSocket( const SocketAddress& address );

	~CRedisSocket( );

	int get( void );

	int peek( void );

	/**
     * @brief readLine: read a line of data.
     * @param data [out]  data recved
	 * @return
	 *
	 */
    bool readLine( string& data );

    /**
     * @brief readN
     * @param n [in] 		the length of the data you want to receive.
     * @param data [out]		date recved
     */
    void readN( const uint64_t n, string &data );
    /**
     * @brief clearBuffer clear buffer of socket
	 */
	void clearBuffer( void );

protected:

	void _allocBuffer( void );
	void _refill( void );
	/**
	 * @brief _flushRecvBuff  Clear receiving buffer of raw socket.
	 */
	void _flushSocketRecvBuff( void );

private:
	DISALLOW_COPY_AND_ASSIGN( CRedisSocket );

	enum
	{
		RECEIVE_BUFFER_SIZE = 1024, EOF_CHAR = -1
	};

	char* _pBuffer;		///< a buffer that  stores received data.
    char* _pNext;			///< a pointer that points to the data to be  read.
    char* _pEnd;			///< the end of the buffer.
};

#endif // CREDISSOCKET_H

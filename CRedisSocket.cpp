#include "CRedisSocket.h"
#include "RdException.hpp"



CRedisSocket::CRedisSocket():
    _pBuffer(0),
    _pNext(0),
    _pEnd(0)
{
    _allocBuffer();
}

CRedisSocket::CRedisSocket(const Poco::Net::SocketAddress &address ):
    StreamSocket(address),
    _pBuffer(0),
    _pNext(0),
    _pEnd(0)
{
    _allocBuffer();
}



CRedisSocket::~CRedisSocket()
{
    delete [] _pBuffer;
}

int CRedisSocket::get()
{
    _refill();
    if (_pNext != _pEnd)
        return std::char_traits<char>::to_int_type(*_pNext++);
    else
        return EOF_CHAR;
}

int CRedisSocket::peek()
{
    _refill();
    if (_pNext != _pEnd)
        return std::char_traits<char>::to_int_type(*_pNext);
    else
        return EOF_CHAR;
}

bool CRedisSocket::readLine( string& line )
{
    line.clear();
    int ch = get();
    //----修复读取一行的内容中只包含'\r'或'\n'的情况----by FanYongTao ,needs review
    while(1)
    {
        if( ch == EOF_CHAR )
        {
            return false;
        }else if( ch == '\r' && peek() == '\n')
        {
            get();
            return true;
        }else
        {
            line += ch;
            ch = get();
        }
    }
}

string CRedisSocket::readN( const int& nConut )
{
	int cnt,insufficientLen;
	string sumStr;

	if ( nConut <= 0 )
		throw ArgmentErr("Invalid [in] arguments!");

	_refill();
	while ( ( cnt = _pEnd - _pNext ) > 0 )
	{
		insufficientLen = nConut-sumStr.length();
		cnt = cnt > insufficientLen ? insufficientLen : cnt;
		sumStr += string(_pNext, cnt);
		_pNext += cnt;

		if ( sumStr.length() >= static_cast<uint32_t>(nConut) )
		{
			_pNext += 2;
			break;
		}

		_refill();
	}

	return sumStr;
}

void CRedisSocket::clearBuffer( void )
{
   _pNext = _pBuffer;
   _pEnd = _pBuffer;
   _flushSocketRecvBuff();
}

//----------------------------------------------protected----------------------------------------------------
void CRedisSocket::_allocBuffer()
{
    _pBuffer = new char [RECEIVE_BUFFER_SIZE];
    _pNext   = _pBuffer;
    _pEnd    = _pBuffer;
}

void CRedisSocket::_refill()
{
    if (_pNext >= _pEnd)
    {
        int n = receiveBytes(_pBuffer, RECEIVE_BUFFER_SIZE);
        if ( n <=0 )
        {
            throw ConnectErr( "socket is disconnect!" );
        }
        if (n > 0)
        {
            _pNext = _pBuffer;
            _pEnd  = _pBuffer + n;
        }
    }
}

void CRedisSocket::_flushSocketRecvBuff()
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int fd = impl()->sockfd();

    fd_set         fds;
    FD_ZERO(&fds);
    FD_SET( fd, &fds );

    int nRet;
    char tmp[1024];

    memset(tmp, 0, sizeof(tmp));

    while(1)
    {
        nRet= ::select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
        if(nRet <= 0)
            break;

        int count = recv(fd, tmp, 1024,0);
        if ( count <= 0 )
            break;
    }
}






#include "CRedisSocket.h"

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
    while (ch != EOF_CHAR && ch != '\r' && ch != '\n')
    {
        line += (char) ch;
        ch = get();
    }
    if (ch == '\r' && peek() == '\n')
        get();
    else if (ch == EOF_CHAR)
        return false;
    return true;
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
    if (_pNext == _pEnd)
    {
        int n = receiveBytes(_pBuffer, RECEIVE_BUFFER_SIZE);
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
    {           nRet= ::select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
               if(nRet <= 0)
                   break;
               recv(fd, tmp, 1024,0);
    }
}






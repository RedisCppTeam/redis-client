/**
 * @file	Command.h
 * @brief 生成 redis 指令的代码。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <sstream>
#include <vector>
#include <Poco/Types.h>
#include "common.h"

using std::stringstream;
using std::vector;

class Command
{
public:
   explicit Command( const string& cmd )
    {
        _make = false;
        _param.push_back( cmd );
    }
    ~Command()
    {

    }

    /**
    * @brief operator <<     add a param
    * @param param
    */
    template <typename T>
    inline Command& operator<<( const T& param )
    {
        std::stringstream str ;
        str << param;
        _param.push_back( str.str() );
        return *this;
    }

    void makeCommand( void )
    {
        if ( _make )
        {
            return;
        }
        _dataString.str("");
        size_t size = _param.size();
        _dataString << '*' << size << _CRLF;
        vector<string>::const_iterator it = _param.begin();
        for ( ; it != _param.end(); ++it )
        {
            _dataString << '$' << it->size() << _CRLF;
            _dataString << *it << _CRLF;
        }
    }

     /**
     * @brief operator string generate a command
     */
    operator string(  )
    {
        makeCommand();
        return _dataString.str();
    }

    size_t getLength( void )
    {
        makeCommand();
        return _dataString.str().length();
    }

    const char* getData( void )
    {
        makeCommand();
        return _dataString.str().data();
    }

    string getCommand( void );
private:
    std::stringstream _dataString;		///< 实现各种类型与 string 的互转。
    std::vector<string> _param;			///< 存放一次交互的参数个数
    static const char* _CRLF;			///< 一行的结束标志
    bool _make;									///< 标记是否已经 makeCommand
};

#endif // COMMAND_H

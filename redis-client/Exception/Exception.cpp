/**
* Copyright (c) 2015, 爱wifi（版权声明）
*
* @file Exception.cpp
* @brief Taiji 的基本异常类
*
*
* @author: yuhaiyang
* @date: 2016/4/11
*/

#include "Exception.h"

namespace Taiji {


//异常编号与异常名字组成的 map


ExceptInfoMap& GetExceptionCode( void )
{
    static ExceptInfoMap mapCode;
    return mapCode;
}


Exception::Exception(const std::string &errInfo) :
    _errInfo(errInfo)
{

}

Exception::~Exception()
{

}

const char *Exception::what() noexcept
{
    return _errInfo.c_str();
}

const std::string Exception::getErrorCodeString() noexcept
{
    std::string errCode;
    std::stringstream ss;
    ss << _errCode;
    ss >> errCode;

    return errCode;
}

int Exception::getErrorCode()
{
    return _errCode;
}

const std::string Exception::getErrInfo()
{
    std::string errCode, errInfo;
    std::stringstream ss;
    ss << _errCode;
    ss >> errCode;
    errInfo = errCode + ":" + _errInfo;

    return errInfo;
}

CExceptionCheck::CExceptionCheck(const std::string &name,
                                 const std::string &parent, int errCode)
{
      parent.size();	//去除编译警告
      ExceptInfoMap& codeMap = __getExceptMap();
      auto it =
           codeMap.insert(ExceptInfoMap::value_type(errCode,name));

    if ( !it.second )
    {
        std::cout <<"Multiply defined ExceptCode，they are"<< it.first->second << " and "<<name<<std::endl;
        exit(1);
    }
}


ExceptInfoMap& CExceptionCheck::__getExceptMap( void )
{
    static ExceptInfoMap mapCode;
    return mapCode;
}


TAIJI_NEW_EXCEPTION_CPP( ExceptProtocal, Exception, 900 )


}


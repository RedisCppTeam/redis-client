/**
 * Copyright (c) 2016, 爱wifi（版权声明）
 *
 * @file	Exception.h
 * @brief 此文件的简单描述。(必填字段)
 *
 * 此文件的详细功能描述。(可选字段)
 * ExceptCode枚举类:只列出几个基类，如果不够用，可根据需要可派生子类
 * Exception类:_inheritNum属性代表继承的次数，当继承次数大于3次时，则_errCode为第三次继承时的相反数
 * @author: 		cj
 * @date: 		2016年1月6日
 *
 * 修订说明:初始版本
 */
#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <sstream>
#include <map>
#include <iostream>

namespace Taiji
{

//异常编号，异常信息 组成的map
typedef std::map<int, std::string> ExceptInfoMap;


class Exception: public std::exception
{
public:
    Exception() = default;
    Exception( const std::string &errInfo );

    ~Exception( ) noexcept;

    virtual const char* what( ) noexcept;

    const std::string getErrorCodeString( ) noexcept;

    int getErrorCode( );

    const std::string getErrInfo( );

protected:
	std::string _errInfo;
	int _errCode = 0;
	int _inheritNum = 0;

private:
    //friend class CExceptionCheck;
};




/**
 * @brief The CExceptionCheck class
 *
 * 在程序启动的时候使用构造函数来检测异常编号是否有重复
 */
class CExceptionCheck
{
public:
    CExceptionCheck( const std::string &name,const std::string& parent, int errCode );
private:
    //static ExceptInfoMap _codeMap;
    static ExceptInfoMap &__getExceptMap();
};

#define TAIJI_NEW_EXCEPTION_INCLUDE( name,parent,code ) \
    class name : public parent \
    { \
        public: 	\
            name( const std::string& pErrInfo) \
            : parent( pErrInfo) \
            { \
                _inheritNum++;\
                if(_inheritNum>3){ \
                    if(_errCode>0) {\
                        _errCode=-_errCode; \
                    } \
                }else{ \
                    _errCode=(int)code+parent::_errCode;\
                } \
            }	\
            \
            ~name( ) throw()\
            { \
                \
            }\
        static CExceptionCheck _check;\
    };

#define TAIJI_NEW_EXCEPTION_CPP( exceptionName, parent,errCode) \
    CExceptionCheck exceptionName::_check( #exceptionName, #parent,errCode );


////////////////////////////////////基本模块异常///////////////////////////////////
//添加基类异常的时候注意不要重复编号
//
/////////////////////////////////////////////////////////////////////////////////
TAIJI_NEW_EXCEPTION_INCLUDE( ExceptProtocal ,Exception,900)






}//namespace Taiji

#endif /* CEXCEPTION_H_ */


/**
 * Copyright (c) 2015, 爱wifi（版权声明）
 *
 * @file	RedisClientScript.cpp		
 * @brief 实现redis客户端Script指令
 *
 *
 * @author: 		Stanley Huang
 * @date: 		Aug 12, 2015
 *
 * 修订说明:初始版本
 */

#include "Command.h"
#include "CRedisClient.h"

void CRedisClient::eval(const string& script , const VecString& keysVec ,
        const VecString& argsVec , CResult& result )
{
	Command cmd("EVAL");
    cmd << script << keysVec.size();

    VecString::const_iterator it = keysVec.begin();
    VecString::const_iterator end = keysVec.end();

    for (  ; it != end; ++it )
	{
        cmd << *it;
	}

    it = argsVec.begin();
    end = argsVec.end();

    for( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getResult( cmd, result );
}

void CRedisClient::eval(const std::string &script, CResult &result)
{
    VecString keysVec;
    VecString argsVec;
    eval( script,keysVec,argsVec,result );
}

void CRedisClient::eval(const std::string &script, const CRedisClient::VecString &keysVec, CResult &result)
{
    VecString argsVec;
    eval( script,keysVec,argsVec,result );
}

void CRedisClient::evalSha(const string& sha , const VecString& keysVec ,
        const VecString& argsVec , CResult& result )
{
	Command cmd("EVALSHA");

    cmd << sha << keysVec.size();

    VecString::const_iterator it = keysVec.begin();
    VecString::const_iterator end = keysVec.end();

    for ( ; it != end; ++it )
	{
        cmd << *it;
    }

    it = argsVec.begin();
    it = argsVec.end();

    for ( ; it != end; ++it )
	{
        cmd << *it;
	}

    _getResult( cmd, result );
}

void CRedisClient::evalSha(const std::string &script, CResult &result)
{
    VecString keysVec;
    VecString argsVec;
    evalSha( script, keysVec, argsVec, result );
}

void CRedisClient::evalSha(const std::string &script,const VecString& keys,CResult &result)
{
    VecString argsVec;
    evalSha( script, keys, argsVec, result );
}

void CRedisClient::scriptLoad(const string& script , string& values )
{
	Command cmd("SCRIPT");

	cmd << "LOAD" << script;
    _getString(cmd, values);
}

void CRedisClient::scriptExists( const VecString& script , VecBool& result )
{
	Command cmd("SCRIPT");
    cmd << "EXISTS";

    VecString::const_iterator sit = script.begin();
    VecString::const_iterator send = script.end();

    for ( ; sit != send; ++sit )
    {
        cmd << *sit;
    }

    CResult res;
    _getArry( cmd, res );

    CResult::ListCResult::const_iterator it = res.getArry().begin();
    CResult::ListCResult::const_iterator end = res.getArry().end();

    for (  ; it != end; ++it )
    {
        result.push_back( it->getInt()) ;
    }

    return;
}

void CRedisClient::scriptFlush( void )
{
	Command cmd("SCRIPT");
	string status;
	cmd << "FLUSH";
    _getStatus(cmd, status);
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}

void CRedisClient::scriptKill()
{
	Command cmd("SCRIPT");
	cmd << "KILL";
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        throw ExceptProtocol( "CONFIG RESETSTAT: data recved is not OK" );
}

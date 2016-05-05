
/**
 * @file	testSet.cpp
 * @brief 测试Set 模块
 *
 * @author: 		yuhaiyang
 * @date: 		2015年8月18日
 *
 */

#include <iostream>
#include "../Command.h"
#include "../CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "../RdException.hpp"
#include "../CResult.h"

using namespace std;


void PrintVector( const string& key,CRedisClient::VecString& values )
{
    CRedisClient::VecString::const_iterator it = values.begin();
    CRedisClient::VecString::const_iterator end = values.end();
    for ( ; it != end; ++it )
    {
        std::cout << key << ": " << *it << std::endl;
    }
}

///////////////////////////////////////////// test set //////////////////////////////////////////
void TestSet( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );

        //--------------------------test sadd-------------------------------
        CRedisClient::VecString members;
        members.push_back( "yuhaiyang" );
        members.push_back( "zhouli" );
        members.push_back( "严兴俊" );
        uint64_t saddNum = redis.sadd( "testSet", members );
        std::cout << "saddNum: " << saddNum << std::endl;
        //-------------------------test scard--------------------------------
        uint64_t scardNum = redis.scard( "testSet" );
        std::cout << "scardNum:" << scardNum << std::endl;
        //-------------------------test sdiff----------------------------------
        std::cout << "=====================================sdiffData==========================" << std::endl;
        CRedisClient::VecString keys;
        keys.push_back("testSet");
        keys.push_back("testSet2");
        CRedisClient::VecString sdiffValues;
        uint64_t sdiffNum = redis.sdiff( keys, sdiffValues );
        std::cout << "sdiffNum: " << sdiffNum << std::endl;

        //-------------------------test sdiffstore----------------------------
        CRedisClient::VecString sdiffstorekeys;
        sdiffstorekeys.push_back("testSet");
        sdiffstorekeys.push_back("testSet2");
        uint64_t sdiffstoreNum = redis.sdiffstore( "diffSetKey",sdiffstorekeys );
        std::cout << "===========================sdiffNum=================================" << std::endl;
        std::cout << "sdiffstoreNum: " << sdiffstoreNum << std::endl;
        //---------------------------test sinter--------------------------------
        CRedisClient::VecString sinterValues;
        CRedisClient::VecString sinterKeys;
        sinterKeys.push_back( "testSet" );
        sinterKeys.push_back( "testSet2" );
        uint64_t sinterNum = redis.sinter( sinterKeys, sinterValues );
        std::cout << "===========================sinterNum================================" << std::endl;
        std::cout << "sinterNum: " << sinterNum << std::endl;
        CRedisClient::VecString::const_iterator sinterIt = sinterValues.begin();
        CRedisClient::VecString::const_iterator sinterEnd = sinterValues.end();
        for ( ; sinterIt != sinterEnd; ++sinterIt )
        {
            std::cout << "sinterIt: " << *sinterIt << std::endl;
        }
        //-----------------------------test sinterstore---------------------------
        CRedisClient::VecString sinterstoreKeys;
        sinterstoreKeys.push_back( "testSet" );
        sinterstoreKeys.push_back( "testSet2" );
        uint64_t sinterstoreNum = redis.sinterstore( "interSetKey", sinterstoreKeys );
        std::cout << "sinterstoreNum: " << sinterstoreNum << std::endl;
        //----------------------------test sismember----------------------------
        bool ret = redis.sismember( "testSet", "yuhaiyang2" );
        if ( ret )
        {
            std::cout << "存在" << std::endl;
        }else
        {
            std::cout << "不存在" << std::endl;
        }

        CRedisClient::VecString membersMembers;
        uint64_t smembersNum = redis.smembers( "testSet", membersMembers );
        std::cout << "smembersNum" << smembersNum << std::endl;
        PrintVector( "members", membersMembers );

        //-----------------------------test smove-------------------------------------
        bool smoveRet = redis.smove( "testSet2", "testSet","duzong");
        if ( smoveRet )
        {
            std::cout << "smove ok" << std::endl;
        }else
        {
            std::cout << "smove failed" << std::endl;
        }

        //-----------------------------test spop--------------------------------------
        string spopMember;
        bool spopRet = redis.spop( "testSet3", spopMember );
        if ( spopRet )
        {
            std::cout << "spop ok" << std::endl;
            std::cout << "spop data:" << spopMember << std::endl;
        }else
        {
            std::cout << "spop faliled" << std::endl;
        }

        //-----------------------------test srandmember-----------------------------
        string srandmember;
        bool srandRet = redis.srandmember( "testSet3", srandmember );
        if ( srandRet )
        {
            std::cout << "srandmember ok" <<std::endl;
            std::cout << "srandmember data:" << srandmember << std::endl;
        }else
        {
            std::cout << "srandmember failed" << std::endl;
        }
        CRedisClient::VecString srandMember;
        redis.srandmember( "testSet2", 20, srandMember );
        PrintVector( "srandmember", srandMember );

        //-----------------------------test srem---------------------------------------------
        CRedisClient::VecString sremMembers;
        sremMembers.push_back( "nihao" );
        sremMembers.push_back( "yuhaiyang" );
        sremMembers.push_back( "zhouli" );
        uint64_t sremNum = redis.srem( "testSet", sremMembers );
        std::cout << "sremNum:" << sremNum << std::endl;
        //-----------------------------test sunion--------------------------------------------
        CRedisClient::VecString sunionMembers;
        CRedisClient::VecString sunionKeys;
        sunionKeys.push_back("testSet");
        sunionKeys.push_back("testSet2");
        redis.sunion( sunionKeys, sunionMembers );
        PrintVector( "sunionmembers", sunionMembers );
        //--------------------------test sunionstore------------------------------------------
        uint64_t sunionstoreNum = redis.sunionstroe( "sunionstoreSet", sunionKeys );
        std::cout << "sunionstoreNum: " << sunionstoreNum << std::endl;
        //--------------------------test sscan--------------------------------------------------

        std::cout << "====================sscan value===================" << std::endl;
        string value = "value_";
        stringstream ss;
        CRedisClient::VecString sscanMembers1;
        for ( int i = 0; i < 100; ++i )
        {
            value = "value_";
            ss.clear();
            ss.str("");
            ss << i;
            value += ss.str();
            sscanMembers1.push_back( value );
        }
        redis.sadd( "testSet4", sscanMembers1 );

        CRedisClient::VecString sscanMembers;
        redis.sscan( "testSet4", 0, sscanMembers,"value_?" );
        while ( redis.sscan( "testSet4", -1, sscanMembers ,"value_?") );

        PrintVector( "sscan", sscanMembers );

    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void Smain( void )
{
    TestSet();
}


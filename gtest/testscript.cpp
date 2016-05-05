#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "RdException.hpp"
#include "CResult.h"
#include <iostream>

using namespace std;

//redis-cli:>EVAL "while true do end" 0
void testScriptKill( )
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);
    redis.scriptKill();
    cout << "scriptFlush ok!" << endl;
}

void testScriptExists( )
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);

    string retString1,retString2;

    redis.scriptLoad( "return 'hello moto'",retString1 );
    redis.scriptLoad( "return 'hello moto2'",retString2 );
    cout << "redis.scriptLoad ok:" << retString1 << endl;
    cout << "redis.scriptLoad ok:" << retString2 << endl;

    CRedisClient::VecString vecString;
    vecString.push_back( retString1 );
    vecString.push_back( retString2 );

    CRedisClient::VecBool result;
    redis.scriptExists(vecString,result);
    CRedisClient::VecBool::const_iterator it = result.begin();
    CRedisClient::VecBool::const_iterator end = result.end();

    for( ; it != end; ++it )
    {
        std::cout << "val: " << *it << std::endl;
    }
}

void testScriptFlush( )
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);
    redis.scriptFlush();
    cout << "scriptFlush ok!" << endl;
}

void testevalSha( )
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);
    CResult result;
    string retString;
    redis.scriptLoad("return 'hello moto'",retString );
    cout << "redis.scriptLoad ok:" << retString << endl;

    redis.evalSha( retString, result );
    cout << "type:" << CResult::getTypeString(result.getType()) << endl;
    cout << "return:" << result << endl;
}


void testEval( void )
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);
    CResult result;
    string script = "return redis.call('set',KEYS[1],ARGV[1])";
    CRedisClient::VecString keys,argv;
    keys.push_back( "foo" );
    argv.push_back("yuhaiyang");

    redis.eval( script, keys ,argv,result );

    std::cout << result << std::endl;
}


void testScript( void )
{
    try
    {
        //testEval();
       // testScriptFlush();
       //testevalSha();
        testScriptExists();
       // testScriptFlush();
    } catch( RdException& e )
    {
        std::cout << endl << "Redis exception !!!!:" << e.what() << std::endl;
    } catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}




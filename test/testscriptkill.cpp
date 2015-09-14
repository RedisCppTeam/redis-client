#include "../Command.h"
#include "../CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "../RdException.hpp"
#include "../CResult.h"
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
    string retString;
    redis.scriptLoad( "return 'hello moto'",retString );
    cout << "redis.scriptLoad ok:" << retString << endl;

    CRedisClient::VecString result;
    redis.scriptExists(retString,result);
    cout << "script exists ok:" << retString << endl;

    result.clear();
    retString += '2';
    redis.scriptExists(retString,result);
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

    redis.evalSha(result, retString);
    cout << "type:" << CResult::getTypeString(result.getType()) << endl;
    cout << "return:" << result << endl;
}





void testScript( void )
{
    try
    {
        testScriptFlush();
        testevalSha();
        testScriptExists();
        testScriptFlush();
    } catch( RdException& e )
    {
        std::cout << endl << "Redis exception !!!!:" << e.what() << std::endl;
    } catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}




/**
 *
 * @file	main.cpp
 * @brief CRedisClient 测试代码。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 * 修订说明:初始版本
 */
#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>

#include "RdException.hpp"

using namespace std;


void TestKey( void )
{
    try
    {
        CRedisClient::VecString value;
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 10 ; i++ )
        {
            //--------------------------keys------------------------------
            redis.keys("test*", value );
            std::vector<string>::const_iterator it = value.begin();

            std::cout << "------------------------start value---------------------------" << std::endl;
            for ( ; it != value.end(); it++ )
            {
               DEBUGOUT( "value", *it );
            }
            std::cout << "----------------------end value----------------------------" << std::endl;

            CRedisClient::VecString keys;
            keys.push_back( "testHash" );
            keys.push_back( "testString" );
            uint64_t num = redis.del( keys );

            std::cout << "del data:" << num << std::endl;

            sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}


void TestList( void )
{
    try
    {
        CRedisClient::VecString value;
        string v0 = "yu";
        string v1 = "hai";
        string v2 = "yang";

        value.push_back( v0 );
        value.push_back( v1 );
        value.push_back( v2 );

        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 10 ; i++ )
        {
            std::cout << "====testList====" << std::endl;
            uint64_t num = redis.lpush( "name", value );
            std::cout << "num" << num << std::endl;
            sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void TestString( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "192.168.10.105", 6379 );

        string value;
        for ( int i = 0; i < 10 ; i++ )
        {

           std::cout << "====testString====" << std::endl;
           //------------------test set---------------------------
           redis.set( "name", "yanxingjun" );

           //------------------test get---------------------------
            redis.get( "testString",value );
            std::cout << "value:" << value << std::endl;

            sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}

void TestHash( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        for ( int i = 0; i < 100000 ; i++ )
        {
         //   std::cout << "====testHash====" << std::endl;
           // uint8_t num = 0;
            redis.hset( "testHash", "name2", "yang" );
            //printf( "num=%u\n", num );
            //sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}


//  写到 TestHash().
int main()
{
    string value;
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );

     redis.hget( "testHash", "dsdas", value ) ;

    //TestList();
    //TestHash();
    //TestKey();
    //TestString();

}




#include "CRedisPool.h"
#include <sys/time.h>




void TestHashEx(CRedisPool* pPool)
{
    try
    {

        for ( int i = 0; i < 10 ; i++ )
        {
        	CRedisClient* pclient = pPool->getConn();
         //   std::cout << "====testHash====" << std::endl;
           // uint8_t num = 0;
        	pclient->hset( "testHash", "name2", "yang" );
        	pPool->pushBackConn(pclient);
            //printf( "num=%u\n", num );
            //sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}




void TestHashsingle( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "192.168.10.179", 6379 );
        for ( int i = 0; i < 100000 ; i++ )
        {
         //   std::cout << "====testHash====" << std::endl;
           // uint8_t num = 0;
            redis.hset( "testHash", "name2", "yang" );
            //printf( "num=%u\n", num );
            //sleep( 1 );
        }
    }catch( RdException& e )
    {
        std::cout << "Redis exception:" << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << "Poco_exception:" << e.what() << std::endl;
    }
}





















#include <Poco/Thread.h>
#include <Poco/Runnable.h>
class WorkRunnable : public Poco::Runnable
{
public:
	WorkRunnable(CRedisPool* redisPool){_redisPool = redisPool;}
	~WorkRunnable(){}
	virtual void run()
	{
		TestHashEx(_redisPool);
	}


private:
	CRedisPool* _redisPool;

};






/*

int main()
{
	Poco::Thread* ttt[10];
	WorkRunnable* kkk[10];
	for(int i=0; i<10; i++)
	{
		ttt[i]= new Poco::Thread();

		kkk[i] = new WorkRunnable(&redisPool);
		ttt[i]->start(*kkk[i]);
	}

	for(int j=0; j<10; j++)
	{
		ttt[j]->join();
		delete ttt[j];
		delete kkk[j];
	}


    CRedisClient* r = redisPool.getConn();
    sleep ( 10 );
    redisPool.pushBackConn( r );
    sleep( 100 );

    return 0;
}*/


#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "ExceptRedis.h"
#include "CResult.h"

using namespace std;
using namespace Taiji::Redis;
typedef VecString VEC;

void *MonitorStop( void *a )
{
    CRedisClient* client = (CRedisClient*)a;
    std::cout << "input Enter to stop monitor!!" << std::endl;
    getchar();
    client->monitorStop();
    return NULL;
}

void TestServerPrint(const string &cmd,uint64_t num)
{
    cout<<cmd<<":"<<num<<endl;
    cout<<endl;
}
void TestServerPrint(const string &cmd,string str)
{
    cout<<cmd<<":"<<str<<endl;
    cout<<endl;
}
void TestServerPrint(const string& cmd,VEC& vec)
{
    cout<<cmd<<":"<<endl;
    VEC::const_iterator it = vec.begin();
    VEC::const_iterator end = vec.end();
    for ( ; it != end; ++it )
    {
        cout<<*it<<endl;
    }
    cout<<endl;
}

void TestSbgrewriteaof(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test bgrewriteaof------" << endl;
    TestServerPrint("redis.bgrewriteaof()",redis.bgrewriteaof());
}
void TestSbgsave(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test bgsave------" << endl;
    TestServerPrint("redis.bgsave()",redis.bgsave());
}
void TestSclientGetname(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test clientGetname------" << endl;
    string cName;

    if ( redis.clientGetname(cName))
        TestServerPrint("redis.clientGetname()",cName);
    else
        std::cout<<"no clientName"<<std::endl;
    redis.clientSetname("");

    if ( redis.clientGetname(cName))
        TestServerPrint("redis.clientGetname()",cName);
    else
        std::cout<<"no clientName"<<std::endl;
}
void TestSclientKill(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test clientKill------" << endl;
    //用 clientList 查看 port ip
    redis.clientKill("127.0.0.1",33831);
}
void TestSclientList(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test clientList------" << endl;
    VEC vec;
    uint64_t num = redis.clientList(vec);
    std::cout << "clientNum:" << num << std::endl;
    TestServerPrint("redis.clientList(vec)",vec);
}
void TestSclientSetname(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );

    cout << "------test clientSetname------" << endl;
    redis.clientSetname("");
    TestServerPrint("redis.clientSetname(qwert)","OK");
}
void TestSconfigGet(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test configGet------" << endl;
    VEC vec;
    uint64_t num = redis.configGet("sssss",vec);
    std::cout << "config get :" << num << std::endl;
    TestServerPrint("redis.configGet(s*,vec);",vec);
}
void TestSconfigResetstat(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test configResetstat-----" << endl;
    redis.configResetstat();
}
void TestSconfigRewrite(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test configRewrite------" << endl;
    redis.configRewrite();
    TestServerPrint("redis.configRewrite()","ok");
}
void TestSconfigSet(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test configSet------" << endl;
    redis.configSet("slowlog-max-lendsa","1024");
    TestServerPrint("redis.configSet(slowlog-max-len,1024)","ok" );
}
void TestSdbsize(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test dbsize------" << endl;
    TestServerPrint("redis.dbsize()",redis.dbsize());
}
void TestSdebugObject(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test debugObject------" << endl;
    TestServerPrint("redis.debugObject(name",redis.debugObject("name"));
}
void TestSdebugSegfault(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test debugSegfault------" << endl;
    redis.debugSegfault();
}
void TestSflushall(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test flushall------" << endl;
    redis.flushall();
}
void TestSflushdb(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test flushdb------" << endl;
    redis.flushdb();
}
void TestSinfo(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test info------" << endl;
    string str;
    VEC vec;
    redis.info(vec,"serverdsads");
    TestServerPrint("redis.info(vec)",vec);
}
void TestSlastsave(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test lastsave------" << endl;
    TestServerPrint("redis.lastsave()",redis.lastsave());
}
void TestSmonitor(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );

    pthread_t tid;
    pthread_create( &tid,NULL,MonitorStop,(void*)&redis );

    cout << "------test monitor-----" << endl;
    string str;
    redis.monitorStart();
   while(redis.monitorRead(str,0))
        TestServerPrint("redis.monitorRead()",str);


   redis.reconnect();
   std::cout << "reconnect!!!!" << std::endl;
   string value;
   redis.ping( value );
   std::cout << "ping: " <<  value << std::endl;
}

void TestSsave(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test save------" << endl;
    redis.save();
    TestServerPrint("redis.save()","ok");
}
void TestSshutdown(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test shutdown------" << endl;
    string info;
    TestServerPrint("redis.shutdown()",redis.shutdown( info ));
    std::cout << "info" << info << std::endl;
}
void TestSslaveof(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test slaveof------" << endl;
    redis.slaveof("127.0.0.1",6367);
}
void TestSslowlog(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test slowlog------" << endl;
    VEC vec;
    vec.push_back("GET");
    CResult res;
    redis.slowlog(vec,res);
    cout<<res<<endl;
}
void TestStime(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test time------" << endl;
    string cur,mic;
    redis.time(cur,mic);
    cout<<"redis.time(cur,mic): "<<"cur="<<cur<<",mic="<<mic<<endl;
}

void TestServerMain(){
    //TestSbgrewriteaof();
    //sleep(1);//重写需要时间，不能立刻执行bgsave
    //TestSbgsave();
    //TestSclientGetname();
    //TestSclientKill();
    //TestSclientList();
    //TestSclientSetname();
    //TestSconfigGet();
    //TestSconfigResetstat();
    //TestSconfigRewrite(); //需要权限
    //TestSconfigSet();
    //TestSdbsize();
    //TestSdebugObject();
    //TestSdebugSegfault(); //执行使redis崩溃
    //TestSslaveof();
    //TestSflushall(); //会碰到修改只读文件的情况
    //TestSflushdb();  //同 flushall
    //TestSinfo();
    //TestSlastsave();
    //TestSmonitor();//调试，永远阻塞
    //TestSsave();//bgsave 执行之后不能再执行save
    //TestSshutdown(); //下线
    TestSslowlog();
    TestStime();
}

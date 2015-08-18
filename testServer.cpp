#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "RdException.hpp"
#include "CResult.h"

using namespace std;

typedef CRedisClient::VecString VEC;

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
    TestServerPrint("redis.clientGetname()",redis.clientGetname());
}
void TestSclientKill(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test clientKill------" << endl;
    //用 clientList 查看 port ip
    redis.clientKill("127.0.0.1",60011);
}
void TestSclientList(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test clientList------" << endl;
    VEC vec;
    redis.clientList(vec);
    TestServerPrint("redis.clientList(vec)",vec);
}
void TestSclientSetname(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );

    cout << "------test clientSetname------" << endl;
    TestServerPrint("redis.clientSetname(qwert)",redis.clientSetname("qwert"));
}
void TestSconfigGet(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test configGet------" << endl;
    VEC vec;
    redis.configGet("s*",vec);
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
    TestServerPrint("redis.configRewrite()",redis.configRewrite());
}
void TestSconfigSet(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test configSet------" << endl;
    TestServerPrint("redis.configSet(slowlog-max-len,1024)",redis.configSet("slowlog-max-len","1024"));
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
    TestServerPrint("redis.debugObject(ZXH_PC",redis.debugObject("ZXH_PC"));
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
    redis.info(vec);
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
    cout << "------test monitor-----" << endl;
    string str;
    redis.monitor(0,str);

}
void TestSsave(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test save------" << endl;
    TestServerPrint("redis.save()",redis.save());
}
void TestSshutdown(){
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout << "------test shutdown------" << endl;
    TestServerPrint("redis.shutdown()",redis.shutdown());
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
    vec.push_back("LEN");
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
void TestServer(){

    TestSbgrewriteaof();

    sleep(1);//重写需要时间，不能立刻执行bgsave
    TestSbgsave();   
    TestSclientGetname();
    //TestSclientKill();

    TestSclientList();

    TestSclientSetname();
    TestSconfigGet();
    TestSconfigResetstat();
    //TestSconfigRewrite(); //需要权限
    TestSconfigSet();
    TestSdbsize();
    //TestSdebugObject();    no key
    //TestSdebugSegfault(); //执行使redis崩溃
    TestSflushall(); //会碰到修改只读文件的情况
    TestSflushdb();  //同 flushall
    TestSinfo();
    TestSlastsave();
    //TestSmonitor();//调试，永远阻塞
    //TestSsave();//bgsave 执行之后不能再执行save
    //TestSshutdown(); //下线
    TestSslaveof();
    TestSslowlog();
    TestStime();


}

#include "test.h"

void TestServer::bgrewriteaof()
{
    string tmp;
    tmp=re.bgrewriteaof();
    print("bgrewriteaof",tmp);
}

void TestServer::bgsave()
{
    string tmp;
    tmp=re.bgsave();
    print("bgsave",tmp);
}

void TestServer::debugSegfault()
{
    re.debugSegfault();
 //   string tmp;
    //tmp=
    //print("debugSegfault",tmp);
}

void TestServer::flushall()
{
    re.flushall();
}

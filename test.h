#ifndef TEST_H
#define TEST_H

#include <iostream>
#include "Command.h"
#include "CRedisClient.h"
#include <stdio.h>
#include <sstream>
#include "RdException.hpp"
#include "CResult.h"
typedef CRedisClient::VecString VEC;
typedef CRedisClient::MapString MAP;
class Test
{

public:
    Test();
    void clearAll();

    void print(const string &cmd,uint64_t num);
    void print(const string &cmd,string& str);
    void print(const string &cmd,VEC vec);
    void print(const string &cmd,MAP map);
    void print(const string cmd ,bool bo);


protected:
    CRedisClient re;
    MAP map;
    VEC vec;
    string key1;
};

class TestSortedSet: public Test
{
public:
    void add();
    void card();
    void count();
    void incrby();
    void range();
    void rangebysocre();
    void rank();
    void rem();
    void remrangebyrank();
    void remrangebyscore();
    void revrange();
    void revrangebyscore();
    void revrank();
    void score();
    void unionstore();
    void interstore();
    void scan();
    void rangebylex();
    void lexcount();
    void remrangebylex();


};

class TestServer: public Test
{
public:
    void bgrewriteaof();
    void bgsave();

    void debugSegfault();
    void flushall();
};

#endif // TEST_H

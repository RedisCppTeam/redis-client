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



#include "test.h"
using namespace std;




int main()
{
    //TestSortedSet tss;
    TestServer tsv;
    //tss.clearAll();
    //tss.add();
   // tss.card();
   // tss.range();
   // tss.count();
   // tss.incrby();
    //tss.rangebysocre();
    //tss.revrange();

    /***********************************/

   // tsv.bgrewriteaof();
    //tsv.bgsave();
   // tsv.debugSegfault();
    tsv.flushall();
}




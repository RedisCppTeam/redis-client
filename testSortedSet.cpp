#include "test.h"
using namespace std;

void TestSortedSet::add()
{
    string score,member;
   std::stringstream ss;
   int i;
    for ( i = 10; i < 100; i++ )
  {

        member="member_";
        ss.str("");

        ss << i;
       score= ss.str();
        member += ss.str();
        map.insert( CRedisClient::MapString::value_type(score,member) );
   }
   print("sadd",re.zadd(key1,map));

}

void TestSortedSet::card()
{
    print("zcard",re.zcard(key1));
}


void TestSortedSet::count()
{
    print( "zcount",re.zcount(key1,"35","98") );

}

void TestSortedSet::incrby()
{
    string tmp;
    //tmp=re.zincrby("hash",1,"a");
    tmp=re.zincrby(key1,1,"member_45");
    print("zincrby",tmp);

}
void TestSortedSet::range()
{
    re.zrange(key1,1,10,vec);
    print("zrangeVec",vec);

    re.zrange(key1,1,10,map);
    print("zrangeMap",map);
}

void TestSortedSet::rangebysocre()
{
    map.clear();
    re.zrangebyscore(key1,"20","50",map,0,10);
    print("zrangebyscore",map);
}

void TestSortedSet::rank()
{

}

void TestSortedSet::remrangebyrank()
{

    print("zremrangebyrank",re.zremrangebyrank(key1,1,10));
}

void TestSortedSet::remrangebyscore()
{

    //print("zremrangebyrank",re.zremrangebyrank(key1,"1","40"));
}

void TestSortedSet::revrange()
{
    vec.clear();
    re.zrevrange(key1,1,0,vec);
    print("zrevrange",vec);
}

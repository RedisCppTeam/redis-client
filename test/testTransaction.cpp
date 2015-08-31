/**
 * @file	testSet.cpp
 * @brief 测试 Transaction 模块
 *
 * @author: 		yuhaiyang
 * @date: 		2015年8月18日
 *
 */




/*
//////////////////////////////////////// test transaction////////////////////////////////////////
int main()
{
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    CResult result;
    redis.multi();
    CRedisClient::VecString params;
    params.push_back( "*" );
    redis.transactionCmd("keys", params );
    params.clear();
    params.push_back("asd");
    redis.transactionCmd("get", params );
    redis.exec( result );

    std::cout << result << std::endl;
}
*/



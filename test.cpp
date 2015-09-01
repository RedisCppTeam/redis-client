/**
 *
 * @file	main.cpp
 * @brief CRedisClient 测试代码入口文件。
 *
 * @author: 		yuhaiyang
 * @date: 		2015年6月14日
 *
 * 修订说明:初始版本
 */

void TestString( void );
void TestPSub( void );
void Hmain( void );
void Smain( void );
void TestSortedSet(void);
void TestServer(void);
void TranSactionMain( void );



int main()
{
    //Hmain();
    //Smain();
    //TestSortedSet();
    //TestServer();
    TranSactionMain();
    return 0;
}




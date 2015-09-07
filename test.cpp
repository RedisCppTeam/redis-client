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
void TestSortedSet( void );
void TestServer( void );
void TranSactionMain( void );
void Lmain( void );

void testKeyMain( );
void testWithOneRedis( );
void testScriptMain( );
void Pmain(void);
void Cmain(void);

int main( )
{
	testKeyMain();
//	testWithOneRedis();
//	testScriptMain();
//	CRedisClient redis;
//	redis.connect("127.0.0.1", 6379);
//	VEC vec;
//	testReadN();
//	TestHash();
//	TestHash2();
//	TestList();
//	TestKey();
//	TestString();
//	TestSet();
//	TestPSub();
//	TestSortedSet();
//	TestPSub();
//
//	Hmain();
//	Smain();
//	TestSortedSet();
//	TestServer();
//	TranSactionMain();
//	Lmain();
//	Pmain();
//	Cmain();
	return 0;
}

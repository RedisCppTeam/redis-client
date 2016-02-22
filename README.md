# CRedisClient
This is a client written with cpp for redis-server.Its socket module depends on poco c++ library.
All commands are supported.

##ubuntu: 
sudo apt-get install libpoconet9 libpocofoundation9



##eg:
### a simple eg
```cpp
void TestZincrby()
{
    CRedisClient redis;
    redis.connect( "127.0.0.1", 6379 );
    cout<<"----------zincrby----------"<<endl;
    double dbl;
    dbl=redis.zincrby("SortedSet",1.1,"member_45");
    cout<<"redis.zincrby(SortedSet,1,member_45)="<<dbl<<endl;
}
```

### Transaction
```cpp
void TranSactionMain( void )
{
    try
    {
        CRedisClient redis;
        redis.connect( "127.0.0.1", 6379 );
        redis.set( "yuhaiyang", "nan" );
        redis.set( "yuhaiyang2", "nan2" );
        CResult result;

        //------------------------test multi, transactionCmd, exec---------------------
        DEBUGOUT("test multi, transactionCmd, and exec","");
        redis.multi();
        CRedisClient::VecString params;
        params.push_back( "*" );
        redis.transactionCmd( "keys", params );

        params.clear();
        params.push_back("yuhaiyang");
        redis.transactionCmd("get", params );
        redis.exec( result );

        std::cout << result << std::endl;

        //---------------------------Watch---------------------------------------
        result.clear();
        params.clear();
        CRedisClient::VecString keys;
        keys.push_back( "yuhaiyang" );
        keys.push_back("yuhaiyang2");
        redis.watch( keys );

        //redis.unwatch();
        redis.multi();
        params.push_back("yuhaiyang");
        params.push_back("yuhaiyang2");
        redis.transactionCmd("mget",params);
        //you can change the key which is watched.
        getchar();
        if ( !redis.exec( result ) )
        {
            std::cout << "transaction exec false" << std::endl;
        }else
        {
            std::cout << "transaction exec successful" << std::endl;
        }
        std::cout << result << std::endl;
    }catch ( std::exception& e )
    {
        std::cout << e.what() << std::endl;
    }
}
```

### CRedisPool
```cpp

int main()
{
    try
    {
        CRedisPool redis;
        redis.init( "127.0.0.1", 6379,"");

        CRedisPool::Handle con = redis.getConn( 10 );
        con->set("name","haiyang");
    }catch( RdException& e )
    {
        std::cout << e.what() << std::endl;
    }catch( Poco::Exception& e )
    {
        std::cout << e.what() << std::endl;
    }
}

```
### TODO:


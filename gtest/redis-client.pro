TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
        -lPocoFoundation \
        -lPocoNet	\
        -lgtest \
        -lgtest_main \
        -lpthread \

INCLUDEPATH += \
    ../redis-client \
    src

HEADERS += \
    src/CTestSortedSet.h \
    ../redis-client/Exception/Exception.h \
    ../redis-client/Command.h \
    ../redis-client/CRedisClient.h \
    ../redis-client/CRedisPool.h \
    ../redis-client/CRedisSocket.h \
    ../redis-client/CResult.h \
    ../redis-client/ExceptRedis.h \
    ../redis-client/redisCommon.h \
    src/CTestHash.h \
    src/CTestList.h \
    src/CTestSet.h \
    src/CTestKey.h \
    src/CTestString.h \
    src/CTestConnect.h

SOURCES += \
    src/CTestSortedSet.cpp \
    src/testHash.cpp \
    src/testHyperLogLog.cpp \
    src/testKey.cpp \
    src/testList.cpp \
    src/testPool.cpp \
    src/testPSub.cpp \
    src/testscript.cpp \
    src/testServer.cpp \
    src/testSet.cpp \
    src/testString.cpp \
    src/testTransaction.cpp \
    ../redis-client/Exception/Exception.cpp \
    ../redis-client/Command.cpp \
    ../redis-client/CRedisClient.cpp \
    ../redis-client/CRedisPool.cpp \
    ../redis-client/CRedisSocket.cpp \
    ../redis-client/CResult.cpp \
    ../redis-client/ExceptRedis.cpp \
    ../redis-client/RedisClientConnection.cpp \
    ../redis-client/RedisClientHash.cpp \
    ../redis-client/RedisClientHyperLogLog.cpp \
    ../redis-client/RedisClientKey.cpp \
    ../redis-client/RedisClientList.cpp \
    ../redis-client/RedisClientPSub.cpp \
    ../redis-client/RedisClientScript.cpp \
    ../redis-client/RedisClientServer.cpp \
    ../redis-client/RedisClientSet.cpp \
    ../redis-client/RedisClientSortedSet.cpp \
    ../redis-client/RedisClientString.cpp \
    ../redis-client/RedisTransaction.cpp \
    src/CTestHash.cpp \
    src/CTestList.cpp \
    src/CTestSet.cpp \
    src/CTestKey.cpp \
    src/CTestString.cpp \
    src/CTestConnect.cpp



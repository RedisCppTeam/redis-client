TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
        -lPocoFoundation \
        -lPocoNet	\
        -lpthread

INCLUDEPATH += \
    ../redis-client

HEADERS += \
    ../redis-client/Command.h \
    ../redis-client/common.h \
    ../redis-client/CRedisClient.h \
    ../redis-client/CRedisPool.h \
    ../redis-client/CRedisSocket.h \
    ../redis-client/CResult.h \
    ../redis-client/RdException.hpp

SOURCES += \
    testConnection.cpp \
    testHash.cpp \
    testHyperLogLog.cpp \
    testKey.cpp \
    testList.cpp \
    testPool.cpp \
    testPSub.cpp \
    testscript.cpp \
    testServer.cpp \
    testSet.cpp \
    testSortedSet.cpp \
    testString.cpp \
    testTransaction.cpp \
    ../redis-client/Command.cpp \
    ../redis-client/CRedisClient.cpp \
    ../redis-client/CRedisPool.cpp \
    ../redis-client/CRedisSocket.cpp \
    ../redis-client/CResult.cpp \
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
    ../redis-client/test.cpp




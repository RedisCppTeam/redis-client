#-------------------------------------------------
#
# Project created by QtCreator 2016-05-16T11:14:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = redisclient
TEMPLATE = lib

DEFINES += REDISCLIENT_LIBRARY

CONFIG += c++11

unix {
    target.path = /usr/local/lib/redis
    INSTALLS += target

    head.files += redis-client/*.h
    head.path = /usr/local/include/redis
    INSTALLS += head

    headexcept.files += redis-client/Exception/Except.h
    headexcept.path = /usr/local/include/redis/Exception
    INSTALLS += headexcept
}

HEADERS += \
    redis-client/Exception/Except.h \
    redis-client/Command.h \
    redis-client/CRedisClient.h \
    redis-client/CRedisPool.h \
    redis-client/CRedisSocket.h \
    redis-client/CResult.h \
    redis-client/ExceptRedis.h \
    redis-client/redisCommon.h

SOURCES += \
    redis-client/Exception/Except.cpp \
    redis-client/Command.cpp \
    redis-client/CRedisClient.cpp \
    redis-client/CRedisPool.cpp \
    redis-client/CRedisSocket.cpp \
    redis-client/CResult.cpp \
    redis-client/RedisClientConnection.cpp \
    redis-client/RedisClientHash.cpp \
    redis-client/RedisClientHyperLogLog.cpp \
    redis-client/RedisClientKey.cpp \
    redis-client/RedisClientList.cpp \
    redis-client/RedisClientPSub.cpp \
    redis-client/RedisClientScript.cpp \
    redis-client/RedisClientServer.cpp \
    redis-client/RedisClientSet.cpp \
    redis-client/RedisClientSortedSet.cpp \
    redis-client/RedisClientString.cpp \
    redis-client/RedisTransaction.cpp

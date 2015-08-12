TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Command.cpp \
    CRedisClient.cpp \
    CRedisSocket.cpp \
    CResult.cpp \
    RedisClientHash.cpp \
    RedisClientKey.cpp \
    RedisClientList.cpp \
    RedisClientString.cpp \
    RedisTransaction.cpp \
    RedisClientSortedSet.cpp \
    CredisClientServer.cpp \
    test.cpp \
    testSortedSet.cpp \
    testServer.cpp

OTHER_FILES += \
    rediscpp.pro.user

HEADERS += \
    Command.h \
    common.h \
    CRedisClient.h \
    CRedisSocket.h \
    CResult.h \
    test.h

LIBS += \
-lPocoNet \
-lPocoFoundation

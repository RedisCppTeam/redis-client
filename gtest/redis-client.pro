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
    ../redis-client



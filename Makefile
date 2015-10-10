CXX=g++
EXEC=libRedisclient.so
ROOT_PATH=/$(EXEC)

INCLUDES=-I ./

CXXFLAGS+=$(INCLUDES)

CXXFLAGS+= -Wall -o2 

#当前目录下的头文件
SRCS = $(wildcard ./C*.cpp ./R*.cpp )

#库名
LDFLAGS=-lPocoFoundation -lPocoNet

OBJS=$(SRCS:.cpp=.o)

all:$(OBJS)
		$(CXX) -shared -fpic C*.cpp R*.cpp R*.hpp -o $(EXEC) 
		@echo "build ($(EXEC)) Successful......."

install:
		chmod +r *
		cp ./*.h /usr/local/include
		cp ./*.hpp /usr/local/include
		cp $(EXEC) /usr/local/lib
		cp $(EXEC) /usr/lib64
		ldconfig
		@echo "install Successful......."
clean :
		rm -rf $(EXEC) $(OBJS)

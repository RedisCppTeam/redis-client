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
		sudo cp ./*.h /usr/local/include
		sudo cp ./*.hpp /usr/local/include
		sudo cp $(EXEC) /usr/local/lib
		sudo cp $(EXEC) /usr/lib64
		sudo ldconfig
		@echo "install Successful......."
clean :
		rm -rf $(EXEC) $(OBJS)

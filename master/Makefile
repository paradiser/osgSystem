CSRCS= dealCsv.cpp dealServerInfo.cpp master.cpp
CSRCS_TEST= dealCsv.cpp dealServerInfo.cpp main_test.cpp

CFLAGS =  -c -O3 -DNDEBUG -fPIC -std=c++11

INCLUDE = -I. -I/usr/include/ -I/usr/include/X11/ -I/usr/local/include/GL 
INCOSG = -I/home/paradiser/download/OpenSceneGraph/include
LDLIBS = -lm -ldl -lGL -lGLU -lpthread -lXext -lX11

LDFLAGS =  -L. -L/usr/lib -L/usr/X11R6/lib -L/usr/local/lib
LDOSG = -L/home/paradiser/OpenSceneGraph/lib -losgDB -losg -losgViewer -losgSim -losgGA -losgFX
COBJS=$(patsubst %.cpp,%.o,$(CSRCS))
COBJS_TEST=$(patsubst %.cpp,%.o,$(CSRCS_TEST))

CC = g++

EXE=master

all: $(EXE)

$(EXE): $(COBJS) 
	g++ -o$(EXE) $(COBJS) $(INCLUDE) $(INCOSG) $(LDFLAGS) $(LDLIBS) $(LDOSG)

test:
	g++ -o$(EXE) $(COBJS_TEST) $(INCLUDE) $(INCOSG) $(LDFLAGS) $(LDLIBS) $(LDOSG)

$(COBJS): %.o : %.cpp 
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f *.o
	rm -f *~

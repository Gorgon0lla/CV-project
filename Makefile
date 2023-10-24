CXX      = g++
EXEC     = program
CXXFLAGS = -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_objdetect -lopencv_imgproc -lstdc++fs -lpthread -lrt -lopencv_imgcodecs
INCLUDE  = /usr/local/include/opencv4/
LIB      = /usr/local/lib/


SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)	

all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $^ -o $@ -I $(INCLUDE) -L $(LIB) $(CXXFLAGS)

.PHONY: all clean run

clean:
	rm -f $(OBJ) $(EXEC)
	rm /dev/mqueue/loggerMQ
	rm cameraLog.txt

run: $(EXEC)
	./$(EXEC)
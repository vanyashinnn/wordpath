APP = wordpath
OBJECTS = main.o


CXX = g++

all: $(APP)

%.o: %.cpp
	$(CXX) -c -o $@ $<

$(APP): $(OBJECTS)
	$(CXX) -o $(APP) $(OBJECTS)

clean:
	rm -f $(APP) $(OBJECTS)
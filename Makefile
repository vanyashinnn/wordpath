APP = wordpath
OBJECTS = main.o

GTEST_SRC = gtest-1.7.0
GTEST_TARGET = $(GTEST_SRC)/target
CXX = g++

all: app
distclean: clean gtest-clean


# compile source
%.o: %.cpp
	$(CXX) -c -o $@ $<

# link app
app: $(APP)
$(APP): gtest $(OBJECTS)
	$(CXX) -o $(APP) $(OBJECTS)

# clean app
clean:
	rm -f $(APP) $(OBJECTS)

# Gtest
gtest: $(GTEST_TARGET)
$(GTEST_TARGET):
	unzip $(GTEST_SRC).zip
	cd $(GTEST_SRC) && ./configure && make
	touch $(GTEST_TARGET)
gtest-clean:
	rm -rf $(GTEST_SRC)
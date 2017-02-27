APP = wordpath
TEST = wordpath_test

APP_OBJECTS = main.o

TEST_OBJECTS = tests/main.o
TEST_OBJECTS += tests/wordpath.o

GTEST_SRC = gtest-1.7.0
GTEST_TARGET = $(GTEST_SRC)/target

CXX = @g++
RM = @rm -rf

INCLUDE += -I$(GTEST_SRC)/include
LIBS += -L$(GTEST_SRC)/lib/.libs -l:libgtest.a -lpthread

LOG_ERROR=@print(){ echo -e "\033[31m"$$*"\033[0m"; }; print
LOG_INFO=@print(){ echo -e "\033[32m"$$*"\033[0m"; }; print

all: app test
distclean: clean gtest-clean
clean: app-clean test-clean

######################
#   Compile source   #
######################
%.o: %.cpp
	$(CXX) -c -o $@ $< $(INCLUDE)

#############
#   Tests   #
#############
test: $(TEST)
$(TEST): gtest $(TEST_OBJECTS)
	$(LOG_INFO) Сборка тестов
	$(CXX) -o $(TEST) $(TEST_OBJECTS) $(LIBS)
tests-run: $(TEST)
	./$(TEST)
test-clean:
	$(RM) $(TEST) $(TEST_OBJECTS)

###########
#   App   #
###########
app: tests-run $(APP)
$(APP): gtest $(APP_OBJECTS)
	$(LOG_INFO) Сборка приложения
	$(CXX) -o $(APP) $(APP_OBJECTS) $(LIBS)
app-clean:
	$(RM) $(APP) $(APP_OBJECTS)


####################
#   Gtest source   #
####################
gtest: $(GTEST_TARGET)
	$(LOG_INFO) Gtest собран
$(GTEST_TARGET):
	$(LOG_INFO) Сборка Gtest
	unzip $(GTEST_SRC).zip
	cd $(GTEST_SRC) && ./configure && make
	touch $(GTEST_TARGET)
gtest-clean:
	$(RM) $(GTEST_SRC)
APP = wordpath
TEST = wordpath_test

APP_OBJECTS =  src/main.o
APP_OBJECTS += src/wordpath.o

TEST_OBJECTS =  tests/main.o
TEST_OBJECTS += tests/wordpath.o
TEST_OBJECTS += src/wordpath.o

GTEST_SRC = gtest-1.7.0
GTEST_TARGET = $(GTEST_SRC)/target

CXX = @g++
RM = @rm -rf
CXX_FLAGS = -std=c++11

INCLUDE += -I$(GTEST_SRC)/include -Isrc
LIBS += -L$(GTEST_SRC)/lib/.libs -l:libgtest.a -lpthread

LOG_ERROR=@print(){ echo -e "\033[31m$$*\033[0m"; }; print
LOG_INFO=@print(){ echo -e "\033[32m$$*\033[0m"; }; print

all: app test
distclean: clean gtest-clean
clean: app-clean test-clean

help:
	$(LOG_INFO) "Использование: make [Цель] \n\
Цели:\n\
    all:           Сборка тестов и приложения\n\
    app:           Сборка только приложения\n\
    test:          Сборка только тестов\n\
    distclean:     Чистка всего проекта\n\
    clean:         Чистка тестов и приложения\n\
    app-clean:     Чистка только приложения\n\
    test-clean:    Чистка только тестов\n\
    help:          Показать эту справку\n"

######################
#   Compile source   #
######################
%.o: %.cpp
	$(CXX) -c -o $@ $< $(INCLUDE) $(CXX_FLAGS)

#############
#   Tests   #
#############
test: $(TEST)
$(TEST): gtest $(TEST_OBJECTS)
	$(LOG_INFO) Сборка тестов
	$(CXX) -o $(TEST) $(TEST_OBJECTS) $(LIBS)
tests-run: $(TEST)
	$(LOG_INFO) Запуск тестов
	./$(TEST)
test-clean:
	$(LOG_INFO) Чистка тестов
	$(RM) $(TEST) $(TEST_OBJECTS)

###########
#   App   #
###########
#app: tests-run $(APP)
app:  $(APP)
$(APP): gtest $(APP_OBJECTS)
	$(LOG_INFO) Сборка приложения
	$(CXX) -o $(APP) $(APP_OBJECTS) $(LIBS)
app-clean:
	$(LOG_INFO) Чистка приложения
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
	$(LOG_INFO) Чистка Gtest
	$(RM) $(GTEST_SRC)
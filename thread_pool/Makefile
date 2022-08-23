CC := g++ -std=c++98
CFLAGS := -ansi -pedantic-errors -Wall -Wextra
CPPFLAGS := -I ../include/ -I ../../ds/include -I ../../system_programming/include
LDFLAGS := -shared
DEBUG := -g
RELEASE := -DNDEBUG -O3

LDEPS := 
#list of dependencies in projects
LSYSDEPS := 
#list of dependencies in system programming
LDSDEPS := 
#list of dependencies in ds
DEPS := $(addsuffix .so,$(addprefix lib,$(LDEPS)))
SYSDEPS := $(addsuffix .so,$(addprefix ../../system_programming/libs/debug/lib,$(LSYSDEPS))) 
DSDEPS := $(addsuffix .so,$(addprefix ../../ds/libs/debug/lib,$(LDSDEPS)))
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
NOTESTSRC := $(filter-out $(wildcard *test.cpp),$(SRCS))
NOTESTOBJ := $(NOTESTSRC:.cpp=.o)
TARGET := test
LIBSO := $(addsuffix .so, $(addprefix lib,$(notdir $(CURDIR))))
DIRNAME := $(notdir $(CURDIR))


.PHONY := test
test: $(TARGET)

$(TARGET) : $(filter *_test.cpp,$(SRCS)) ../libs/debug/$(LIBSO) $(DEPS) $(DSDEPS) $(SYSDEPS)
	@$(CC) $(CPPFLAGS) -L../libs/debug -L../../ds/libs/debug -L../../system_programming/libs/debug -Wl,-rpath=../libs/debug -Wl,-rpath=../../ds/libs/debug -Wl,-rpath=../../system_programming/libs/debug $(CFLAGS) -o $@ $(DIRNAME)_test.cpp -l$(DIRNAME) $(addprefix -l,$(LDEPS)) $(addprefix -l,$(LSYSDEPS)) $(addprefix -l,$(LDSDEPS)) $(DEBUG) -lboost_thread -lpthread -lboost_chrono
	
$(DEPS) : lib%.so :
	@$(MAKE) -C ../$* debug
	rm ../$*/*.o
	
$(DSDEPS) : ../../ds/libs/debug/lib%.so :
	@$(MAKE) -C ../../ds/$* debug
	rm ../../ds/$*/*.o

$(SYSDEPS) : ../../system_programming/libs/debug/lib%.so :
	@$(MAKE) -C ../../system_programming/$* debug
	rm ../../system_programming/$*/*.o

.PHONY := all
all : release debug

.PHONY := debug
debug : ../libs/debug/$(LIBSO)

../libs/debug/$(LIBSO) : $(NOTESTOBJ)
	@$(CC) $(LDFLAGS) $(DEBUG) -o $@ $^

$(NOTESTOBJ) : $(NOTESTSRC)
	@$(CC) $(CPPFLAGS) -c $(CFLAGS) $(DEBUG) -fpic $^

.PHONY := release
release : ../libs/release/$(LIBSO)

../libs/release/$(LIBSO) : $(NOTESTOBJ)
	@$(CC) $(LDFLAGS) $(RELEASE) -o $@ $^

.PHONY := clean
clean : 
	@rm $(TARGET) *.o

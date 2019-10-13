CC= g++ -std=c++17 -fpermissive
DBG= gdb

DFLAGS= -pg -g
OFLAGS= -O2
CPPFLAGS= -Wall -Wextra $(OFLAGS)

TARGET= brain

DIR1= src
LIBS= 

SOURCES = $(wildcard $(DIR1)/*.cpp)

## End sources definition
INCLUDE= 
## end more includes

OBJECTS= $(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))

## Default rule executed
all: obj_setup $(TARGET)

## Debug
debug: $(TARGET)
	$(DBG) ./$(TARGET)

## Rule for making the actual target
$(TARGET): $(OBJECTS)
	@-echo "============="
	@-echo "Linking the target $@"
	@-echo "============="
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@-echo "-- Link finished --"

## compile and generate dependency 
obj/%.o: */%.cpp makefile
	@-echo "Building $@\n\t$(CC) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)"
	@-$(CC) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)

## Clean / Build Rule
obj_setup:
	mkdir -p ./obj
obj_clean:
	rm -rf ./obj
clean:
	rm -f $(TARGET) $(OBJECTS) $(AUTODEPS)
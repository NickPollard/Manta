CXX = g++
CXXFLAGS = -Wall -I . `pkg-config --cflags gtkmm-2.4`
LFLAGS = `pkg-config --libs gtkmm-2.4`
EXECUTABLE = manta
include Makelist
DEPS = $(SRCS:src/%.cpp=bin/%.d)
OBJS = $(SRCS:src/%.cpp=bin/%.o)
OBJS_DBG = $(SRCS:src/%.cpp=bin/debug/%.o)

all : $(EXECUTABLE)

clean :
	@echo "--- Removing Object Files ---"
	@-rm -vf bin/*.o;
	@echo "--- Removing Executable ---"
	@-rm -vf $(EXECUTABLE);

cleandebug : 
	@echo "--- Removing Object Files ---"
	@-rm -vf bin/debug/*.o;
	@echo "--- Removing Debug Executable ---"
	@-rm -vf $(EXECUTABLE)_debug;

$(EXECUTABLE) : $(SRCS) $(OBJS) $(DEPS)
	@echo "- Linking $@"
	@$(CXX) $(LFLAGS) -O2 -o $(EXECUTABLE) $(OBJS)

debug : $(EXECUTABLE)_debug

$(EXECUTABLE)_debug : $(SRCS) $(OBJS_DBG)
	@echo "- Linking $@"
	@$(CXX) -g $(LFLAGS) -o $(EXECUTABLE)_debug $(OBJS_DBG)

bin/debug/%.o : src/%.cpp
	@echo "- Compiling $@"
	@$(CXX) -g $(CXXFLAGS) -c -o $@ $<

bin/%.o : src/%.cpp
	@echo "- Compiling $@"
	@$(CXX) $(CXXFLAGS) -O2 -MD -c -o $@ $<

bin/%.d : 
	@touch $@

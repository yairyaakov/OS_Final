# Compiler
CXX = g++

# Common Compiler flags
CXXFLAGS = -Wall -g -std=c++20 -pthread -fprofile-arcs -ftest-coverage

# Valgrind-specific flags (keep debugging info)
VALGRIND_FLAGS = -Wall -g -std=c++20

# Valgrind options
VALGRIND_MEMCHECK_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND_HELGRIND_FLAGS = --tool=helgrind
VALGRIND_CALLGRIND_FLAGS = --tool=callgrind

# Source directories
SRC_DIRS = Network/src Algorithms/src DataStructures/src DesignPatterns/src

# Demo source file containing main()
MAIN_SRC_LF = LfserverDemo.cpp
MAIN_SRC_P = PipelineServerDemo.cpp

# Test source file containing main()
MAIN_SRC_TESTS = TestsMain.cpp

# Find all .cpp files in the source directories
SRCS_LF = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS))) $(MAIN_SRC_LF)
SRCS_P = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS))) $(MAIN_SRC_P)
SRCS_TESTS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS))) $(MAIN_SRC_TESTS) Tests/src/Tests.cpp

# All Source Files
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS))) $(MAIN_SRC_LF) $(MAIN_SRC_P)

# Object files
OBJS_LF = $(SRCS_LF:.cpp=.o)
OBJS_P = $(SRCS_P:.cpp=.o)
OBJS_TESTS = $(SRCS_TESTS:.cpp=.o)

# First target executable
TARGET1 = LFServer

# Second target executable
TARGET2 = PServer

# Third target executable
TARGET3 = tests

# Default rule that builds both targets without coverage/valgrind flags
all: $(TARGET1) $(TARGET2) $(TARGET3)

# Rule to build the first target
$(TARGET1): $(OBJS_LF)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(OBJS_LF)

# Rule to build the second target
$(TARGET2): $(OBJS_P)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(OBJS_P)

# Rule to build the third target
$(TARGET3): $(OBJS_TESTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(OBJS_TESTS)

# Compile with coverage flags
coverage: $(TARGET1) $(TARGET2)
	gcov -r --relative-only $(SRCS) > gcov_report.txt
	@echo "Code coverage report saved to gcov_report.txt"
	@echo "HTML report generated in ./coverage_html"
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/include/*' --output-file coverage.info
	genhtml coverage.info --output-directory coverage_html


valgrind-memcheck_LF: $(TARGET1)
	@echo "Running Leader Folower program with Valgrind memcheck..."
	valgrind $(VALGRIND_MEMCHECK_FLAGS) ./$(TARGET1)

valgrind-memcheck_P: $(TARGET2)
	@echo "Running Pipeline program with Valgrind memcheck..."
	valgrind $(VALGRIND_MEMCHECK_FLAGS) ./$(TARGET2)

# Compile for Valgrind (Helgrind)
valgrind-helgrind_LF: CXXFLAGS = $(VALGRIND_FLAGS)
valgrind-helgrind_LF: clean $(TARGET1)
	@echo "Running Leader Folower program with Valgrind helgrind..."
	valgrind $(VALGRIND_HELGRIND_FLAGS) ./$(TARGET1)

valgrind-helgrind_P: CXXFLAGS = $(VALGRIND_FLAGS)
valgrind-helgrind_P: clean $(TARGET2)
	@echo "Running Pipeline program with Valgrind helgrind..."
	valgrind $(VALGRIND_HELGRIND_FLAGS) ./$(TARGET2)
	
valgrind-callgrind_LF: $(TARGET1)
	@echo "Running Leader Folower program with Valgrind Callgrind..."
	valgrind $(VALGRIND_CALLGRIND_FLAGS) ./$(TARGET1)
	@echo "Callgrind output saved as callgrind.out.<pid>"
	@echo "Use 'kcachegrind' to visualize the callgraph."
	
valgrind-callgrind_P: $(TARGET2)
	@echo "Running Pipeline program with Valgrind Callgrind..."
	valgrind $(VALGRIND_CALLGRIND_FLAGS) ./$(TARGET2)
	@echo "Callgrind output saved as callgrind.out.<pid>"
	@echo "Use 'kcachegrind' to visualize the callgraph."

# Clean up compiled files
clean:
	rm -f $(OBJS_LF) $(OBJS_P) $(OBJS_TESTS) $(TARGET1) $(TARGET2) $(TARGET3) *.gcda *.gcno *.info *.gcov

	find Network/src -name '*.gcda' -delete
	find Network/src -name '*.gcno' -delete
	find Algorithms/src -name '*.gcda' -delete
	find Algorithms/src -name '*.gcno' -delete
	find DataStructures/src -name '*.gcda' -delete
	find DataStructures/src -name '*.gcno' -delete
	find DesignPatterns/src -name '*.gcda' -delete
	find DesignPatterns/src -name '*.gcno' -delete
	find Tests/src -name '*.gcda' -delete
	find Tests/src -name '*.gcno' -delete


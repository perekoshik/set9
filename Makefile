CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
TARGET   := stringsort_bench
OBJS     := main.o string_generator.o string_sort_tester.o sorts.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp sorts.h string_generator.h string_sort_tester.h
string_generator.o: string_generator.cpp string_generator.h
string_sort_tester.o: string_sort_tester.cpp string_sort_tester.h sorts.h
sorts.o: sorts.cpp sorts.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET) --out results.csv

clean:
	rm -f $(OBJS) $(TARGET) results.csv

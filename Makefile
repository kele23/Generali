
#CONFIG
DESIGN_NAME=generali
EXECUTABLE=bin/$(DESIGN_NAME).x

CXX=g++
CFLAGS= -c -std=c++11
LD=g++

all: $(EXECUTABLE)

$(EXECUTABLE): obj/$(DESIGN_NAME).o
	@echo Linking $@
	$(LD) -g $(INCDIR) -o $@ $^

obj/$(DESIGN_NAME).o: src/$(DESIGN_NAME).cpp
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<
	
clean:
	@echo Cleaning
	@rm -f obj/*.o *~
	@rm -f $(EXECUTABLE)

.PHONY: all clean

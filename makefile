CFLAGS=-std=gnu11 -Wall -g -O3
CPPFLAGS=-std=c++11 -Wall -g -O1

SOURCES_CPP=$(wildcard src/*.cpp) 
SOURCES_C=$(wildcard src/*.c)

OBJDIR=obj
OBJECTS=$(addprefix $(OBJDIR)/,$(notdir $(SOURCES_CPP:.cpp=.o))) $(addprefix $(OBJDIR)/,$(notdir $(SOURCES_C:.c=.o)))

EXECUTABLE=comp

all: $(OBJDIR) $(SOURCES_CPP) $(SOURCES_C) $(EXECUTABLE) 
    
$(EXECUTABLE): $(OBJECTS) 
	g++ -o $@ $^

# Including all .d files, that contain the make statements
# describing depencencies of a files based on #include
# statements. This is needed, so that changes to header files
# get noticed, and apropriate files that depend on them get
# recompiled. The dependencies get generated with compilers -MM
# option. 
-include $(OBJDIR)/*.d

$(OBJDIR)/%.o: src/%.cpp
	g++ -c $(CPPFLAGS) -o $@ $< 
	g++ -MM $(CPPFLAGS) -MT '$@' src/$*.cpp > $(OBJDIR)/$*.d

$(OBJDIR)/%.o: src/%.c
	gcc -c $(CFLAGS) -o $@ $<
	gcc -MM $(CFLAGS) -MT '$@' src/$*.c > $(OBJDIR)/$*.d

# Creates 'obj' directory if it doesent exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/* $(EXECUTABLE)

# Convert a drawing textfile to a drawing.hpp, containing
# that textfile in a string constant.
src/drawing.hpp: src/resources/drawing
	./tools/parseDrawing > /tmp/drawing.hpp
	mv -f /tmp/drawing.hpp src/drawing.hpp

#	sed '/Do not edit/q' src/drawing.hpp > /tmp/drawing.hpp
#	printf "\nconst string[] drawing = {" >> /tmp/drawing.hpp 
#	xxd -i src/resources/drawing | tail -n+2 | head -n-1 >> /tmp/drawing.hpp
#	unicode -s -m0 $(cat src/resources/drawing) | grep "UTF-16BE: " | sed 's/^.*UTF-16BE: \([^ ]*\).*$/\1/g' | sed 's/^/u8\"\\u/'  | sed 's/$/\"/' | tr '\n' ' ' | sed 's/ /, /g' | sed 's/, $/\n/' | fold -s
#	printf "};\n\n#endif" >> /tmp/drawing.hpp
#	mv -f /tmp/drawing.hpp src/drawing.hpp


CC      = g++
CFLAGS  = -std=c++11 -MMD -MP -g
LDFLAGS = -L/usr/X11R6/lib -lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lpthread
LIBS    = 
INCLUDE = -I/usr/X11R6/include
SRC_DIR = .
OBJ_DIR = .
SOURCES = $(shell ls $(SRC_DIR)/*.cpp) 
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
	TARGET  = main
DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	  $(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	   @if [ ! -d $(OBJ_DIR) ]; \
		         then echo "mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR); \
				       fi
	     $(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 

clean:
	  $(RM) $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: all clean


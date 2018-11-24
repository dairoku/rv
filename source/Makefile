# ----------------------------------------------------------------------
#  Macros for glutDrverTest
# ----------------------------------------------------------------------
RAWVIEW_DIR		= .
RAWVIEW_OBJS	= \
  main.o
RAWVIEW_SOURCES	= $(RAWVIEW_OBJS:.o=.cpp)
RAWVIEW_DEPENDS	= $(RAWVIEW_OBJS:.o=.d)
RAWVIEW_NAME	= rv

# ----------------------------------------------------------------------
#  Macros for GTK Library
# ----------------------------------------------------------------------
GLIB_CFLAGS		= `pkg-config --cflags gtkmm-3.0`
GLIB_LDFLAGS	= `pkg-config --libs gtkmm-3.0`

# ----------------------------------------------------------------------
#  Macros for Common part
# ----------------------------------------------------------------------
SOURCES	= $(RAWVIEW_SOURCES)
DEPENDS	= $(RAWVIEW_DEPENDS)
OBJS	= $(RAWVIEW_OBJS)
TARGETS	= $(RAWVIEW_NAME)
VPATH	= $(RAWVIEW_DIR)

# ----------------------------------------------------------------------
#  Build Options
# ----------------------------------------------------------------------
CC			= g++
LD			= $(CC)
CFLAGS		= $(GLIB_CFLAGS) -MD -fpic
LDFLAGS 	= $(GLIB_LDFLAGS)
INCLUDES	= -I /usr/include -I $(RAWVIEW_DIR)

# ----------------------------------------------------------------------
#  Default Rules
# ----------------------------------------------------------------------
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.cpp.d:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

# ----------------------------------------------------------------------
#  Build Rules
# ----------------------------------------------------------------------
all: $(TARGETS)
#	$(CC) $(LDFLAGS) $<

$(TARGETS) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o output/$@

$(DEPENDS) : $(SOURCES)

test:

clean:
	$(RM) $(TARGETS) $(OBJS) $(DEPENDS)

include $(DEPENDS)

# ----------------------------------------------------------------------
#  Macros for rv
# ----------------------------------------------------------------------
RAWVIEW_DIR		= .
RAWVIEW_OBJS	= \
  main.o
RAWVIEW_SOURCES	= $(RAWVIEW_OBJS:.o=.cpp)
RAWVIEW_DEPENDS	= $(RAWVIEW_OBJS:.o=.d)
RAWVIEW_NAME	= rv

# ----------------------------------------------------------------------
#  Macros for libibc
# ----------------------------------------------------------------------
LIBIBC_DIR		= ../libibc
LIBIBC_INC_DIR		= $(LIBIBC_DIR)/include
LIBIBC_SRC_DIR		= $(LIBIBC_DIR)/source
LIBIBC_OBJS	=
LIBIBC_SOURCES	= $(LIBIBC_OBJS:.o=.cpp)
LIBIBC_DEPENDS	= $(LIBIBC_SOURCES:.cpp=.d)

# ----------------------------------------------------------------------
#  Macros for GTK Library
# ----------------------------------------------------------------------
GLIB_CFLAGS		= `pkg-config --cflags gtkmm-3.0`
GLIB_LDFLAGS	= `pkg-config --libs gtkmm-3.0`

# ----------------------------------------------------------------------
#  Macros for GTK Library
# ----------------------------------------------------------------------
BOOST_CFLAGS		= 
BOOST_LDFLAGS		= -lboost_program_options

# ----------------------------------------------------------------------
#  Macros for Common part
# ----------------------------------------------------------------------
SOURCES	= $(RAWVIEW_SOURCES) $(LIBIBC_SOURCES)
DEPENDS	= $(RAWVIEW_DEPENDS) $(LIBIBC_DEPENDS)
OBJS	= $(RAWVIEW_OBJS) $(LIBIBC_OBJS)
TARGETS	= $(RAWVIEW_NAME)
VPATH	= $(RAWVIEW_DIR)

# ----------------------------------------------------------------------
#  Build Options
# ----------------------------------------------------------------------
CC			= g++
LD			= $(CC)
CFLAGS		= $(GLIB_CFLAGS) $(BOOST_CFLAGS) -MD -fpic
LDFLAGS 	= $(GLIB_LDFLAGS) $(BOOST_LDFLAGS)
INCLUDES	= -I /usr/include -I $(RAWVIEW_DIR) -I $(LIBIBC_INC_DIR)

# ----------------------------------------------------------------------
#  Default Rules
# ----------------------------------------------------------------------
# Note that we are using "-0 $@" option to generate target objects
# in the source file directory
#
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

.cpp.d:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# ----------------------------------------------------------------------
#  Build Rules
# ----------------------------------------------------------------------
all: $(TARGETS)

$(DEPENDS) : $(SOURCES)

$(TARGETS) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o output/$@

.PHONY : clean depend
clean:
	$(RM) output/$(TARGETS) $(OBJS) $(DEPENDS)

-include $(DEPENDS)

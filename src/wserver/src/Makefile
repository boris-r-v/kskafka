ifndef MAIN_INCLUDE
CURDIR = ../../../..
include $(CURDIR)/Makefile.config
endif
 
MODULE=kafka
ITEM=server
VERSION=1_0

GUI_INC = #`pkg-config --cflags gtkmm-3.0` #`pkg-config --cflags goocanvasmm-1.0`
GUI_LIBS = #`pkg-config --libs gtkmm-3.0` #`pkg-config --libs goocanvasmm-1.0`

CPPFLAGS = -I./ -I$(PREFIX)/include -I$(PREFIX)/include/ksa  -I$(PREFIX)/include/ksa/modules  -I$(PREFIX)/include/ksa/modules/$(MODULE) $(GUI_INC) 

LIBS = $(GUI_LIBS) -lKSA 
LDFLAGS = -shared -L$(PREFIX)/lib/ksa/  -L$(PREFIX)/lib/ksa/modules -L/usr/local/lib

OPT = -fpic -std=c++11
CXXFLAGS = $(OPT)
CFLAGS = $(OPT)

TARGET_LIB = lib$(MODULE)_$(ITEM)_$(VERSION).so

all: $(TARGET_LIB) 

SOURCES = $(wildcard *.cpp) $(wildcard mongoose/*.cpp)
COM_OBJECTS= $(ITEM)Factory.o $(ITEM)Base.o $(SOURCES:.cpp=.o) mongoose/mongoose.o
LIB_ITEM = $(LD_TSAN) -lcppkafka

LIBS_PATH=$(PREFIX)/lib/ksa/modules
INC_PATH=$(PREFIX)/include/ksa/modules/$(MODULE)
IDL_PATH=$(PREFIX)/include/ksa/idl/$(MODULE)
TEMPLATE_PATH=$(PREFIX)/share/ksa/template/$(MODULE)

OBJECTS = $(COM_OBJECTS) 

IDL = ../idl/$(ITEM).xml

$(TARGET_LIB): $(OBJECTS) 
	$(LINK.cpp) $(COM_OBJECTS) -o $(TARGET_LIB) $(LIBS) $(LIB_ITEM)

clean:
	rm -f $(OBJECTS) $(TARGET_LIB) *.cc *.hh

install: $(TARGET_LIB)
	@sudo mkdir -pv $(INC_PATH)
	@sudo chmod a+w $(INC_PATH)
	@sudo mkdir -pv $(IDL_PATH)
	@sudo chmod a+w $(IDL_PATH)
	@sudo mkdir -pv $(LIBS_PATH)
	@sudo chmod a+w $(LIBS_PATH)
	@cp -v $(TARGET_LIB) $(LIBS_PATH)
	@cp -v *.hpp $(INC_PATH)
	@cp -v ../idl/*.xml $(IDL_PATH)
	@sudo mkdir -pv $(TEMPLATE_PATH)
	@sudo chmod a+w $(TEMPLATE_PATH)
	@find . -name "*.template" -exec cp -v "{}" $(TEMPLATE_PATH)  ";"

stub: 
	$(PREFIX)/bin/ksmake

remake: 
	make clean; $(PREFIX)/bin/ksmake; make install

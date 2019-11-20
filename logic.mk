#---------
#
# CppUTest Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
PROJECT_NAME = logic
COMPONENT_NAME = logic

CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_USE_GCOV = Y

CPP_PLATFORM = Gcc

CPPUTEST_HOME=/home/chromebuntu/Downloads/C/cpputest/

CFLAGS = -Dmalloc=cpputest_malloc -Dfree=cpputest_free
CPPFLAGS =

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.
#CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h
SRC_DIRS = \
  src/$(COMPONENT_NAME)\
  include\

TEST_SRC_DIRS = \
  tests\
  tests/$(PROJECT_NAME)\
  mocks\

INCLUDE_DIRS =\
  .\
  src\
  src/$(COMPONENT_NAME)\
  include\
  mocks\
  $(CPPUTEST_HOME)/include\
  
LD_LIBRARIES += -lpthread
include $(CPPUTEST_HOME)/build/MakefileWorker.mk


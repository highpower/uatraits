INC = -I../include -I../library -I.
LIBS = -luatraits 
TESTS = tests

CXX_FLAGS = -pthread -O2 -fPIC 

SWIG = swig
SWIG_INC = -I../include

PREFIX =
FAKE_PREFIX = blib

JAVA = java
JAVA_VERSION = $(shell $(JAVA) -version 2>&1 | grep "java version" | sed -e 's/.*\"\(.*\)\".*/\1/g')
JAVA_INC = -I/usr/lib/jvm/default-java/include/
JAVA_LIBS = -ljava$(JAVA_VERSION)

JAVA_INSTALL_PATH = $(FAKE_PREFIX)/usr/local/share/java
JAVA_UATRAITS_INSTALL_PATH = $(JAVA_INSTALL_PATH)/uatraits
JAVA_DYNLOAD_INSTALL_PATH = $(JAVA_INSTALL_PATH)/lib-dynload

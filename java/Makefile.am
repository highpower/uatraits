include Variables.mk

CURRENT=detector

all-local: build-local

build-local: $(CURRENT)-build

install-exec-local: $(CURRENT)-install

clean-local: $(CURRENT)-clean

%-install: %-build
	cp -r -T $(FAKE_PREFIX) $(DESTDIR)

%_wrap_java.cpp %.java: %.i
	mkdir -p ru/yandex/uatraits
	$(SWIG) $(SWIG_INC) -c++ -java -package ru.yandex.uatraits -outdir ru/yandex/uatraits -o $*_wrap_java.cpp  $<

%.o: %.cpp
	$(CXX) $(INC) -I$$(find /usr/l* -name 'jni.h' | head -1 | sed -e 's/\/jni\.h$$//') -I$$(find /usr/l* -name 'jni.h' | head -1 | sed -e 's/\/jni\.h$$//')/linux/ $(CXX_FLAGS) -c $< -o $@

%_java.so: %.o %_wrap_java.o
	$(CXX) $(LIBS) -shared $^ -o $@

%-build-java: %.java %_java.so
	mkdir -p $(JAVA_UATRAITS_INSTALL_PATH) $(JAVA_DYNLOAD_INSTALL_PATH)
	cp -R ru $(JAVA_UATRAITS_INSTALL_PATH)
	cp $*_java.so $(JAVA_DYNLOAD_INSTALL_PATH)/lib$*.so

%-build: %-build-java ; 

%-test-java: %-build-java 
	javac $(TESTS)/*.java $(JAVA_UATRAITS_INSTALL_PATH)/ru/yandex/uatraits/*.java
	java -ea -Djava.library.path=$(JAVA_DYNLOAD_INSTALL_PATH) -cp $(TESTS):$(JAVA_UATRAITS_INSTALL_PATH) Test

%-test: %-test-java ;

%-clean:
	$(RM) *.so *.o *.java *.class *wrap_java.cpp *.cxx
	$(RM) -r $(FAKE_PREFIX)

check-TESTS:
	echo test

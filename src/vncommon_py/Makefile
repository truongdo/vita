#
# Makefile
# truong-d, 2016-02-15 20:58
#

all:
	@echo "Makefile needs your attention"
	/usr/bin/c++   -Dtext_util_EXPORTS -Wall -std=gnu++0x -g -fPIC -lboost_regex -Wl,-rpath -L. -o text_util.o -c text_util.cpp
	#/usr/bin/c++  -fPIC  -Wall -std=gnu++0x -g   -shared -Wl,-soname,libtext_util.so -o libtext_util.so text_util.o -lboost_program_options -lboost_regex
	/usr/bin/c++   -Dvncommon_EXPORTS -Wall -std=gnu++0x -g -fPIC -lboost_regex -Wl,-rpath -L. -o vncommon.o -c vncommon.cpp
	#/usr/bin/c++  -fPIC  -Wall -std=gnu++0x -g   -shared -Wl,-soname,libvncommon.so -o libvncommon.so vncommon.o libtext_util.so -lboost_program_options -lboost_regex -Wl,-rpath
	swig -c++ -python vncommon.i
	g++ -w -std=c++11 -O2 -fPIC  -c vncommon_wrap.cxx -I/usr/include/python2.7
	g++ -w -std=c++11 -shared text_util.o vncommon.o  vncommon_wrap.o -lboost_regex -Wl,-rpath -L. -o _vncommon.so

# vim:ft=make
#

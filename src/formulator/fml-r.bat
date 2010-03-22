qmake -makefile formulator-release.pro
del stdout.txt
del stderr.txt
mingw32-make 1>> stdout.txt 2>> stderr.txt
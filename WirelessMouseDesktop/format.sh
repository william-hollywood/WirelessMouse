#!/bin/bash -ex

find src \(\
	-name "*.h" \
	-o -name "*.hh" \
	-o -name "*.hpp" \
	-o -name "*.c" \
	-o -name "*.cc" \
	-o -name "*.cpp" \
	\) -exec clang-format -i {} \+

find . -name "CMakeLists.txt" \
	-not -path "./external/*" \
	-exec gersemi -i ./mingw.cmake {} \+

go fmt bundle-resources.go

#/bin/sh

set -e
mkdir -p build
cd build/
cmake .. -DCOVERAGE=ON
make
ctest -T Test -T Coverage --output-on-failure
cd ..
rm -f build/coverage.info
lcov -c -o build/coverage.info -d build/CMakeFiles --exclude=$PWD/test*.c
genhtml build/coverage.info -o build

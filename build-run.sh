export LD_LIBRARY_PATH=/usr/local/lib64:/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH;/usr/local/bin/cmake -DCMAKE_CXX_COMPILER:STRING=/usr/local/bin/g++  -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON .
make
./ttaskconsprod

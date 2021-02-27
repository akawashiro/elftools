#! /bin/bash -eu

g++ -fPIC -c -o lib.o lib.cc
g++ -lpthread -Wl,--hash-style=gnu -shared -Wl,-soname,lib.so -o lib.so lib.o
g++ -Wl,--hash-style=gnu -o main main.cc lib.so -lpthread

mv lib.so lib.so.original
../../build/sold -i lib.so.original -o lib.so.soldout --section-headers --check-output

# Use sold
ln -sf lib.so.soldout lib.so

# Use original
# ln -sf lib.so.original lib.so

LD_LIBRARY_PATH=. ./main

rm preinit.o
rm remap.o
rm libremap.so
rm application
gcc -c preinit.c
gcc -c -fPIC remap.c
gcc remap.o -shared -o libremap.so
gcc -o application application.c preinit.o -L. -lremap -T linkerscript.ld
gcc -o tracer tracer.c
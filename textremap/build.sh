rm preinit.o
rm remap.o
rm libremap.so
rm application
gcc -std=gnu99 -c preinit.c
gcc -std=gnu99 -c -fPIC remap.c
gcc -std=gnu99 remap.o -shared -o libremap.so
gcc -std=gnu99 -o application application.c preinit.o -L. -lremap -T linkerscript.ld
gcc -std=gnu99 -o tracer tracer.c
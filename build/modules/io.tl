declare int32 scanf(int8_ptr, ...);
declare int32 printf(int8_ptr, ...);
declare int32 puts(int8_ptr);
declare void fopen(int8_ptr, int8_ptr);
declare int32 getchar();

void createFile(int8_ptr name) {
    fopen(name, "w+");
}

void println(int8_ptr message) {
    puts(message);
}

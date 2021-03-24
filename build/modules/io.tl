declare i32 scanf(i8_ptr, ...);
declare i32 printf(i8_ptr, ...);
declare i32 puts(i8_ptr);
declare void fopen(i8_ptr, i8_ptr);
declare i32 getchar();

void createFile(i8_ptr name) {
    fopen(name, "w+");
}

void println(i8_ptr message) {
    puts(message);
}

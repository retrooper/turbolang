declare i32 scanf(i8*, ...);
declare i32 printf(i8*, ...);
declare i32 puts(i8*);
declare void fopen(i8*, i8*);
declare i32 remove(i8*);
declare i32 getchar();

void createFile(i8* name) {
    fopen(name, "w+");
}

void deleteFile(i8* name) {
    remove(name);
}

void println(i8* message) {
    puts(message);
}
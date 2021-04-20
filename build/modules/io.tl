declare i32 scanf(i8*, ...);
declare i32 printf(i8*, ...);
declare i32 puts(i8*);
declare void fopen(i8*, i8*);
declare i32 getchar();

void createFile(i8* name) {
    fopen(name, "w+");
}

void println(i8* message) {
    puts(message);
}

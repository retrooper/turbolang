class IOFile {
}

class File {
    i8* name;
    IOFile* iof;
}

declare i32 scanf(i8*, ...);

declare i32 printf(i8*, ...);
declare i32 puts(i8*);

declare IOFile* fopen(i8*, i8*);
declare void fclose(IOFile*);
declare i32 fputc(i32, IOFile*);
#declare i32 fputs(i8*, IOFile*);
declare i32 remove(i8*);
declare i32 getchar();
#Its actually u32 not i32
declare i8* realloc(i8*, i32);

File createFile(i8* name) {
    IOFile* iof = fopen(name, "w");
    File f;
    f.name = name;
    #f.iof = iof;
    return f;
}

#void writeFile(File file, i8* content) {
 #   IOFile* iof = file.iof;
  #  fputs(content, iof);
#}

#void closeFile(File file) {
 #   IOFile* iof = file.iof;
  #  fclose(iof);
#}

#void deleteFile(File file) {
  #  i8* name = file.name;
 #   remove(name);
#}

#File createFileThenWriteThenClose(i8* name, i8* content) {
 #   IOFile* iof = fopen(name, "w");
  #  fputs(content, iof);
   # fclose(iof);
    #File f;
    #f.name = name;
    #f.iof = iof;
    #return f;
#}

void println(i8* message) {
    puts(message);
}

void readLine(i8* destination) {
    scanf("%[^\n]", destination);
}
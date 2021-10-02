declare i32 strlen(i8*);
declare i32 strcmp(i8*, i8*);
declare i8 strcpy(i8*, i8*);
declare i8* strcat(i8*, i8*);
declare f64 atof(i8*);
declare i32 atoi(i8*);

f64 stringToDouble(i8* str) {
    f64 result = atof(str);
    return result;
}

i32 stringToInt(i8* str) {
    i32 result = atoi(str);
    return result;
}
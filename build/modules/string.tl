declare i32 strlen(i8*);
declare i32 strcmp(i8*, i8*);
declare f64 atof(i8*);
declare i32 atoi(i8*);

f64 stringToDouble(i8* number) {
    f64 result = atof(number);
    return result;
}

i32 stringToInt(i8* number) {
    i32 result = atoi(number);
    return result;
}
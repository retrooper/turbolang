import "io"
i32 main() {
    #i8[100] message;
    #TODO support initializing arrays
    i32 i = 0;
    while (true) {
        printf("%s%i\n", "I: ", i);
        i = i + 1;
    }
    return 0;
}
import "io"
i32 main() {
    i32 i = 2;
    printf("%s%i\n", "Yea: ", i);
    while (true) {
        i = i + i;
        printf("%s%i\n", "I: ", i);
    }
    return 0;
}
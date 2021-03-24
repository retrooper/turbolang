import "io"
int32 main() {
    int8 value;
    int8_ptr valuePtr = &value;
    scanf("%i", valuePtr);
    printf("%i\n", value);
    return 0;
}
#FIX UNSIGNED AND SIGNED INTEGERS, CURRENT SIGNED ONES ARE UNSIGNED???
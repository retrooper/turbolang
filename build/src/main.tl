import "io"
i32 main() {
    i32[100] array;
    i32 i = 0;
    while (i < 100) {
       i32* ptr = &array[i];
       *ptr = 2;
       i = i + 1;
    }
    i32 index0 = array[99];
    printf("We got %d\n", index0);
    return 0;
}
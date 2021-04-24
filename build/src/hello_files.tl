import "io"
i32 main() {
    File file = createFile("Example.txt");
    writeFile(file, "Hello, World!");
    closeFile(file);
    return 0;
}
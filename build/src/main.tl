import "io"
import "time"
i32 main() {
    File file = createFileThenWriteThenClose("Test.txt", "Content!");
    sleepms(10000);
    deleteFile(file);
    return 0;
}
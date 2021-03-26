import "io"
import "time"
i32 main() {
    println("Counting up to 1000...");
    sleepms(1000);
    i32 counter = 1;
    while (counter < 1001) {
        printf("%s%i\n", "Number: ", counter);
        counter = counter + 1;
    }
    println("Finished");
    return 0;
}
import "io"
import "time"
i32 main() {
    bool running = true;
   i32 i = 2;
   while (running) {
        printf("%i\n", i);
        sleepms(1000);
        running = false;
    }
    return 0;
}
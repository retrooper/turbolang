import "io"
import "time"
i32 main() {
    while (true) {
         i64 ct = getCurrentTimeMilli();
         printf("We got: %d\n", ct);
    }
    return 0;
}
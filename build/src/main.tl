import "math"
import "scanner"
import "system"
import "time"
i32 main() {
    system("echo Hello, World!");
    while (true) {
        println("Enter in a number you want the absolute value of:");
        i32 input;
        i32 result = scanf("%i", &input);
        #result 1 is success, non 1 is fail(=0)
        i32 output = abs(input);
        printf("%s%i", "Absolute value: ", output);
        println("");
        sleep(1000000);
    }
    return 0;
}
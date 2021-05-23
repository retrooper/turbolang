import "io"
import "string"
i32 main() {
    i8[100] text;
    strcpy(&text, "Test");
    strcat(&text, " Yeah");
    println("We got: ");
    puts(&text);
    return 0;
}
import "io"
import "thread"

void test() {
    println("Test");
    println("Test2");
}

i32 main() {
    println("Hello, World!");
    Thread threadA;
    threadA.name = "Thread A";
    startThread(&threadA, test);
    i64 id = threadA.id;
    joinThread(&threadA);
    return 0;
}
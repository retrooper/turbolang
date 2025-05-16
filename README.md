# turbolang
TurboLang is a compiled programming language. This is the implementation of the compiler for TurboLang. This compiler was written using C++ 17.

## Sample Program written in TurboLang
```
import "io"
import "string"
import "time"
i32 main() {
    println("Hey, what is your name?");
    i8[1024] data;
    readLine(&data);
    i8* outputData = &data;
    printf("Welcome to my program, %s.\n", outputData);
    sleepms(2000);
    printf("How old are you %s?", outputData);
    i32 age;
    scanf("%i", &age);
    if (age < 13) {
        println("You are so young? Leave LOL\n");
    }
    elif (age > 35) {
        printf("You're a bit old XD. %i already??\n", age);
    }
    else {
        printf("You are %i? Thats fine..\n", age);
    }
    sleepms(2000);
    println("Alright, I've got to go!");
    return 0;
}
```

## How to Build The Project

#### Dependencies:
* CMake
* [llvm](https://github.com/llvm/llvm-project) (Available in AUR package manager)
* [wclang](https://github.com/tpoechtrager/wclang) (Available in AUR package manager)
* [osxcross](https://github.com/tpoechtrager/osxcross) (Available in AUR package manager)

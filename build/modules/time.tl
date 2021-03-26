 declare i32 usleep(i32);

 void sleep(i32 micro) {
    usleep(micro);
 }

 void sleepms(i32 ms) {
    i32 micro = ms * 1000;
    usleep(micro);
 }
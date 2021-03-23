 declare int32 usleep(int32);

 void sleep(int32 microseconds) {
    usleep(microseconds);
 }

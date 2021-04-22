class TimeVal {
    i64 tv_sec;
    i64 tv_usec;
}
declare i32 usleep(i32);
declare i32 gettimeofday(TimeVal*, i8*);

i64 getCurrentTimeMicro() {
    TimeVal tv;
    TimeVal* tvPtr = &tv;
    i8* ptr = nullptr;
    gettimeofday(tvPtr, ptr);
    i64 tvsec = tv.tv_sec;
    i64 tvusec = tv.tv_usec;
    return tvsec * 1000 + tvusec;
}

i64 getCurrentTimeNano() {
    i64 micro = getCurrentTimeMicro();
    return micro * 1000;
}

i64 getCurrentTimeMilli() {
    i64 micro = getCurrentTimeMicro();
    return micro / 1000;
}

 void sleep(i32 micro) {
    usleep(micro);
 }

 void sleepms(i32 ms) {
    i32 micro = ms * 1000;
    usleep(micro);
 }
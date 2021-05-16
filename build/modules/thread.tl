link "pthread"

class Thread {
    i8* name;
    i64 id;
}

declare i32 pthread_create(i64*, i64*, i8*, i8*);

declare i32 pthread_join(i64, ...);

void startThread(Thread* threadPtr, i8* task) {
    i64 id;
    i8* name = *threadPtr.name;
    pthread_create(&id, nullptr, task, name);
    *threadPtr.id = id;
}

void joinThread(Thread* threadPtr) {
    i64 id = *threadPtr.id;
    pthread_join(id, nullptr);
}
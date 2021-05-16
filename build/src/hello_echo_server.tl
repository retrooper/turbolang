import "io"
import "thread"
import "networking"

void onData(DyadEvent* e) {
    DyadStreamHandle* stream = *e.stream;
    i8* data = *e.data;
    i32 size = *e.size;
    dyad_write(stream, data, size);
}

void onAccept(DyadEvent* e) {
    #DyadStreamHandle* remote = *e.remote;
    #dyad_addListener(remote, 7, onData, nullptr);
    #dyad_writef(remote, "echo server\r\n");
    println("Client connected!!!!");
}

void onError(DyadEvent* e) {
    i8* msg = *e.msg;
    printf("server error: %s\n", msg);
}

i32 main() {
    println("Server!");
    dyad_init();
    DyadStreamHandle* s = dyad_newStream();
    dyad_addListener(s, 9,  onError,  nullptr);
    dyad_addListener(s, 2, onAccept, nullptr);
    dyad_listen(s, 8000);
    while (true) {
        dyad_update();
    }
    return 0;
}
import "io"
import "networking"
void onConnect(DyadEvent* e) {
    i8* msg = *e.msg;
    printf("connected: %s\n", msg);
}

void onData(DyadEvent* e) {
    i8* data = *e.data;
    printf("%s", data);
}

i32 main() {
    println("Client!");
    dyad_init();
    DyadStreamHandle* s = dyad_newStream();
    dyad_addListener(s, 4, onConnect, nullptr);
    dyad_addListener(s, 7, onData, nullptr);
    dyad_connect(s, "127.0.0.1", 8000);

    while (dyad_getStreamCount() > 0) {
       dyad_update();
     }
    dyad_shutdown();
    return 0;
}
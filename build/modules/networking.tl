class DyadStreamHandle {

}

class DyadEvent {
    i32 type;
    i8* udata;
    DyadStreamHandle* stream;
    DyadStreamHandle* remote;
    i8* msg;
    i8* data;
    i32 size;
}

declare void dyad_init();
declare DyadStreamHandle* dyad_newStream();
declare void dyad_addListener(DyadStreamHandle*, i32, i8*, i8*);
declare i32 dyad_listen(DyadStreamHandle*, i32);
declare i32 dyad_connect(DyadStreamHandle*, i8*, i32);
declare i32 dyad_getStreamCount();
declare void dyad_update();
declare void dyad_write(DyadStreamHandle*, i8*, i32);
declare void dyad_writef(DyadStreamHandle*, i8*, ...);
declare void dyad_shutdown();
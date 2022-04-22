; ModuleID = 'inputFile'
source_filename = "inputFile"

%IOFile = type {}
%File = type { i8*, %IOFile* }

@0 = private unnamed_addr constant [2 x i8] c"w\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"%[^\0A]\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Time to go!\00", align 1

declare i32 @scanf(i8*, ...)

declare i32 @printf(i8*, ...)

declare i32 @puts(i8*)

declare %IOFile* @fopen(i8*, i8*)

declare void @fclose(%IOFile*)

declare i32 @fputc(i32, %IOFile*)

declare i32 @remove(i8*)

declare i32 @getchar()

declare i8* @realloc(i8*, i32)

define %File @createFile(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %iof = alloca %IOFile*, align 8
  %ame = load i8*, i8** %name, align 8
  %function_call_fopen = call %IOFile* @fopen(i8* %ame, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @0, i32 0, i32 0))
  store %IOFile* %function_call_fopen, %IOFile** %iof, align 8
  %f = alloca %File, align 8
  store %File* null, %File* %f, align 8
  %name1 = getelementptr inbounds %File, %File* %f, i32 0, i32 0
  %.name = load %File, i8** %name1, align 8
  %ame2 = load i8*, i8** %name, align 8
  %name3 = getelementptr inbounds %File, %File* %f, i32 0, i32 0
  store i8* %ame2, i8** %name3, align 8
  %1 = load %File, %File* %f, align 8
  ret %File %1
}

define void @println(i8* %0) {
entry:
  %message = alloca i8*, align 8
  store i8* %0, i8** %message, align 8
  %essage = load i8*, i8** %message, align 8
  %function_call_puts = call i32 @puts(i8* %essage)
  ret void
}

define void @readLine(i8* %0) {
entry:
  %destination = alloca i8*, align 8
  store i8* %0, i8** %destination, align 8
  %estination = load i8*, i8** %destination, align 8
  %function_call_scanf = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i8* %estination)
  ret void
}

define i32 @main() {
entry:
  call void @println(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0))
  ret i32 0
}

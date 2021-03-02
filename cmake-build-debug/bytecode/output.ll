; ModuleID = 'inputFile'
source_filename = "inputFile"

@0 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello, World\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @exit(i32, ...)

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @1, i32 0, i32 0))
  ret i32 0
}

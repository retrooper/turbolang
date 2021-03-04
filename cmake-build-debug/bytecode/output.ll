; ModuleID = 'inputFile'
source_filename = "inputFile"

declare i32 @printf(i8*, ...)

declare i32 @exit(i32, ...)

define i32 @main() {
entry:
  %a = alloca i32, i32 10, align 4
  ret i32 0
}

; ModuleID = 'inputFile'
source_filename = "inputFile"

@a = external global i32

declare i32 @printf(i8*, ...)

declare i32 @exit(i32, ...)

define i32 @main() {
entry:
  ret i32 0
}

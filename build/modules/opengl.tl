link "OpenGL"

#Actually unsigned i32, but TODO change
declare void glClear(i32);
#ALSO UNSIGNED I32
declare void glBegin(i32);
declare void glEnd();
declare void glFlush();

declare void glVertex2f(f32, f32);
declare void glVertex3f(f32, f32, f32);
declare void glVertex4f(f32, f32, f32, f32);

declare void glColor3f(f32, f32, f32);
declare void glColor4f(f32, f32, f32, f32);
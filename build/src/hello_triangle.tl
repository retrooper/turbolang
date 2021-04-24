import "io"
import "time"
import "system"
import "glfw"
import "opengl"

void render(GLFWHandle* window) {
    glBegin(4);
    glVertex3f(-0.5, -0.5, 0);
    glVertex3f(0.5, -0.5, 0);
    glVertex3f(0, 0.5, 0);
    glColor4f(1, 0, 0, 1);
    glEnd();
    glFlush();
}

void loop(GLFWHandle* window) {
    bool shouldClose = false;
    i32 temp;
    while (shouldClose == false) {
        temp = glfwWindowShouldClose(window);
        shouldClose = temp != 0;
        i32 clearValue = 16640;
        glClear(clearValue);
        f32 color = 0.0;
        f32 alpha = 1.0;
        glClearColor(color, color, color, alpha);
        render(window);
        glfwSwapBuffers(window):
        glfwPollEvents();
    }
}

void terminate() {
    glfwTerminate();
}

i32 main() {
    i32 result = glfwInit();
    if (result != 0) {
        GLFWHandle* window = glfwCreateWindow(1280, 720, "Hello, World", nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glClearColor(0.5, 0.5, 0.5, 1);
        loop(window);
        terminate();
    }
    else {
        println("GLFW failed to initialize!");
        exit(-1);
    }
    return 0;
}
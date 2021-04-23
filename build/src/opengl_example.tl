import "io"
import "time"
import "system"
import "glfw"
import "opengl"

void render(GLFWHandle* window) {

}

void loop(GLFWHandle* window) {
    bool shouldClose = false;
    i32 temp;
    while (shouldClose == false) {
        temp = glfwWindowShouldClose(window);
        shouldClose = temp != 0;
        i32 clearValue = 16640;
        glClear(clearValue);
        f32 color = 0.5;
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
        GLFWHandle* np = nullptr;
        GLFWHandle* window = glfwCreateWindow(1280, 720, "Hello, World", np, np);
        glfwMakeContextCurrent(window);
        f32 rgb = 0.1;
        f32 alpha = 0.5;
        glfwSwapInterval(1);
        glClearColor(rgb, rgb, rgb, alpha);
        loop(window);
        terminate();
    }
    else {
        println("Why did GLFW init fail!");
        exit(-1);
    }
    return 0;
}
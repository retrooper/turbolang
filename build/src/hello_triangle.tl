import "io"
import "time"
import "system"
import "glfw"
import "opengl"

void render() {
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
    i32 frames = 0;
    i32 fps = 0;
    f64 lastSecondTime;
    while (shouldClose == false) {
        temp = glfwWindowShouldClose(window);
        shouldClose = temp != 0;
        i32 clearValue = 16640;
        glClear(clearValue);
        f32 color = 0.0;
        f32 alpha = 1.0;
        glClearColor(color, color, color, alpha);
        render();
        glfwSwapBuffers(window):
        glfwPollEvents();
        frames = frames + 1;
        f64 currentFrameTime = glfwGetTime();
        f64 deltaTime = currentFrameTime - lastSecondTime;
        bool oneSecPassed = deltaTime > 1.0;
        if (oneSecPassed) {
            fps = frames;
            printf("FPS: %d\n", fps);
            frames = 0;
            lastSecondTime = currentFrameTime;
        }
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
        bool vSync = false;
        i32 swapInterval = 0;
        if (vSync) {
            swapInterval = 1;
        }
        glfwSwapInterval(swapInterval);
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
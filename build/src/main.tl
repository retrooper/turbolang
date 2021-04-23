import "io"
import "time"
import "glfw"
import "system"
i32 main() {
    i32 result = glfwInit();
    if (result == 0) {
        println("Why did GLFW init fail!");
        exit(-1);
    }
    else {
        GLFWHandle* np = nullptr;
        GLFWHandle* window = glfwCreateWindow(1280, 720, "Hello, World", np, np);
        println("Created window!");
        i32 temp = glfwVulkanSupported();
        bool vkSupported = temp != 0;
        if (vkSupported) {
            println("Vulkan is supported!");
        }
        else {
            println("Why is vulkan not supported?");
        }
        bool shouldClose = false;
        while (shouldClose == false) {
            glfwPollEvents();
            temp = glfwWindowShouldClose(window);
            shouldClose = temp != 0;
        }
        println("Exiting program!");
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    return 0;
}
link "glfw"

class GLFWHandle {

}

declare i32 glfwInit();
declare void glfwTerminate();

declare GLFWHandle* glfwCreateWindow(i32, i32, i8*, GLFWHandle*, GLFWHandle*);
declare void glfwDestroyWindow(GLFWHandle*);
declare i32 glfwWindowShouldClose(GLFWHandle*);

declare void glfwPollEvents();
declare f64 glfwGetTime();
declare i8* glfwGetClipboardString(GLFWHandle*);
declare i32 glfwVulkanSupported();
declare void glfwWindowHint(i32, i32);
declare void glfwShowWindow(GLFWHandle*);
declare void glfwHideWindow(GLFWHandle*);
declare void glfwFocusWindow(GLFWHandle*);
declare i32 glfwGetKey(GLFWHandle*, i32);
declare void glfwGetWindowSize(GLFWHandle*, i32*, i32*);
declare void glfwSetWindowTitle(GLFWHandle*, i8*);
declare void glfwGetFramebufferSize(GLFWHandle*, i32*, i32*);
declare void glfwMakeContextCurrent(GLFWHandle*);
declare void glfwSwapBuffers(GLFWHandle*);
declare void glfwSwapInterval(i32);
declare void glClearColor(f32, f32, f32, f32);
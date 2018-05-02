#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Defining the vertex shader source code in OpenGL Shading Language (GLSL)

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Defining the fragment shader source code in GLSL

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {

	// Initialises the glfw library
	// glfwWindowhint() sets hints for the next call to glfwCreateWindow()
	// Changing the major and minor version to 3 targets OpenGL version 3.3
	// Changing the targeted profile to GLFW_OPENGL_CORE_PROFILE means we get access to a smaller subset of features as it excludes backwards compatible features we don't need

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates a GLFW window and stores it in a GLFWwindow object
	// glfwCreateWindow parameters = width, height, Initial title, the monitor to use for fullscreen mode or NULL for windows, the window whose context to share or NULL to not share resources

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL - Creating a window", NULL, NULL);

	// If the glfwCreateWindow has failed we can compare it to NULL to print an error message

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// glfwMakeCurrentContext() sets the given window to the main context on the current thread

	glfwMakeContextCurrent(window);
	
	// Sets the function to be called when the given window is resized

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialise GLAD

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Defining vertex input

	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f // top left
	};

	unsigned int indices[] = {
		0, 2, 3, // first triangle
		0, 1, 2 // second triangle
	};

	// Vertex shader

	// Creates a shader object and returns a reference ID to the created object
	// Shader object created of type GL_VERTEX_SHADER and it's ID stored as a uint in vertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// glShaderSource replaces the source code of a given shader object
	// Parameters are shader object reference ID, the number of elements in the string array, an array of pointers to strings containing the source code, arrays of string lengths
	// glCompileShader then compiles the given shader object

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Vertex shader compile error checking

	int success;
	char infoLog[512];

	// glGetShaderiv allows querying of a shader object for information on a given parameter
	// In this instance we are querying that the compilation of the shader object vertexShader was successful

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


	if (!success)
	{
		// glGetShaderInfoLog returns the informaiton log for a shader object and contains relevant information about a shader's compilation
		// Parameters = shader object, size of the character buffer, length of the string returned in infoLog, a character array to store returned information from the log

		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	//Fragment shader

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Shader Program
	// To use recently compiled shaders they need to be linked to a shader program object and then activate this shader program when rendering objects
	// The shader program's shaders will then be used when issuing render calls.

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Shader program linking error checking

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Vertex Array Object, Vertex Buffer Object, Element Buffer Object

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);

	// glGenBuffers generates a buffer object
	// First parameter equals number of buffers to generate, second is where to store the resulting buffer IDs

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Binds the buffer object to the current buffer type target
	// In this instance bind buffer object VBO to buffer type GL_ARRAY_BUFFER
	// Parameters are buffer type target and ID of buffer object to bind

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Linking vertex attributes

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Sets front and back side of the triangles to render the lines only

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	// glfwWindowShouldClose checks if the given window object has been instructed to close

	while (!glfwWindowShouldClose(window))
	{
		// Checks if ESC has been pressed

		processInput(window);

		// glClearColor defines the color used when resetting the buffer's color values
		// glClear then clears the buffer with the color defined in glClearColor

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfwSwapBuffers swaps the front and back buffers of the given window. Rendering commands draw to the back buffer whereas the front buffer contains the final image displayed on screen
		// glfwPollEvents processes events received and then returns immediately. Processing events will cause the window and input callbacks associated with those events to be called
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfwTerminate destroys all remaining windows, frees any allocated resources and sets the glfw library to an uninitialised state

	glfwTerminate();
	return 0;
}

// Processes the input for a given window object. Specifically if a key press == ESC, which then sets the WindowShouldClose variable to true for the give window object
// The next time this variable is evaluated by glfwWindowShouldClose(), the window will be closed

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Gets called when window is resized as it is set to do so by glfwFramebufferSizeCallback
// glViewport defines the actual viewport for rendering
// The first two parameters define the left-most and bottom-most co-ordinate of the viewport respectively, acting as the origin point
// The final two parameters define the width and height of the viewport
// These co-ordinates are given as normalized device co-ordinates

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
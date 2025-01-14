#include <iostream>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// int main(int, char**){
//     std::cout << "Hello, from hellowork! Done linking glfw \n";
// }

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "} \n";

int main()
{   
    //// INIT A WINDOW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearningOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Shit happens, check how shit happen, likely because glfw not installed correctly \n" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //// BUILD and COMPILE shader programs
    // vertexShader 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check vertexShader compiled sucess or not
    int  successVertexShader;
    char infoLogVertexShader[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertexShader);
    if(!successVertexShader)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertexShader);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogVertexShader << std::endl;
    }
    // fragmentShader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check fragmentShader compiled sucess or not
    int  successFragmentShader;
    char infoLogFragmentShader[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragmentShader);
    if(!successFragmentShader)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragmentShader);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogFragmentShader << std::endl;
    }

    // Link shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check shaderProgram compiled sucess or not
    int  success;
    char infoLogShaderProgram[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogShaderProgram);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLogShaderProgram << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  


    //// CREATE objects and tranfer to buffer/object arrays
    float verticesTRI[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    float verticesREC[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indicesREC[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    unsigned int VBO_tri, VBO_rec, VAO_tri, EBO, VAO_rec;
    glGenVertexArrays(1, &VAO_tri); 
    glGenVertexArrays(1, &VAO_rec); 
    glGenBuffers(1, &VBO_tri);
    glGenBuffers(1, &VBO_rec);
    glGenBuffers(1, &EBO);

    // TRIANGLE
    glBindVertexArray(VAO_tri);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tri);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTRI), verticesTRI, GL_STATIC_DRAW);
    // Config points atributes 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // Rectangle
    glBindVertexArray(VAO_rec);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_rec);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesREC), verticesREC, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesREC), indicesREC, GL_STATIC_DRAW);

    // Config points atributes 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    

    // 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    int i=1;

    while(!glfwWindowShouldClose(window))
    {   //
        processInput(window);

        //
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //
        if (i == 0){
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO_rec);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        } else if (i == 1) {
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO_tri);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        }


        //
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO_rec);
    glDeleteBuffers(1, &VBO_rec);
    glDeleteVertexArrays(1, &VAO_tri);
    glDeleteBuffers(1, &VBO_tri);

    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
#define GLEW_STATIC
#define NUM_SEEDS 50
#define HEIGHT 705
#define WIDTH 1366
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include "ShaderStore.txt"

//Compile with "-mwindows" to make the command line window not appear when *.exe is run

static unsigned int compileShader(const char* source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = malloc(len * sizeof(char));
        glGetShaderInfoLog(id, len, &len, message);
        printf("Oh Dear! The shader failed to compile!\n");
        printf("%s\n", message);
        glDeleteShader(id);
        return 0;
    }


    return id;
}

static unsigned int CreateShader(const char* VertexShader, const char* FragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vertexshader = compileShader(VertexShader, GL_VERTEX_SHADER);
    unsigned int fragmentshader = compileShader(FragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(program, vertexshader);
    glAttachShader(program, fragmentshader);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
    return(program);
}

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Voronoi", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        printf("Oh dear! It seems that for some reason, GLEW didn't initialise correctly.\n");
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    float positions[] = {
        -1.0f, -1.0f,    //0 
         1.0f, -1.0f,    //1
         1.0f,  1.0f,    //2
        -1.0f,  1.0f,    //3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, /*6 * 2 * sizeof(float)*/ sizeof(positions), positions, GL_STATIC_DRAW); //Don't change the size!

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, /*6 * sizeof(unsigned int)*/ sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int shader = CreateShader(basicvertex, euclideanfragment);
    glUseProgram(shader);
    
    int seedx[NUM_SEEDS] = {0};
    int seedy[NUM_SEEDS] = {0};
    int incx[NUM_SEEDS]  = {0};
    int incy[NUM_SEEDS]  = {0};
    float seedc[NUM_SEEDS * 3] = {0};

    srand(time(NULL));
    for (int i = 0; i < NUM_SEEDS; i++) {
        seedx[i] = rand() % WIDTH;
        seedy[i] = rand() % HEIGHT;
        incx[i] = (rand() % 11) - 5;
        incy[i] = (rand() % 11) - 5;
    }
    for (int i = 0; i < (3 * NUM_SEEDS); i++) {
        seedc[i] = (float)(rand() % 256) / 255.0;
    }
    
    GLint seedxlocation = glGetUniformLocation(shader, "u_seedx");
    GLint seedylocation = glGetUniformLocation(shader, "u_seedy");
    GLint seedclocation = glGetUniformLocation(shader, "u_seedc");
    
    glUniform1iv(seedxlocation, NUM_SEEDS, &seedx);
    glUniform1iv(seedylocation, NUM_SEEDS, &seedy);
    glUniform1fv(seedclocation, NUM_SEEDS * 3, &seedc);
    
    GLint heightlocation = glGetUniformLocation(shader, "u_height");
    GLint widthlocation  = glGetUniformLocation(shader, "u_width");
    glUniform1f(heightlocation, (float)HEIGHT);
    glUniform1f(widthlocation, (float)WIDTH);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    int frames = 0;
    time_t period = time(NULL);
    float framerate;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        
        for (int i = 0; i < NUM_SEEDS; i++) {
            seedx[i] = seedx[i] + incx[i];
            seedy[i] = seedy[i] + incy[i];
        }
        
        for (int i = 0; i < NUM_SEEDS; i++) {
            if(seedx[i] > WIDTH  || seedx[i] < 0){
                incx[i] = incx[i] * -1;
            }

            if(seedy[i] > HEIGHT || seedy[i] < 0) {
                incy[i] = incy[i] * -1;
            }
        }

        glUniform1iv(seedxlocation, NUM_SEEDS, &seedx);
        glUniform1iv(seedylocation, NUM_SEEDS, &seedy);

        glfwSwapBuffers(window);
        glfwPollEvents();
        /*
        frames++;
        if (frames == 600) {
            time_t now = time(NULL);
            framerate = 600.0 / (now - period);
            printf("%f\n", framerate);
            frames = 0;
            period = now;
        }
        */
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
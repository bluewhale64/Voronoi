#include "GL/glew.h"
#include "GL/glut.h"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

void display(void){

    // clear the drawing buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);

    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0,  1.0);
    glVertex2f( 1.0,  1.0);
    glVertex2f( 1.0, -1.0);
    
    glEnd();

    
    glFlush();
    // swap buffers called because we are using double buffering
    glutSwapBuffers();

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
	glutCreateWindow("GLEW Test");
    
    glewInit();
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
    glUseProgram(LoadShaders("vertexshader.glsl","fragmentshader.glsl"));
    
    glutDisplayFunc(display);
    
    glutMainLoop();
    
    return 0;
}
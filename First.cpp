#include "GL/freeglut.h"
#include "GL/gl.h"

#include <iostream>

int CurrentWidth = 640;
int CurrentHeight = 480;

void reshapeFunction(int Width, int Height) {
  CurrentWidth = Width;
  CurrentHeight = Height;
  glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void renderFunction() {
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(CurrentWidth, CurrentHeight);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  int WindowHandle = glutCreateWindow("OpenGL - First window demo");
  if (WindowHandle < 1)
    return 1;
  std::cerr << glGetString(GL_VERSION) << "\n";

  glClearColor(1.0, 0.0, 0.0, 0.0);
  glutReshapeFunc(reshapeFunction);
  glutDisplayFunc(renderFunction);
  glutMainLoop();
  return 0;
}

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/gl.h"

#include <iostream>
#include <sstream>

int CurrentWidth = 640;
int CurrentHeight = 480;

unsigned FrameCount = 0;

void reshapeFunction(int Width, int Height) {
  CurrentWidth = Width;
  CurrentHeight = Height;
  glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void renderFunction() {
  ++FrameCount;
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
}

void idleFunction() {
  glutPostRedisplay();
}

enum {
  TF_InitialCall,
  TF_SubsequentCall
};

void timerFunction(int Value) {
  const int kTimerInterval = 250;
  const int kMillisPerSec = 1000;
  const int kFPSScale = kMillisPerSec / kTimerInterval;

  glutTimerFunc(kTimerInterval, timerFunction, TF_SubsequentCall);
  if (Value == TF_InitialCall)
    return;
  std::stringstream S;
  int FPS = FrameCount * kFPSScale;
  FrameCount = 0;
  S << FPS << " FPS @ " << CurrentWidth << " x " << CurrentHeight;
  glutSetWindowTitle(S.str().c_str());
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

  GLenum GlewInitResult = glewInit();
  if (GlewInitResult != GLEW_OK) {
    std::cerr << "glewInit: " << glewGetErrorString(GlewInitResult) << "\n";
    return 1;
  }

  glClearColor(1.0, 0.0, 0.0, 0.0);
  glutReshapeFunc(reshapeFunction);
  glutDisplayFunc(renderFunction);
  glutIdleFunc(idleFunction);
  glutTimerFunc(0, timerFunction, TF_InitialCall);
  glutMainLoop();
  return 0;
}

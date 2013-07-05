#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/gl.h"

#include <iostream>
#include <sstream>

const GLchar VertexShader[] = R"(
#version 330
layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Color;
out vec4 ex_Color;

void main(void) {
  gl_Position = in_Position;
  ex_Color = in_Color;
}

)";

const GLchar FragmentShader[] = R"(
#version 330
in vec4 ex_Color;
out vec4 out_Color;

void main(void) {
  out_Color = ex_Color;
}

)";

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
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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

void myErrorCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity,
                     GLsizei _length, const char *_message, void *_userParam) {
  // Protip:
  // Put a breakpoint in this callback so that ARB_debug_output messages
  // will trigger the breakpoint, and then look at the current stack trace.
  //
  // Source: <http://www.youtube.com/watch?v=btNVfUygvio>
  std::cerr << _message << "\n";
}

GLuint VertexShaderID;
GLuint FragmentShaderID;
GLuint ProgramID;
void createShaders() {

  VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *VShaderSource = VertexShader;
  glShaderSource(VertexShaderID, 1, &VShaderSource, nullptr);
  glCompileShader(VertexShaderID);

  FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar *FShaderSource = FragmentShader;
  glShaderSource(FragmentShaderID, 1, &FShaderSource, nullptr);
  glCompileShader(FragmentShaderID);

  ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);
  glUseProgram(ProgramID);
}

void destroyShaders() {
  glUseProgram(0);
  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);
  glDeleteShader(FragmentShaderID);
  glDeleteShader(VertexShaderID);
  glDeleteProgram(ProgramID);
}

GLuint VAOID;
GLuint VBOID;
struct Vertex {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
};

template <typename T> T *typedNullptr() { return static_cast<T *>(nullptr); }

template <typename MemberTy, typename StructTy>
GLvoid *offsetOfAsPtr(MemberTy StructTy::*MemberPtr) {
  return std::addressof(typedNullptr<StructTy>()->*MemberPtr);
}

void createVertexBufferObject() {
  Vertex Vertices[] = {{{-0.8f, -0.8f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                       {{0.0f, 0.8f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                       {{0.8f, -0.8f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
                       {{0.8f, 0.8f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}};

  glGenVertexArrays(1, &VAOID);
  glBindVertexArray(VAOID);

  glGenBuffers(1, &VBOID);
  glBindBuffer(GL_ARRAY_BUFFER, VBOID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        offsetOfAsPtr(&Vertex::XYZW));
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        offsetOfAsPtr(&Vertex::RGBA));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void destroyVertexBufferObject() {
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &VBOID);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VAOID);
}

void closeFunction() {
  destroyShaders();
  destroyVertexBufferObject();
}

int main(int argc, char **argv) {

  glutInit(&argc, argv);
  // For some reason, this causes ARB_debug_output to not be enabled:
  // glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

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

  glClearColor(0.0, 0.0, 0.0, 0.0);
  createShaders();
  createVertexBufferObject();

  glutCloseFunc(closeFunction);
  glutReshapeFunc(reshapeFunction);
  glutDisplayFunc(renderFunction);
  glutIdleFunc(idleFunction);
  glutTimerFunc(0, timerFunction, TF_InitialCall);
  // See ARB_debug_output. This means that you don't have to constantly be
  // checking glGetError().
  glDebugMessageCallbackARB(myErrorCallback, NULL);
  // Cause a GL_INVALID_ENUM error:
  // glEnable(723932423);
  glutMainLoop();
  return 0;
}

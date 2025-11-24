#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <thread>
#include "Graphics.h"
extern EGLContext g_context;



GLuint program, vbo, posAttrib, colUniform;

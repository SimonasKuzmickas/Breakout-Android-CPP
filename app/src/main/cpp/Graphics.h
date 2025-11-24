#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

// Global EGL state
extern EGLDisplay g_display;
extern EGLSurface g_surface;
extern EGLContext g_context;
extern int g_width;
extern int g_height;

// Global GL objects
extern GLuint program;
extern GLuint vbo;
extern GLuint posAttrib;
extern GLuint colUniform;

// Initialization
bool initGraphics(ANativeWindow* window);

// Rendering helpers
void renderRedScreen();
void flip();
void initSquare();
void renderSquare();

#endif // GRAPHICS_UTILS_H
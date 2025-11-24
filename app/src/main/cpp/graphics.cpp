#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

static EGLDisplay g_display = EGL_NO_DISPLAY;
static EGLSurface g_surface = EGL_NO_SURFACE;
static EGLContext g_context = EGL_NO_CONTEXT;
static int g_width = 0;
static int g_height = 0;

bool initGraphics(ANativeWindow* window)
{
    EGLint attributes[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
            EGL_RED_SIZE,        8,
            EGL_GREEN_SIZE,      8,
            EGL_BLUE_SIZE,       8,
            EGL_NONE
    };

    EGLConfig config;
    EGLint num;

    EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(dpy, nullptr, nullptr);
    eglChooseConfig(dpy, attributes, &config, 1, &num);

    EGLSurface surf = eglCreateWindowSurface(dpy, config, window, nullptr);

    EGLint ctxAttributes[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    EGLContext ctx = eglCreateContext(dpy, config, EGL_NO_CONTEXT, ctxAttributes);
    eglMakeCurrent(dpy, surf, surf, ctx);

    g_display = dpy;
    g_surface = surf;
    g_context = ctx;

    eglQuerySurface(dpy, surf, EGL_WIDTH,  &g_width);
    eglQuerySurface(dpy, surf, EGL_HEIGHT, &g_height);

    glViewport(0, 0, g_width, g_height);
    glClearColor(1.f, 0.f, 0.f, 1.f);

    return true;
}

GLuint program, vbo, posAttrib, colUniform;

void renderRedScreen() {

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    // eglSwapBuffers(g_display, g_surface);
}

void flip() {

    eglSwapBuffers(g_display,g_surface);
}

void initSquare() {
    // Vertex data: two triangles for a square
    GLfloat vertices[] = {
            -0.5f,-0.5f,  0.5f,-0.5f,  0.5f,0.5f,
            -0.5f,-0.5f,  0.5f,0.5f,  -0.5f,0.5f
    };

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    const char* vsSrc="attribute vec2 aPos;void main(){gl_Position=vec4(aPos,0.0,1.0);}";
    const char* fsSrc="precision mediump float;uniform vec4 uColor;void main(){gl_FragColor=uColor;}";

    GLuint vs=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vsSrc,nullptr); glCompileShader(vs);
    GLuint fs=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fsSrc,nullptr); glCompileShader(fs);

    program=glCreateProgram();
    glAttachShader(program,vs); glAttachShader(program,fs); glLinkProgram(program);

    posAttrib=glGetAttribLocation(program,"aPos");
    colUniform=glGetUniformLocation(program,"uColor");
}

void renderSquare() {
    if (g_display==EGL_NO_DISPLAY) return;
    eglMakeCurrent(g_display,g_surface,g_surface,g_context);

    // glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    glUniform4f(colUniform,0.0f,1.0f,0.0f,1.0f); // green

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,0,0);

    glDrawArrays(GL_TRIANGLES,0,6);

    glDisableVertexAttribArray(posAttrib);
}
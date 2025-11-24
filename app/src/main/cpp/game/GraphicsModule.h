#pragma once
#include "IModule.h"

struct GraphicsContext {
    EGLContext context = EGL_NO_CONTEXT;
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    int width;
    int height;
};

struct AppContext {
    ANativeWindow* window = nullptr;
    std::thread thread;
    std::atomic<bool> running{false};
};

class GraphicsModule : public IModule, public IModuleRender {
public:
    GraphicsModule(AppContext* context) {
        appContext = context;
    }

    void onStart() override {
        initGraphics();
        initSquare();
    }

    void onRender() override {
        renderRedScreen();
        renderSquare();
        flip();
    }

    void onShutdown() override {

    }

private:
    GraphicsContext* graphicsContext;
    AppContext* appContext;

    GLuint program;
    GLuint vbo;
    GLuint posAttrib;
    GLuint colUniform;

    bool initGraphics()
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

        EGLSurface surf = eglCreateWindowSurface(dpy, config, appContext->window, nullptr);

        EGLint ctxAttributes[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };

        EGLContext ctx = eglCreateContext(dpy, config, EGL_NO_CONTEXT, ctxAttributes);
        eglMakeCurrent(dpy, surf, surf, ctx);

        graphicsContext = new GraphicsContext();
        graphicsContext->display = dpy;
        graphicsContext->surface = surf;
        graphicsContext->context = ctx;

        eglQuerySurface(dpy, surf, EGL_WIDTH,  &graphicsContext->width);
        eglQuerySurface(dpy, surf, EGL_HEIGHT, &graphicsContext->height);

        glViewport(0, 0, graphicsContext->width, graphicsContext->height);

        return true;
    }

    void renderRedScreen() {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        // eglSwapBuffers(g_display, g_surface);
    }

    void flip() {

       eglSwapBuffers(graphicsContext->display,graphicsContext->surface);
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
        if (graphicsContext->display==EGL_NO_DISPLAY) return;
        eglMakeCurrent(graphicsContext->display,graphicsContext->surface,graphicsContext->surface,graphicsContext);

        // glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        glUniform4f(colUniform,0.0f,1.0f,0.0f,1.0f); // green

        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,0,0);

        glDrawArrays(GL_TRIANGLES,0,6);

        glDisableVertexAttribArray(posAttrib);
    }
};
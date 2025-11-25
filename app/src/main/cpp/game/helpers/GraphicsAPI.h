#pragma once
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <functional>
#include <vector>
#include <thread>

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
    AAssetManager* assetManager = nullptr;
};

class GraphicsAPI {
public:
    GraphicsAPI(AppContext* context) {
        appContext = context;

        initGraphics();
        initSquare();
    }

    void drawRectangle(float x, float y, float w, float h,
                       float r, float g, float b, float a) const {

        const float VIRTUAL_WIDTH = 1920.0f;
        const float VIRTUAL_HEIGHT = 1080.0f;

        float x0 = (2.0f * x / VIRTUAL_WIDTH) - 1.0f;
        float y0 = (2.0f * y / VIRTUAL_HEIGHT) - 1.0f;
        float x1 = (2.0f * (x + w) / VIRTUAL_WIDTH) - 1.0f;
        float y1 = (2.0f * (y + h) / VIRTUAL_HEIGHT) - 1.0f;

        GLfloat vertices[] = {
                x0, y0, x1, y0, x1, y1,
                x0, y0, x1, y1, x0, y1
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glUseProgram(program);
        glUniform4f(colUniform, r, g, b, a);

        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void flip() {
        eglSwapBuffers(graphicsContext->display,graphicsContext->surface);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
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

        // --- Letterbox viewport calculation ---
        const int VIRTUAL_WIDTH  = 1920;
        const int VIRTUAL_HEIGHT = 1080;
        float targetAspect = static_cast<float>(VIRTUAL_WIDTH) / VIRTUAL_HEIGHT;
        float screenAspect = static_cast<float>(graphicsContext->width) / graphicsContext->height;

        int vpX, vpY, vpW, vpH;

        if (screenAspect > targetAspect) {
            // Screen is wider → black bars left/right
            vpH = graphicsContext->height;
            vpW = static_cast<int>(vpH * targetAspect);
            vpX = (graphicsContext->width - vpW) / 2;
            vpY = 0;
        } else {
            // Screen is taller → black bars top/bottom
            vpW = graphicsContext->width;
            vpH = static_cast<int>(vpW / targetAspect);
            vpX = 0;
            vpY = (graphicsContext->height - vpH) / 2;
        }

        glViewport(vpX, vpY, vpW, vpH);

        return true;
    }

    void initSquare() {
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
};
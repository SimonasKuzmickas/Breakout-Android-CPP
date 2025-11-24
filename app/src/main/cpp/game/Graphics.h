#pragma once
#include "scene/ISceneComponent.h"

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

class Graphics : public ISceneComponent, public ISceneRender {
public:
    Graphics(AppContext* context) {
        appContext = context;
    }

    void onAwake() override {
        initGraphics();
        initSquare();
    }

    void onRender() override {
        //renderRedScreen();
       // renderSquare();

        //DrawRectangle(0,0,80,80, 255, 0, 1, 1);
        flip();
    }

    void onDestroy() override {

    }

    void drawRectangle(float x, float y, float w, float h,
                       float r, float g, float b, float a) {
        float screenW = graphicsContext->width;
        float screenH = graphicsContext->height;

        float xPixels = (x / 100.0f) * screenW;
        float yPixels = (y / 100.0f) * screenH;
        float wPixels = (w / 100.0f) * screenW;
        float hPixels = (h / 100.0f) * screenH;

        float x0 = (2.0f * xPixels / screenW) - 1.0f;
        float y0 = (2.0f * yPixels / screenH) - 1.0f;
        float x1 = (2.0f * (xPixels + wPixels) / screenW) - 1.0f;
        float y1 = (2.0f * (yPixels + hPixels) / screenH) - 1.0f;


        // Vertex data: two triangles
        GLfloat vertices[] = {
                x0, y0,   x1, y0,   x1, y1,
                x0, y0,   x1, y1,   x0, y1
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glUseProgram(program);
        glUniform4f(colUniform, r, g, b, a);

        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(posAttrib);
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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
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
};
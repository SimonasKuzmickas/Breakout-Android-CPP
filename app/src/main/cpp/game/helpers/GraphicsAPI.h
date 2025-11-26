#pragma once
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <iostream>
#include <functional>
#include <vector>
#include <thread>
#include "AppContext.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

struct Vertex {
    float x, y;
    float u, v;
};

struct GraphicsContext {
    EGLContext context = EGL_NO_CONTEXT;
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;

    int width;
    int height;
};

class GraphicsAPI {
public:
    const float VIRTUAL_WIDTH = 1920.0f;
    const float VIRTUAL_HEIGHT = 1080.0f;

    GraphicsAPI(AppContext *context) {
        appContext = context;

        initGraphics(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
        initSquare();
    }

    void drawTexture(GLuint texture,
                     float x, float y, float w, float h) const
    {
        float x0 = (2.0f * x / VIRTUAL_WIDTH) - 1.0f;
        float y0 = (2.0f * y / VIRTUAL_HEIGHT) - 1.0f;
        float x1 = (2.0f * (x + w) / VIRTUAL_WIDTH) - 1.0f;
        float y1 = (2.0f * (y + h) / VIRTUAL_HEIGHT) - 1.0f;

        GLfloat verts[] = {
                x0, y0, 0.0f, 1.0f,   // bottom-left
                x1, y0, 1.0f, 1.0f,   // bottom-right
                x1, y1, 1.0f, 0.0f,   // top-right

                x0, y0, 0.0f, 1.0f,   // bottom-left
                x1, y1, 1.0f, 0.0f,   // top-right
                x0, y1, 0.0f, 0.0f    // top-left
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(texUniform, 0);

        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), (void*)0);

        glEnableVertexAttribArray(uvAttrib);
        glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(posAttrib);
        glDisableVertexAttribArray(uvAttrib);
    }

    void flip() {
        eglSwapBuffers(graphicsContext->display, graphicsContext->surface);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
    }

    GLuint loadTextureFromAssets(const char* filename) {
        AAsset* asset = AAssetManager_open(appContext->assetManager, filename, AASSET_MODE_BUFFER);
        size_t length = AAsset_getLength(asset);
        const void* buffer = AAsset_getBuffer(asset);

        int width, height, channels;
        unsigned char* pixels = stbi_load_from_memory(
                reinterpret_cast<const unsigned char*>(buffer),
                length,
                &width, &height, &channels,
                STBI_rgb_alpha);

        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        stbi_image_free(pixels);
        AAsset_close(asset);

        return texId;
    }

private:
    GraphicsContext *graphicsContext;
    AppContext *appContext;

    GLuint program;
    GLuint vbo;
    GLuint posAttrib;
    GLuint colUniform;

    GLint uvAttrib;
    GLint texUniform;   // for texture sampler

    bool initGraphics(int width, int height) {
        EGLint attributes[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
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

        eglQuerySurface(dpy, surf, EGL_WIDTH, &graphicsContext->width);
        eglQuerySurface(dpy, surf, EGL_HEIGHT, &graphicsContext->height);

        // --- Letterbox viewport calculation ---
        float targetAspect = static_cast<float>((float) width) / float(height);
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
        // Vertex data: position (x,y) + texture coordinates (u,v)
        GLfloat vertices[] = {
                // x,    y,    u, v
                -0.5f, -0.5f, 0.0f, 0.0f,  // bottom-left
                0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
                0.5f,  0.5f, 1.0f, 1.0f,  // top-right

                -0.5f, -0.5f, 0.0f, 0.0f,  // bottom-left
                0.5f,  0.5f, 1.0f, 1.0f,  // top-right
                -0.5f,  0.5f, 0.0f, 1.0f   // top-left
        };

        // Upload vertex data
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Vertex shader
        const char *vsSrc =
                "attribute vec2 aPos;"
                "attribute vec2 aUV;"
                "varying vec2 vUV;"
                "void main() {"
                "  gl_Position = vec4(aPos, 0.0, 1.0);"
                "  vUV = aUV;"
                "}";

        // Fragment shader
        const char *fsSrc =
                "precision mediump float;"
                "uniform sampler2D uTex;"
                "varying vec2 vUV;"
                "void main() {"
                "  gl_FragColor = texture2D(uTex, vUV);"
                "}";

        // Compile vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsSrc, nullptr);
        glCompileShader(vs);

        // Compile fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsSrc, nullptr);
        glCompileShader(fs);

        // Link program
        program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        // Query attribute/uniform locations
        posAttrib   = glGetAttribLocation(program, "aPos");
        uvAttrib    = glGetAttribLocation(program, "aUV");
        texUniform  = glGetUniformLocation(program, "uTex");

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
};


//    void drawRectangle(float x, float y, float w, float h,
//                       float r, float g, float b, float a) const {
//
//        float x0 = (2.0f * x / VIRTUAL_WIDTH) - 1.0f;
//        float y0 = (2.0f * y / VIRTUAL_HEIGHT) - 1.0f;
//        float x1 = (2.0f * (x + w) / VIRTUAL_WIDTH) - 1.0f;
//        float y1 = (2.0f * (y + h) / VIRTUAL_HEIGHT) - 1.0f;
//
//        GLfloat vertices[] = {
//                x0, y0, x1, y0, x1, y1,
//                x0, y0, x1, y1, x0, y1
//        };
//
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
//
//        glUseProgram(program);
//        glUniform4f(colUniform, r, g, b, a);
//
//        glEnableVertexAttribArray(posAttrib);
//        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//    }
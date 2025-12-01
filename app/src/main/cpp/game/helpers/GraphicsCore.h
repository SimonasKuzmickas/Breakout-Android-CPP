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
#include "AssetLoader.h"

namespace Breakout {

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

    GraphicsContext(EGLDisplay d, EGLSurface s, EGLContext c)
            : display{d}, surface{s}, context{c} {}
};

class GraphicsCore {
public:
    const float VIRTUAL_WIDTH = 1920.0f;
    const float VIRTUAL_HEIGHT = 1080.0f;

    GraphicsCore(AppContext *context) {
        appContext = context;

        initGraphics(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    }

    void setColor(float r, float g, float b, float a) {
        colorR = r;
        colorG = g;
        colorB = b;
        colorA = a;
    }

    void setOffset(float x, float y) {
        offsetX = x;
        offsetY = y;
    }

    // Draw simple textured quad
    void drawTexture(GLuint texture, float x, float y, float w, float h) const {
        x += offsetX;
        y += offsetY;

        float x0 = (2.0f * x / VIRTUAL_WIDTH) - 1.0f;
        float y0 = (2.0f * y / VIRTUAL_HEIGHT) - 1.0f;
        float x1 = (2.0f * (x + w) / VIRTUAL_WIDTH) - 1.0f;
        float y1 = (2.0f * (y + h) / VIRTUAL_HEIGHT) - 1.0f;

        GLfloat verts[] = {
                x0, y0, 0.0f, 1.0f,
                x1, y0, 1.0f, 1.0f,
                x1, y1, 1.0f, 0.0f,

                x0, y0, 0.0f, 1.0f,
                x1, y1, 1.0f, 0.0f,
                x0, y1, 0.0f, 0.0f
        };

        drawQuad(texture, verts, 24);
    }

    // Draw animated frame from sprite sheet
    void drawTextureAnim(GLuint texture,
                         float x, float y, float w, float h,
                         int frameId,
                         int frameWidth, int frameHeight,
                         int sheetWidth, int sheetHeight) const {
        x += offsetX;
        y += offsetY;

        float x0 = (2.0f * x / VIRTUAL_WIDTH) - 1.0f;
        float y0 = (2.0f * y / VIRTUAL_HEIGHT) - 1.0f;
        float x1 = (2.0f * (x + w) / VIRTUAL_WIDTH) - 1.0f;
        float y1 = (2.0f * (y + h) / VIRTUAL_HEIGHT) - 1.0f;

        int framesPerRow = sheetWidth / frameWidth;
        int frameX = (frameId % framesPerRow) * frameWidth;
        int frameY = (frameId / framesPerRow) * frameHeight;

        float u0 = static_cast<float>(frameX) / sheetWidth;
        float v0 = static_cast<float>(frameY) / sheetHeight;
        float u1 = static_cast<float>(frameX + frameWidth) / sheetWidth;
        float v1 = static_cast<float>(frameY + frameHeight) / sheetHeight;

        GLfloat verts[] = {
                x0, y0, u0, v1,
                x1, y0, u1, v1,
                x1, y1, u1, v0,

                x0, y0, u0, v1,
                x1, y1, u1, v0,
                x0, y1, u0, v0
        };

        drawQuad(texture, verts, 24);
    }

    void flip() {
        eglSwapBuffers(graphicsContext->display, graphicsContext->surface);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
    }

    GLuint loadTextureFromAssets(const char *filename) {
        ImageData img = AssetLoader::loadImage(appContext->assetManager, filename);
        if (img.pixels.empty()) return 0;

        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     img.width, img.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, img.pixels.data());

        return texId;
    }

    void shutdown() {
        if (!graphicsContext) return;

        // Delete GL resources you own
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }

        if (vbo) {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }

        // Detach context
        eglMakeCurrent(graphicsContext->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                       EGL_NO_CONTEXT);

        // Destroy EGL objects
        if (graphicsContext->surface != EGL_NO_SURFACE) {
            eglDestroySurface(graphicsContext->display, graphicsContext->surface);
            graphicsContext->surface = EGL_NO_SURFACE;
        }
        if (graphicsContext->context != EGL_NO_CONTEXT) {
            eglDestroyContext(graphicsContext->display, graphicsContext->context);
            graphicsContext->context = EGL_NO_CONTEXT;
        }
        if (graphicsContext->display != EGL_NO_DISPLAY) {
            eglTerminate(graphicsContext->display);
            graphicsContext->display = EGL_NO_DISPLAY;
        }

        // Free context memory
        delete graphicsContext;
        graphicsContext = nullptr;
    }

private:
    GraphicsContext *graphicsContext;
    AppContext *appContext;

    GLuint program;
    GLuint vbo;
    GLuint posAttrib;

    GLint uvAttrib;
    GLint texUniform;

    float colorR = 1, colorG = 1, colorB = 1, colorA = 1;
    float offsetX = 0, offsetY = 0;

    void initGraphics(int width, int height) {
        // --- 1. EGL setup ---
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

         // --- 2. Store graphics context info ---
        graphicsContext = new GraphicsContext{ dpy, surf, ctx };
        eglQuerySurface(dpy, surf, EGL_WIDTH, &graphicsContext->width);
        eglQuerySurface(dpy, surf, EGL_HEIGHT, &graphicsContext->height);

         // --- 3. Viewport calculation (letterboxing) ---
        float targetAspect = static_cast<float>(width) / static_cast<float>(height);
        float screenAspect =
                static_cast<float>(graphicsContext->width) / height;

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

        // --- 4. Shader sources ---
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
                "uniform vec4 uColor;"
                "varying vec2 vUV;"
                "void main() {"
                "  gl_FragColor = texture2D(uTex, vUV) * uColor;"
                "}";

         // --- 5. Compile & link shaders ---
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

         // --- 6. Query attribute/uniform locations ---
        posAttrib = glGetAttribLocation(program, "aPos");
        uvAttrib = glGetAttribLocation(program, "aUV");
        texUniform = glGetUniformLocation(program, "uTex");

         // --- 7. Enable blending ---
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glGenBuffers(1, &vbo);
        glUseProgram(program);
     }

    // --- Common draw helper ---
    void drawQuad(GLuint texture, const GLfloat* verts, size_t vertCount) const {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(GLfloat), verts, GL_DYNAMIC_DRAW);

        // Set color uniform
        GLint colorLoc = glGetUniformLocation(program, "uColor");
        glUniform4f(colorLoc, colorR, colorG, colorB, colorA);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(texUniform, 0);

        // Vertex attributes
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), (void*)0);

        glEnableVertexAttribArray(uvAttrib);
        glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Cleanup (optional if you always re‑enable before draw)
        glDisableVertexAttribArray(posAttrib);
        glDisableVertexAttribArray(uvAttrib);
    }
};

}
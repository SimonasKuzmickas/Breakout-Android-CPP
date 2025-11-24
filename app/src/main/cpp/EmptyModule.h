#pragma once
#include "IModule.h"
#include "Graphics.h"
#include <thread>
#include <atomic>

ANativeWindow* g_window = nullptr;
std::thread g_thread;
std::atomic<bool> running = false;

class EmptyModule : public IModule {
public:
    void onStart() override   {
        initGraphics(g_window);
        initSquare();
    }
    void onUpdate() override  {

    }
    void onRender() override  {
        renderRedScreen();
        renderSquare();
        flip();
    }

    void onShutdown() override{

    }
};
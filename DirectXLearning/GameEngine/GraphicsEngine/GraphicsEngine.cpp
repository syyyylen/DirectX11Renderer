#include "GraphicsEngine.h"
#include"../Logger/Log.h"
#include "Renderer/Renderer.h"

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Init()
{
    m_renderer = new Renderer();
    m_renderer->Init();

    LOG("Graphics Engine Initialized");
    return true;
}

bool GraphicsEngine::Release()
{
    m_renderer->Release();
    delete m_renderer;
    
    LOG("Graphics Engine Released");
    return true;
}

Renderer* GraphicsEngine::GetRenderer()
{
    return m_renderer;
}

GraphicsEngine* GraphicsEngine::Get()
{
    static GraphicsEngine engine;
    return &engine;
}

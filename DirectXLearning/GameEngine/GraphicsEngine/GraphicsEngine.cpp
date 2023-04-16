#include "GraphicsEngine.h"
#include"../Logger/Log.h"
#include "Renderer/Renderer.h"
#include <exception>

GraphicsEngine* GraphicsEngine::m_graphicsEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
    try
    {
        m_renderer = new Renderer();
    }
    catch (...) {throw std::exception("Graphics Engine creation failed");}
    
    LOG("Graphics Engine Initialized");
}

GraphicsEngine::~GraphicsEngine()
{
    m_graphicsEngine = nullptr;
    delete m_renderer;
    
    LOG("Graphics Engine Destroyed");
}

Renderer* GraphicsEngine::GetRenderer()
{
    return m_renderer;
}

GraphicsEngine* GraphicsEngine::Get()
{
    return m_graphicsEngine;
}

void GraphicsEngine::Create()
{
    if(m_graphicsEngine)
        throw std::exception("Graphics Engine already created");

    m_graphicsEngine = new GraphicsEngine();
}

void GraphicsEngine::Release()
{
    if(!m_graphicsEngine)
        return;
    
    delete m_graphicsEngine;
}

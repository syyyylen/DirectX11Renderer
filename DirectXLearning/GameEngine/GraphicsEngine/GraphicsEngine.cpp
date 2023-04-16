#include "GraphicsEngine.h"
#include"../Logger/Log.h"
#include "Renderer/Renderer.h"
#include <exception>

GraphicsEngine* GraphicsEngine::m_graphicsEngine = nullptr;

GraphicsEngine::GraphicsEngine()
{
    // Instancing Subsystems
    
    try
    {
        m_renderer = new Renderer();
    }
    catch (...) {throw std::exception("Renderer creation failed");}

    try
    {
        m_textureManager = new TextureManager();
    }
    catch (...) {throw std::exception("Texture Manager creation failed");}
    
    LOG("Graphics Engine Initialized");
}

GraphicsEngine::~GraphicsEngine()
{
    m_graphicsEngine = nullptr;
    delete m_textureManager;
    delete m_renderer;
    
    LOG("Graphics Engine Destroyed");
}

Renderer* GraphicsEngine::GetRenderer()
{
    return m_renderer;
}

TextureManager* GraphicsEngine::GetTextureMgr()
{
    return m_textureManager;
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

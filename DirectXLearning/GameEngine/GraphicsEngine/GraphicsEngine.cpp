#include "GraphicsEngine.h"
#include"../Logger/Log.h"
#include "Renderer/Renderer.h"
#include <exception>
#include "ResourceManager/MeshManager/MeshManager.h"

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

    try
    {
        m_meshManager = new MeshManager();
    }
    catch (...) {throw std::exception("Mesh Manager creation failed");}
    
    LOG("Graphics Engine Initialized");

    void* shaderByteCode = nullptr;
    size_t shaderSize = 0;

    // Vertex Shader/Buffer compilation & creation
    m_renderer->CompileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
    ::memcpy(m_meshLayoutByteCode, shaderByteCode, shaderSize);
    m_meshLayoutSize = shaderSize;
    m_renderer->ReleaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
    m_graphicsEngine = nullptr;
    delete m_meshManager;
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

MeshManager* GraphicsEngine::GetMeshMgr()
{
    return m_meshManager;
}

void GraphicsEngine::GetVertexMeshLayoutShaderByteCodeAndSize(void** ByteCode, size_t* Size)
{
    *ByteCode = m_meshLayoutByteCode;
    *Size = m_meshLayoutSize;
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

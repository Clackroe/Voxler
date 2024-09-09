#include "Scene/Components.hpp"
#include "imgui.h"
#include <PortalEditor.hpp>
namespace Dimensional {

void PortalLayer::OnAttatch()
{
    DM_INFO("Portal Initialized");

    m_EditorCamera = EditorCamera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

    // TEST SCENE SETUP
    MaterialSettings ms;
    ms.Albedo = AssetManager::getTexture("Bell_BaseColor");
    ms.Normal = AssetManager::getTexture("Bell_Normal");
    ms.Metalness = AssetManager::getTexture("Bell_Metallic");
    ms.Roughness = AssetManager::getTexture("Bell_Roughness");

    AssetManager::loadMaterial(ms);
    Ref<Material> mat = AssetManager::getMaterial("Material");

    AssetManager::loadModel(Application::getApp().engineAssetDirectory + "/Models/bell.fbx");
    Ref<Model> mod = AssetManager::getModel("bell");

    AssetManager::loadModel(Application::getApp().engineAssetDirectory + "/Models/Cube.obj");
    Ref<Model> cube = AssetManager::getModel("Cube");

    m_ActiveScene = CreateRef<Scene>();
    m_HierarchyPanel.setSceneContext(m_ActiveScene);

    auto ent1 = m_ActiveScene->createEntity("BellNoTexture");
    ent1.addComponent<MeshRenderer>(mod, mat);

    {
        auto cu = m_ActiveScene->createEntity("Cube1");
        cu.addComponent<MeshRenderer>(cube);
    }

    {
        auto cu = m_ActiveScene->createEntity("Cube2");
        cu.addComponent<MeshRenderer>(cube);
    }

    auto ent2 = m_ActiveScene->createEntity("TexturedBell");
    ent2.addComponent<MeshRenderer>(mod, mat);
    auto& t = ent2.getComponent<TransformComponent>();
    t.Position += 2.0f;

    {
        auto ent3 = m_ActiveScene->createEntity("SpotLight");
        ent3.addComponent<SpotLightComponent>();
        auto& t1 = ent3.getComponent<TransformComponent>();
        auto& l = ent3.getComponent<SpotLightComponent>();
        l.intensity = 15;
        l.cutOff = 12.5;
        l.outerCutOff = 15.0;
        l.constant = 1;
        l.linear = 0.09;
        l.quadratic = 0.032;
        t1.Scale = glm::vec3(0.1);
    }

    {
        auto ent3 = m_ActiveScene->createEntity("PointLight");
        ent3.addComponent<PointLightComponent>();
        auto& t1 = ent3.getComponent<TransformComponent>();
        auto& l = ent3.getComponent<PointLightComponent>();
        l.intensity = 10;
        l.constant = 1;
        l.linear = 0.09;
        l.quadratic = 0.032;
        t1.Scale = glm::vec3(0.1);
    }

    //
}
void PortalLayer::OnDetatch() { }

void PortalLayer::OnUpdate()
{
    m_EditorCamera.Update();
    glm::vec3 p = m_EditorCamera.calcPos();

    m_ActiveScene->beginScene();

    Renderer::beginScene(CameraData { m_EditorCamera.getViewProj(), p });

    m_ActiveScene->updateEditor();

    Renderer::endScene();

    if (Input::IsKeyDown(Key::Escape)) {
        Application::getApp().stopApplication();
    }
}
void PortalLayer::OnImGuiRender()
{

    // Note: Switch this to true to enable dockspace
    static bool useDocking = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &useDocking, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    m_HierarchyPanel.renderImGui();

    // Viewport
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if ((viewportPanelSize.x != m_ViewPortSize.x) || (viewportPanelSize.y != m_ViewPortSize.y)) {
        m_EditorCamera.setViewportDimensions(viewportPanelSize.x, viewportPanelSize.y);
        m_ViewPortSize = { viewportPanelSize.x, viewportPanelSize.y };
    }
    Ref<FrameBuffer> buf = Renderer::getFrameBuffer();

    ImGui::Image(reinterpret_cast<ImTextureID>(buf->m_ColorGLId), ImVec2 { viewportPanelSize.x, viewportPanelSize.y }, { 0, 1 }, { 1, 0 });
    ImGui::End();
    //

    ImGui::End();
}
};

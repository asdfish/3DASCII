#include "systems/UISystem.hpp"

#include <imgui.h>
#include <ImGuiFileDialog.h>
#include <glm/gtc/quaternion.hpp>
#include "RenderContext.hpp"
#include <cmath>

void UISystem::Update()
{
    HandleInputLogic();

    DrawMenuBar();
    DrawAssetBrowser();
    DrawSceneViewer();
    DrawInspectorMenu();
    DrawSceneSettingsMenu();
}

void UISystem::DrawMenuBar()
{
    static bool openPopup = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import from .obj file"))
            {
                IGFD::FileDialogConfig config;
                config.path = "."; 
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose .obj File", ".obj", config);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }    

    if (openPopup)
    {
        ImGui::OpenPopup("SavingSplash");
        openPopup = false;
    }

    if (ImGui::BeginPopupModal("SavingSplash", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::Text("Saving Done!");
        if (ImGui::Button("Close")) {ImGui::CloseCurrentPopup();}
        ImGui::EndPopup();
    }
}

void UISystem::DrawAssetBrowser()
{
    if (ImGui::Begin("Asset Browser"))
    {

        ImGui::Text("Assets:");
        
        for (const auto &name : AssetManager::Instance().GetMeshNames())
        {
            ImGui::BeginGroup();
            ImGui::Text("%s", name);
            if (ImGui::Button(("Add##" + name).c_str()))
            {
                AssetManager::Instance().MeshLoad(name);
            }
            if (ImGui::Button(("Delete##" + name).c_str()))
            {
                AssetManager::Instance().RemoveMesh(name);
            }
            ImGui::EndGroup();
            ImGui::SameLine();
        }
        ImGui::End();
    }
}

void UISystem::DrawSceneViewer()
{

    if (ImGui::Begin("Scene Viewer"))
    {
        //Scene Objects
        ImGui::Separator();
        //ImGui::Text("Scene Objects:");
        if (ImGui::BeginTable("SceneObjects", 1))
        {
            ImGui::TableSetupColumn("Objects: ", ImGuiTableColumnFlags_WidthStretch);

            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); 
            for (auto &entity : entities)
            {
                ImGuiStyle style;
                
                ImGui::TableNextColumn();

                if (m_selectedEntity==entity)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 0.0f));
                }
                
                if (ImGui::Button((Coordinator::Instance().GetComponent<ID>(entity).name + "##" + std::to_string(entity)).c_str(), ImVec2(-FLT_MIN, 0)))
                {
                    m_selectedEntity = entity;
                }

                ImGui::PopStyleColor();
                
            }
            ImGui::PopStyleVar();
            ImGui::EndTable();
        }
        ImGui::End();
    }

}
void UISystem::DrawInspectorMenu()
{
    if (ImGui::Begin("Inspector"))
    {
        if (m_selectedEntity != 0)
        {
            ImGui::Text("Identification:");
            std::string name = Coordinator::Instance().GetComponent<ID>(m_selectedEntity).name;
            ImGui::Text("Name: %s", name);
            ImGui::Text("ID: %d", m_selectedEntity);

            ImGui::Separator();

            // Display Transform
            ImGui::Text("Transform:");
            Transform &transform = Coordinator::Instance().GetComponent<Transform>(m_selectedEntity);
            glm::vec3 rotation = glm::eulerAngles(transform.rotation);
            ImGui::DragFloat3("Position", &transform.position.x, 0.05f);
            ImGui::DragFloat3("Rotation", &rotation.x, 0.1f, 0.0f, glm::two_pi<float>(), "%.2f rad", ImGuiSliderFlags_WrapAround);
            transform.rotation = glm::quat(rotation);
            transform.ResetMatrixTransform(); 
        }
        ImGui::End();
    }
}

void UISystem::DrawSceneSettingsMenu()
{
    if(ImGui::Begin("Scene Settings Menu"))
    {    
        ImGui::DragFloat("FOV", &RenderContext::Instance().FOV);
        ImGui::DragFloat("Near Plane", &RenderContext::Instance().nearPlane);
        ImGui::DragFloat("Far Plane", &RenderContext::Instance().farPlane);
        ImGui::DragFloat3("Cam Position", &RenderContext::Instance().camTransform.position.x);
        ImGui::DragFloat3("Cam rot", &RenderContext::Instance().camTransform.rotation.x);
        RenderContext::Instance().ResetProjMatrix();
        ImGui::End();
    }
}

void UISystem::HandleInputLogic()
{

    if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
    {
        auto& RCI = RenderContext::Instance();

        ImGuiIO& io = ImGui::GetIO();
        Transform& transform = RCI.camTransform;
        float scroll = io.MouseWheel;
        ImVec2 mouseDrag = io.MouseDelta;
        if (scroll != 0.f)
        {
            if (io.KeyMods == ImGuiMod_Shift) 
            {
                transform.position += (transform.Forward() * scroll * RCI.scrollVel)/3.f;
                RCI.ResetProjMatrix();
            }
            else if (io.KeyMods == ImGuiMod_Ctrl) 
            {
                RCI.FOV += scroll*FastSmoothFalloff(
                    RCI.FOV + scroll * RCI.FOVChangeSpeed, 0.f, 180.f);
                RCI.ResetProjMatrix();
            }
            else 
            {
                transform.position += transform.Forward()*scroll*RCI.scrollVel;
                RCI.ResetViewMatrix();
            }
        }
        float& pitch = RenderContext::Instance().camPitch;
        float& yaw  = RenderContext::Instance().camYaw;
        if (mouseDrag[0] != 0)
        {
            //Middle mouse down
            if (io.MouseDown[2]) 
            {
                transform.position += (transform.Right()*-mouseDrag[0]*RCI.panVel/3.f);
                RCI.ResetViewMatrix();
            }
            //Right mouse down
            else if (io.MouseDown[1]) 
            {
                yaw += mouseDrag[0]*RCI.rotVel;
                if (yaw >  glm::pi<float>())yaw=-glm::pi<float>();
                if (yaw < -glm::pi<float>())yaw=+glm::pi<float>();
                
            }
        }
        if (mouseDrag[1] != 0)
        {
            //Middle mouse down
            if (io.MouseDown[2]) 
            {
                transform.position += (-transform.Up()*-mouseDrag[1]*RCI.panVel/3.f);
                RCI.ResetViewMatrix();
            }
            //Right mouse down
            else if (io.MouseDown[1]) 
            {
                pitch += mouseDrag[1]*RCI.rotVel;
                pitch = glm::clamp(pitch, -glm::half_pi<float>()+0.01f, glm::half_pi<float>()-0.01f);
                
            }
        }

        RCI.ResetViewMatrix();

        //WASDQE movements

        if (ImGui::IsKeyDown(ImGuiKey_W))
        {
            transform.position += transform.Forward()*RCI.motionVel*io.DeltaTime;
            RCI.ResetViewMatrix();
        }
        if (ImGui::IsKeyDown(ImGuiKey_S))
        {
            transform.position -= transform.Forward()*RCI.motionVel*io.DeltaTime;
            RCI.ResetViewMatrix();
        }
        if (ImGui::IsKeyDown(ImGuiKey_D))
        {
            transform.position += transform.Right()*RCI.motionVel*io.DeltaTime;
            RCI.ResetViewMatrix();
        }
        if (ImGui::IsKeyDown(ImGuiKey_A))
        {
            transform.position -= transform.Right()*RCI.motionVel*io.DeltaTime;
            RCI.ResetViewMatrix();
        }        
        if (ImGui::IsKeyDown(ImGuiKey_Q))
        {
            transform.position += transform.Up()*RCI.motionVel*io.DeltaTime;
            RCI.ResetViewMatrix();
        }
        if (ImGui::IsKeyDown(ImGuiKey_E))
        {
            transform.position -= transform.Up()*RCI.motionVel*io.DeltaTime;
            RCI.ResetViewMatrix();
        }
    }
}


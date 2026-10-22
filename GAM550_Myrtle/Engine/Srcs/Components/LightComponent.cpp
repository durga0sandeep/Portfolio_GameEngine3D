#include "pch.h"
#include "Components/LightComponent.h"
#include "Memory/AssetLoadManager.h"
#include "Entities/GameObject.h"
#include "Components/ComponentFactory.h"

//std::shared_ptr<Shader> StaticMeshComponent::phong_shader = nullptr;

LightComponent::LightComponent(GameObject* _owner, glm::vec3 position, glm::vec3 color,
    float radius, float intensity, const std::string& meshPath)
    :dataRef(Renderer::instance().creatingPointLights(position, color, intensity, radius, meshPath))
{
    setOwner(_owner);
    //Renderer::instance().initializeObjects(mesh, material, &(owner->getTransform()), renderPassState::DEFERRED);
}

void LightComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("LightComponent",
        [](GameObject* owner, sol::variadic_args args) -> std::shared_ptr<Component>
        {

            glm::vec3 position;
            glm::vec3 color;
            float radius;
            float intensity;
            std::string meshPath;
            // Case 1: All components provided separately
            // (px, py, pz, r, g, b, radius, intensity)
            if (args.size() >= 8 && args[0].is<float>())
            {
                position = glm::vec3(
                    args[0].as<float>(),
                    args[1].as<float>(),
                    args[2].as<float>());

                color = glm::vec3(
                    args[3].as<float>(),
                    args[4].as<float>(),
                    args[5].as<float>());

                radius = args[6].as<float>();
                intensity = args[7].as<float>();
                meshPath = args.size() > 8 ? args[8].as<std::string>() : "Model/Fbx/sphere.fbx";
            }
            else
            {
                // Case 2: vec3 position, vec3 color, float radius, float intensity
                position = args[0].as<glm::vec3>();
                color = args[1].as<glm::vec3>();
                radius = args[2].as<float>();
                intensity = args[3].as<float>();
                meshPath = args.size() > 4 ? args[4].as<std::string>() : "Model/Fbx/sphere.fbx";
            }

            return std::make_shared<LightComponent>(owner, position, color, radius, intensity, meshPath);
        },
        0,
        [](lua_State* L, std::shared_ptr<Component> comp) -> sol::object
        {
            auto derived = std::dynamic_pointer_cast<LightComponent>(comp);
            return sol::make_object(L, derived);
        }
    );

    bindToLua(lua);

}



void LightComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<LightComponent>("LightComponent",
        sol::base_classes, sol::bases<Component>(),
        "movelight", &LightComponent::isMove,
        "setPosition", &LightComponent::setPosition,
        "setScale", &LightComponent::setScale
    );
}

void LightComponent::isMove() const
{

    
}

void LightComponent::setPosition(glm::vec3 pos)
{
    Renderer::instance().getLocalLightList().GetPointLight(dataRef).position = pos;
}

void LightComponent::setScale(glm::vec3 scale)
{
    Renderer::instance().getLocalLightList().setScale(dataRef,scale);
}














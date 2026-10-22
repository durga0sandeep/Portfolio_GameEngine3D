#include "pch.h"
#include "Components/SpotLightComponent.h"
#include <Graphics/Renderer.h>
#include "Memory/AssetLoadManager.h"
#include "Entities/GameObject.h"
#include "Components/ComponentFactory.h"

SpotLightComponent::SpotLightComponent(GameObject* _owner, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color, float radius, float intensity, const std::string& meshPath)
    :dataRef(Renderer::instance().creatingSpotLights(position, color, intensity, radius, meshPath))
{
}
void SpotLightComponent::registerWithFactory(sol::state& lua)
{
    ComponentFactory::instance().registerComponent
    ("SpotLightComponent",
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
                meshPath = args.size() > 8 ? args[8].as<std::string>() : "Model/Fbx/cone.fbx";
            }
            else
            {
                // Case 2: vec3 position, vec3 color, float radius, float intensity
                position = args[0].as<glm::vec3>();
                color = args[1].as<glm::vec3>();
                radius = args[2].as<float>();
                intensity = args[3].as<float>();
                meshPath = args.size() > 4 ? args[4].as<std::string>() : "Model/Fbx/cone.fbx";
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



void SpotLightComponent::bindToLua(sol::state& lua)
{
    lua.new_usertype<SpotLightComponent>("SpotLightComponent",
        sol::base_classes, sol::bases<Component>(),
        "setPosition", &SpotLightComponent::setPosition,
        "setScale", &SpotLightComponent::setScale
    );
}

void SpotLightComponent::setPosition(glm::vec3 pos)
{
    Renderer::instance().getSpotLightList().GetPointLight(dataRef).position = pos;
}

void SpotLightComponent::setScale(glm::vec3 scale)
{
    Renderer::instance().getSpotLightList().setScale(dataRef, scale);
}

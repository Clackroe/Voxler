#ifndef DM_SCENE_H
#define DM_SCENE_H
#include <Core/UUID.hpp>
#include <core.hpp>

#include <entt/entt.hpp>

namespace Dimensional {
class SceneSerializer;
class Entity;
class SceneHierarchy;

class DMCORE_API Scene {
public:
    Scene();
    ~Scene();

    void updateEditor();

    void beginScene();

    Entity createEntity(const std::string& name = std::string());
    Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());

    void destroyEntity(Entity entity);

    Entity getEntityByName(std::string_view name);
    Entity getEntityByID(UUID id);

    template <typename... Components>
    auto getAllEntitiesWith() { return m_Registry.view<Components...>(); }

private:
    template <typename T>
    void onComponentAdded(Entity entity, T& component);
#ifdef DM_WINDOWS
    template <>
    void onComponentAdded<IDComponent>(Entity entity, IDComponent& component);
    template <>
    void onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component);
    template <>
    void onComponentAdded<MeshRenderer>(Entity entity, MeshRenderer& component);
    template <>
    void onComponentAdded<TagComponent>(Entity entity, TagComponent& component);
    template <>
    void onComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component);
    template <>
    void onComponentAdded<SpotLightComponent>(Entity entity, SpotLightComponent& component);
#endif

    entt::registry m_Registry;

    UMap<UUID, entt::entity> m_EntityMap;

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchy;
};

}
#endif

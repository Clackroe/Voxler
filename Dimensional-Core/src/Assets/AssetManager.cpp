#include "Assets/AssetMeta.hpp"
#include <Assets/AssetImporter.hpp>
#include <Assets/AssetManager.hpp>
#include <Assets/AssetRegistrySerializer.hpp>
#include <memory>

namespace Dimensional {

static UMap<std::string, AssetType> s_ExtensionToType = {
    { ".png", AssetType::TEXTURE },
    { ".jpg", AssetType::TEXTURE },
    { ".jpeg", AssetType::TEXTURE },
    { ".PNG", AssetType::TEXTURE },
    { ".JPG", AssetType::TEXTURE },
    { ".JPEG", AssetType::TEXTURE },
    { ".obj", AssetType::MODELSOURCE },
    { ".gltf", AssetType::MODELSOURCE },
    { ".fbx", AssetType::MODELSOURCE },
    { ".FBX", AssetType::MODELSOURCE },
    { ".dmod", AssetType::MODEL },
    { ".glsl", AssetType::SHADER }
};

template <typename T>
Ref<T> AssetManager::getAsset(AssetHandle handle)
{
    // if it is valid registered asset
    // if not, return invalid
    if (!isAssetRegistered(handle)) {
        return nullptr;
    }
    // Check if it needs load
    Ref<Asset> outAsset = nullptr;
    if (isAssetLoaded(handle)) {
        DM_CORE_INFO("RETURNING ASSET");
        outAsset = m_LoadedAssets.at(handle);
    } else {
        // load/return i
        const AssetMetaData& data = getMetaData(handle);
        outAsset = AssetImporter::importAsset(data);
        if (!outAsset) {
            DM_CORE_WARN("ASSETMANAGER | Asset {0} Load Failed from path {1}", (u64)handle, data.sourcePath);
        }
    }
    return std::static_pointer_cast<T>(outAsset);
}

AssetHandle AssetManager::registerAsset(std::filesystem::path path)
{
    AssetHandle handle;
    AssetMetaData meta;
    meta.sourcePath = path;
    auto it = s_ExtensionToType.find(path.extension());
    if (it != s_ExtensionToType.end() && it->second != AssetType::NONE) {
        auto t = it->second;
        meta.type = t;
    } else {
        DM_CORE_WARN("UNABLE TO RECOGNIZE FILE EXTENSION \"{}\"", path.extension().string())
        return 0;
    }

    Ref<Asset> asset = AssetImporter::importAsset(meta);
    if (asset) {
        asset->handle = handle;
        m_LoadedAssets[handle] = asset;
        m_Registry[handle] = meta;
        AssetRegistrySerializer::Serialize("Assets/Registry.dreg", AssetManager::getInstance());
        return handle;
    }
    return 0;
}

const AssetMetaData& AssetManager::getMetaData(AssetHandle handle) const
{
    static AssetMetaData s_NullData;
    auto it = m_Registry.find(handle);
    if (it == m_Registry.end()) {
        return s_NullData;
    }
    return it->second;
}

bool AssetManager::isAssetLoaded(AssetHandle handle) { return m_LoadedAssets.find(handle) != m_LoadedAssets.end(); }
bool AssetManager::isAssetRegistered(AssetHandle handle) { return m_Registry.find(handle) != m_Registry.end(); }

}
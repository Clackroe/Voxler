#include <Assets/AssetManager.hpp>
#include <Rendering/Mesh.hpp>
#include <Rendering/Model.hpp>

namespace Dimensional {

Model::Model(ModelLoadSettings settings)
{
    m_Settings = settings;
    load();
}

void Model::load()
{
    m_Meshes = m_Settings.meshes;
    m_MaterialHandles = m_Settings.matHandles;
}

}

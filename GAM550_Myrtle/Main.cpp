//
#include "pch.h"
#include "Memory/AssetLoadManager.h"
#include "Graphics/Renderer.h"

//
#include "Physics/PhysicsManager.h"
#include <Engine.h>

#include <sol/sol.hpp>
//
// Assimp testing
// Function to load a 3D model using Assimp and print some info about it delete later
void testAssimpModelLoad(const std::string& path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp Error: " << importer.GetErrorString() << "\n";
        return;
    }

    std::cout << "Model loaded successfully!\n";
    std::cout << "Number of meshes: " << scene->mNumMeshes << "\n";
    std::cout << "Root node name: " << scene->mRootNode->mName.C_Str() << "\n";
}

int cppMultiply(int x, int y) {
    std::cout << "C++ multiply called!" << std::endl;
    return x * y;
}


int main()
{

    // Run the Lua file
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

    lua.set_function("cppMultiply", cppMultiply);


    try {
        lua.script_file("test.lua");
    }
    catch (const sol::error& e) {
        std::cerr << "Lua error: " << e.what() << std::endl;
        return 1;
    }

    sol::function multi = lua["cppMultiply"];

    double a = lua["a"].get_or(0.0);
    int result_from_lua = lua["result"].get_or(-1);

    std::cout << "a in C++: " << a << "\n";
    std::cout << "result in C++: " << result_from_lua << "\n";

    //this should initialize everything and start the game loop
    Engine& engine = Engine::instance();
    //run the game loop
    engine.runEngine();
}



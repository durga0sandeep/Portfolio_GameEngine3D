return function(scene)
    print("GAME OBJECT: Creating Ground")
    local ground = scene:addGameObject("Ground")
    local ground_mesh
    local ground_phy

    -- Create 4 walls
    local wall_north = scene:addGameObject("WallNorth")
    local wall_south = scene:addGameObject("WallSouth")
    local wall_east = scene:addGameObject("WallEast")
    local wall_west = scene:addGameObject("WallWest")

    local elevated_floor_2 = scene:addGameObject("elevated2")


    local wall_height = 100
    local wall_thickness = 5
    local arena_size = 200

    -- Set the Lua start function
    ground:setLuaStart(function(self)
        -- Ground
        ground_mesh = ground:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/floor-small-square.obj", 
            "Game/Assets/Textures/colormap.png")
        ground_phy = ground:addComponent("PhysicsComponent", "static", "non_moving", 300, 0.1, 300)
        ground_phy:setPosition(0, -4, 0)
        ground:setScale(300, 0.1, 300)
        ground_phy:setGravityEnabled(false)

        -- North Wall (positive Z)
        local wall_north_mesh = wall_north:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_north_phy = wall_north:addComponent("PhysicsComponent", 
            "static", "non_moving", arena_size, wall_height, wall_thickness)
        wall_north_phy:setPosition(0, -10, arena_size/2)
        wall_north:setScale(arena_size, wall_height, wall_thickness)
        wall_north_phy:setGravityEnabled(false)

        -- South Wall (negative Z)
        local wall_south_mesh = wall_south:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_south_phy = wall_south:addComponent("PhysicsComponent", 
            "static", "non_moving", arena_size, wall_height, wall_thickness)
        wall_south_phy:setPosition(0,-10, -arena_size/2)
        wall_south:setScale(arena_size, wall_height, wall_thickness)
        wall_south_phy:setGravityEnabled(false)

        -- East Wall (positive X)
        local wall_east_mesh = wall_east:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_east_phy = wall_east:addComponent("PhysicsComponent", 
            "static", "non_moving", wall_thickness, wall_height, arena_size)
        wall_east_phy:setPosition(arena_size/2, -10, 0)
        wall_east:setScale(wall_thickness, wall_height, arena_size)
        wall_east_phy:setGravityEnabled(false)

        -- West Wall (negative X)
        local wall_west_mesh = wall_west:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_west_phy = wall_west:addComponent("PhysicsComponent", 
            "static", "non_moving", wall_thickness, wall_height, arena_size)
        wall_west_phy:setPosition(-arena_size/2, -10, 0)
        wall_west:setScale(wall_thickness, wall_height, arena_size)
        wall_west_phy:setGravityEnabled(false)

        --elevated floor 2

        local elevated_floor_2_mesh = elevated_floor_2:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local elevated_floor_2_phy = elevated_floor_2:addComponent("PhysicsComponent", 
            "static", "non_moving", 75, 25, arena_size)
        elevated_floor_2_phy:setPosition(arena_size/2, -10, 0)
        elevated_floor_2:setScale(150, 25, arena_size)


        -- Start all walls
        wall_north:start()
        wall_south:start()
        wall_east:start()
        wall_west:start()
    end)

    ground:start()
    return ground
end

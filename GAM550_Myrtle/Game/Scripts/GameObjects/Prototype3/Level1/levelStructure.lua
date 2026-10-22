return function(scene)
    print("GAME OBJECT: Creating Ground")
    local ground_start = scene:addGameObject("GroundStart")
    local ground_end = scene:addGameObject("GroundEnd")
    local ground_mesh_start
    local ground_phy_start
    local ground_mesh_end
    local ground_phy_end
    
    -- Create 4 walls
    local wall_north = scene:addGameObject("WallNorth")
    local wall_south = scene:addGameObject("WallSouth")
    local wall_east = scene:addGameObject("WallEast")
    local wall_west = scene:addGameObject("WallWest")
    local wall_height = 100
    local wall_thickness = 5
    local arena_width = 150
    local arena_length = 600
    local ground_section_length = 70
    
    -- Set the Lua start function for start ground
    ground_start:setLuaStart(function(self)
        -- Ground at the start (negative Z end)
        ground_mesh_start = ground_start:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/floor-small-square.obj", 
            "Game/Assets/Textures/colormap.png")
        ground_phy_start = ground_start:addComponent("PhysicsComponent", "static", "non_moving", arena_width, 0.1, 32)
        -- Position at the start: -arena_length/2 + ground_section_length/2
        ground_phy_start:setPosition(0, -4, -arena_length/2 + ground_section_length/2)
        ground_start:setScale(arena_width, 0.1, ground_section_length)
        ground_phy_start:setGravityEnabled(false)
    end)
    
    -- Set the Lua start function for end ground
    ground_end:setLuaStart(function(self)
        -- Ground at the end (positive Z end)
        ground_mesh_end = ground_end:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/floor-small-square.obj", 
            "Game/Assets/Textures/colormap.png")
        ground_phy_end = ground_end:addComponent("PhysicsComponent", "static", "non_moving", arena_width, 0.1, 32+20)
        -- Position at the end: arena_length/2 - ground_section_length/2
        ground_phy_end:setPosition(0, -4, arena_length/2 - (ground_section_length)/2)
        ground_end:setScale(arena_width, 0.1, ground_section_length + 20)
        ground_phy_end:setGravityEnabled(false)
        
        -- Setup walls here
        -- North Wall (positive Z)
        local wall_north_mesh = wall_north:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_north_phy = wall_north:addComponent("PhysicsComponent", 
            "static", "non_moving", arena_width, wall_height, wall_thickness)
        wall_north_phy:setPosition(0, -10, arena_length/2)
        wall_north:setScale(arena_width, wall_height, wall_thickness)
        wall_north_phy:setGravityEnabled(false)
        
        -- South Wall (negative Z)
        local wall_south_mesh = wall_south:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_south_phy = wall_south:addComponent("PhysicsComponent", 
            "static", "non_moving", arena_width, wall_height, wall_thickness)
        wall_south_phy:setPosition(0, -10, -arena_length/2)
        wall_south:setScale(arena_width, wall_height, wall_thickness)
        wall_south_phy:setGravityEnabled(false)
        
        -- East Wall (positive X)
        local wall_east_mesh = wall_east:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_east_phy = wall_east:addComponent("PhysicsComponent", 
            "static", "non_moving", wall_thickness, wall_height, arena_length)
        wall_east_phy:setPosition(arena_width/2, -10, 0)
        wall_east:setScale(wall_thickness, wall_height, arena_length)
        wall_east_phy:setGravityEnabled(false)
        
        -- West Wall (negative X)
        local wall_west_mesh = wall_west:addComponent("StaticMeshComponent", 
            "Game/Assets/Models/OBJformat/shape-cube-rounded.obj", 
            "Game/Assets/Textures/colormap.png")
        local wall_west_phy = wall_west:addComponent("PhysicsComponent", 
            "static", "non_moving", wall_thickness, wall_height, arena_length)
        wall_west_phy:setPosition(-arena_width/2, -10, 0)
        wall_west:setScale(wall_thickness, wall_height, arena_length)
        wall_west_phy:setGravityEnabled(false)
        
        -- Start all walls
        wall_north:start()
        wall_south:start()
        wall_east:start()
        wall_west:start()
    end)
    
    ground_start:start()
    ground_end:start()
    return ground_start
end

return function(scene, id, position)
    local name = "box_" .. tostring(id)
    print("GAME OBJECT: Creating " .. name)
    local box = scene:addGameObject(name)
    local mesh_component
    local phy 
    
    -- Set collision callback AFTER component is created
    box:setLuaStart(function(self)
        print("GAME OBJECT: Initializing " .. name)
        mesh_component = box:addComponent("StaticMeshComponent", "Model/Obj/box.obj", "Texture/sample.png")
        phy = box:addComponent("PhysicsComponent", "dynamic", "moving", 1, 1, 1)
        
        -- Use provided position
        local x = position and position.x or 0
        local y = position and position.y or 100
        local z = position and position.z or 0
        phy:setPosition(x, y, z)

        box:setScale(1, 1, 1)
        phy:setGravityEnabled(true)
        
        phy:setOnCollision(function(self, contactPoint, normal, depth, otherBody)
            print(name .. " COLLISION!")
            print("Contact Y: " .. contactPoint.y)
            print("Normal Y: " .. normal.y)
            if normal.y > 0.7 then
                print("Hit ground!")
            end
        end)
    end)
    
    box:start()
    return box
end

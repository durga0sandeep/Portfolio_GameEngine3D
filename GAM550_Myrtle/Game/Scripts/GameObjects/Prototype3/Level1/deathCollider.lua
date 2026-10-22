return function(scene)
    print("GAME OBJECT: Creating Death Collider")
    local death_collider = scene:addGameObject("DeathCollider")
    local death_phy
    
    -- Arena dimensions (should match your ground script)
    local arena_width = 150
    local arena_length = 450
    local collider_height = 10
    local death_zone_y = -50  -- Position below the arena
    
    death_collider:setLuaStart(function(self)
        -- Create a large trigger collider below the arena
        death_phy = death_collider:addComponent("PhysicsComponent", 
            "static", "trigger", arena_width, collider_height, arena_length)
        death_phy:setPosition(0, death_zone_y, 0)
        death_collider:setScale(arena_width, collider_height, arena_length)
        death_phy:setGravityEnabled(false)

        death_phy:setOnCollision(function(self, contactPoint, normal, depth, otherComponent)
            other_obj = otherComponent:getOwnerObject()
            print("Object detected: " .. other_obj:getName())
            otherComponent:setPosition(0, 10, -250)  -- Respawn at start
            otherComponent:setLinearVelocity(0, 0, 0)  -- Reset velocity
            otherComponent:setAngularVelocity(0, 0, 0)  -- Reset rotation
        end)
        print("Death collider created at Y = " .. death_zone_y)
    end)
    
    death_collider:start()
    return death_collider
end

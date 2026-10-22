/*!****************************************************************************
 * file  : Animation.h
 * brief : class for Animation, store keyframe and interpolate
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/

#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <unordered_map>
#include <map>
#include "mySkeleton.h"
#include "AssimpGLMHelpers.h"

// vqs
struct Keyframe {
    float time;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

struct BoneChannel {
    std::string nodeName;              // aiNodeAnim::mNodeName
    std::vector<Keyframe> keyframes;   // merged VQS keyframes
};

class Animation {
public:
    //Animation() = default;
    Animation(const aiAnimation* anim,
        std::vector<BoneInfo>& boneInfoMap, std::unordered_map<std::string, int>& name_index);
	void setName(std::string n) { name = n; }
	void setDuration(float d) { duration = d; }
    void setTicksPerSecond(float t) { ticksPerSecond = t; }
    const std::string& getName() const { return name; }
    float getDuration() const { return duration; }
    float getTicksPerSecond() const { return ticksPerSecond; }
    glm::mat4 getLocalTransform(const std::string& nodeName, float time) const;
	void restart() { currentFrame = 0; }

private:
	int currentFrame = 0;
    std::string name;
    float duration = 0;
    float ticksPerSecond = 0;
    std::map<std::string, BoneChannel> channels;
    BoneChannel readChannel(aiNodeAnim* channel);

    glm::mat4 composeTransform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& sca) const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
        glm::mat4 R = glm::mat4_cast(rot);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), sca);
        return T * R * S;
    }
};


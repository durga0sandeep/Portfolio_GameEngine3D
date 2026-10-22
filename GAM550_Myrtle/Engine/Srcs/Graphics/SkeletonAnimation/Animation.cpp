#include "pch.h"
#include "Graphics/SkeletalAnimation/Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>

/**
 * $BRIEF
 * code to create the Animation from ai Node, load all channel keyframe
 * \param anim
 * \param boneInfoMap
 * \param name_index
 */
Animation::Animation(const aiAnimation* anim, std::vector<BoneInfo>& boneInfoMap, std::unordered_map<std::string, int>& name_index)
{
    // Store animation metadata
    name = anim->mName.C_Str();// Animation name
    duration = static_cast<float>(anim->mDuration);// Total duration in ticks
    ticksPerSecond = static_cast<float>(anim->mTicksPerSecond) != 0 ? static_cast<float>(anim->mTicksPerSecond) : 25.0f;// Use provided tick rate, or default to 25.0 if missing



    // Build channels
     // Iterate through all animation channels (one per bone/node)
    for (unsigned int i = 0; i < anim->mNumChannels; i++) {
        aiNodeAnim* channel = anim->mChannels[i];// Raw Assimp channel
        BoneChannel boneChannel = readChannel(channel);// Convert to internal format

        // Ensure boneInfoMap has an entry for this node, prob not going to happen
        if (name_index.find(boneChannel.nodeName) == name_index.end()) {
            // just set to indentity for now
            BoneInfo info(glm::mat4(1.0f));
            // set name
            info.name = boneChannel.nodeName;
            name_index[boneChannel.nodeName] = static_cast<int>(boneInfoMap.size()); // Map name to index
            boneInfoMap.push_back(info); // Add to bone info list
        }

        // Store the channel under its node name for later sampling
        channels[boneChannel.nodeName] = boneChannel;
    }
}

/**
 * $BRIEF
 * get local Tranform with lerp at time on a bone
 * \param nodeName
 * \param time
 * \return \b 
 */
glm::mat4 Animation::getLocalTransform(const std::string& nodeName, float time) const
{
    auto it = channels.find(nodeName);
    if (it == channels.end()) {
        return glm::mat4(1.0f); // no animation for this node
    }

    if (it->second.keyframes.empty()) return glm::mat4(1.0f);

    // Clamp time into animation duration
    float animTime = fmod(time, duration);
    if (animTime < 0.0f) animTime += duration;

    // Find keyframes
    const Keyframe* kf1 = nullptr;
    const Keyframe* kf2 = nullptr;
    for (size_t i = 0; i < it->second.keyframes.size() - 1; i++) {
        if (animTime >= it->second.keyframes[i].time &&
            animTime <= it->second.keyframes[i + 1].time) {
            kf1 = &it->second.keyframes[i];
            kf2 = &it->second.keyframes[i + 1];
            break;
        }
    }
    if (!kf1 || !kf2) {
        // before first or after last
        const Keyframe& kf = (animTime < it->second.keyframes.front().time)
            ? it->second.keyframes.front()
            : it->second.keyframes.back();
        return composeTransform(kf.position, kf.rotation, kf.scale);
    }

    // Interpolate
    float delta = kf2->time - kf1->time;
    float t = (delta > 0.0f) ? (animTime - kf1->time) / delta : 0.0f;
    t = glm::clamp(t, 0.0f, 1.0f);

    glm::vec3 pos = glm::mix(kf1->position, kf2->position, t);
    glm::quat rot = glm::slerp(kf1->rotation, kf2->rotation, t);
    //glm::vec3 sca = glm::mix(kf1->scale, kf2->scale, t);
    glm::vec3 sca = kf1->scale * glm::pow(kf2->scale / kf1->scale, glm::vec3(t));

    return composeTransform(pos, rot, sca);
}

BoneChannel Animation::readChannel(aiNodeAnim* channel)
{
    BoneChannel bc;
    bc.nodeName = channel->mNodeName.C_Str();

    unsigned int numKeys = std::max({ channel->mNumPositionKeys,
                                     channel->mNumRotationKeys,
                                     channel->mNumScalingKeys });

    for (unsigned int i = 0; i < numKeys; i++) {
        Keyframe kf;
        kf.time = 0.0f;
        kf.position = AssimpGLMHelpers::GetGLMVec(channel->mPositionKeys[i].mValue);
        kf.rotation = AssimpGLMHelpers::GetGLMQuat(channel->mRotationKeys[i].mValue);
        kf.scale = AssimpGLMHelpers::GetGLMVec(channel->mScalingKeys[i].mValue);

        kf.time = static_cast<float>(channel->mPositionKeys[i].mTime);

        bc.keyframes.push_back(kf);
    }
    return bc;
}

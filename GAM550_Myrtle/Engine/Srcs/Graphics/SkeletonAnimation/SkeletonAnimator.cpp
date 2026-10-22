#include "pch.h"
#include "Graphics/SkeletalAnimation/SkeletonAnimator.h"
#include <set>
#include <iostream>
#include <Graphics/SkeletalAnimation/Animation.h>


struct BoneAnimation {
    std::string boneName;
    std::vector<Keyframe> keyframes;
};

SkeletonAnimator::SkeletonAnimator()
{
}

/**
 * $BRIEF
 * load a animation with a path
 * \param path
 * \return \b 
 */
bool SkeletonAnimator::init(const std::string& path)
{
    multiplyer = 1.0f;
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate
        | aiProcess_GenSmoothNormals
        | aiProcess_FlipUVs
        | aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->HasAnimations()) {
        throw std::runtime_error("Failed to load animation from FBX.");
        //assert(false);
    }

    for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        createAnimation(scene->mAnimations[i]);
	}
	return true;
}

void SkeletonAnimator::createAnimation(aiAnimation* aiAnim)
{
    // process unify boneAnimations into animations
    int size = static_cast<int>(skeleton->m_boneInfo.size());
    animations.push_back(std::make_shared<Animation>(aiAnim,skeleton->m_boneInfo,skeleton->name_index));

    currentAnimation = animations.back();

}

void SkeletonAnimator::resetAnimation()
{
    currentTime = 0.0;
    if (skeleton)
    {
        skeleton->applyBindPose(); // Reset all bones to their rest transform
	}
}

/**
 * $BRIEF
 * this will update the skeleton with Animation.
 * \param deltaTime
 */
void SkeletonAnimator::update(float deltaTime)
{
    if (!currentAnimation || !skeleton) return;

    // Update current time
    currentTime += deltaTime * currentAnimation->getTicksPerSecond() * 2;

    if (isLooping) {
        currentTime = fmod(currentTime, currentAnimation->getDuration());
    }
    else if (currentTime > currentAnimation->getDuration()) {
        //stop(false);
        return;
    }

    //update all node that need animated
    for(int i=0; i< skeleton->m_boneInfo.size();i++)
    {
        auto it = skeleton->nodeMap.find(skeleton->m_boneInfo[i].name);
        glm::mat4 matrix;
        if (it != skeleton->nodeMap.end()) {
            matrix =currentAnimation->getLocalTransform(skeleton->m_boneInfo[i].name, currentTime);
            it->second->setLocalTransform(matrix);
        }
        else
        {
            std::cout << "There is something wrong in Skeleton Animator update!" << std::endl;
            std::cout << skeleton->m_boneInfo[i].name << " Not Found!" << std::endl;
        }
    }
}



void SkeletonAnimator::play(std::string name, bool loop)
{
    if (animationMap.find(name) != animationMap.end()) {
        currentAnimation = animationMap[name];
        isLooping = loop;
        currentTime = 0.0;
    }
}

void SkeletonAnimator::play(int name, bool loop)
{
    currentAnimation = animations[name];
    isLooping = loop;
    currentTime = 0.0;
}

void SkeletonAnimator::setLoop(bool loop)
{
    currentTime = 0.0;
    isLooping = loop;
}

void SkeletonAnimator::stop(bool resetToBindPose) {
    currentTime = 0.0f;
    isLooping = false;
    currentAnimation = nullptr;
    isStop = true;
    if (skeleton && resetToBindPose) {
        skeleton->applyBindPose();
    }
}

void SkeletonAnimator::setstop(bool state)
{
    isStop = state;
}

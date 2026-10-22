/*!****************************************************************************
 * file  : Skeleton Animator
 * brief : class to load and control the Animation
 * it should be a 1 to 1 relation with Skeleton
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <string>
#include <vector>
#include "Graphics/SkeletalAnimation/Animation.h"
#include "mySkeleton.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class SkeletonAnimator
{
public:
	SkeletonAnimator();
	
	bool init(const std::string& path);
	void setSkeleton(std::shared_ptr<mySkeleton> skel) { skeleton = skel; }
	void play(std::string name, bool loop);
	void play(int name, bool loop);
	void setLoop(bool loop);
	void stop(bool resetToBindPose);
	void setstop(bool state);
	//void stop();
	
	void update(float deltaTime);
private:
    float   multiplyer;
	void resetAnimation();
	void createAnimation(aiAnimation* anilist);
	bool isLooping = true;
	bool isStop = false;
	// current time in the animation
	float currentTime = 0.0;
	std::shared_ptr<Animation> currentAnimation = nullptr;

	// the skeleton we are animating
	std::shared_ptr<mySkeleton> skeleton = nullptr;
	//real storage of animations
	std::vector<std::shared_ptr<Animation>> animations;
	std::unordered_map<std::string, std::shared_ptr<Animation>> animationMap;
};


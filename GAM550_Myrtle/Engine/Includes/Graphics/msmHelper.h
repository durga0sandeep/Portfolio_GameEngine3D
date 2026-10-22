#pragma once
class msmHelper
{
public:
    GLuint bufferId;
    int bindpoint;
    int Size = 101;
    // Kernel size variables
    int blurWidth = 2; // Initial half-width of the kernel
    const int maxBlurWidth = 100; // Max half-width
    const int minBlurWidth = 1;   // Min half-width
    std::vector<float> weights; // Kernel weights (fixed size of 101)

    bool Flag;

    msmHelper();
    void createUniformBuffer();
    void updateWeights();

    void incrementKernal();
    void decrementKernal();
    int  kernalValue();

};


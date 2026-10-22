#include "pch.h"
#include "Graphics/msmHelper.h"

msmHelper::msmHelper() : weights(101, 0.0f) {

    Flag = false;

    bindpoint = 0;

}


void msmHelper::createUniformBuffer() {

    Flag = true;
    glGenBuffers(1, &bufferId);

    updateWeights();

    //
    //glBindBuffer(GL_UNIFORM_BUFFER, blockID)
    //    glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, bufferID)
    //    glBufferData(GL_UNIFORM_BUFFER, <#bytes>, <data pointer>, GL_STATIC_DRAW)
    //
}

//
//change the logic
void msmHelper::updateWeights() {

    int ReqWidth = (2 * blurWidth) + 1;// 2*1 + 1 => 3
    double s = blurWidth / 2.0;

    for (int i = -blurWidth; i <= blurWidth; ++i) {
        weights[i + blurWidth] = std::exp((-0.5) * std::pow(i / s, 2));
    }

    double sum = 0.0;
    for (int i = -blurWidth; i <= blurWidth; ++i) {
        sum += weights[i + blurWidth];
    }

    for (int i = -blurWidth; i <= blurWidth; ++i) {

        weights[i + blurWidth] /= sum;

    }

    //std::cout <<"WEIGHTS"<< std::endl;
    //for (int i = 0; i < (2 * (blurWidth)+1); i++) {
    //    std::cout << weights[i] << std::endl;
    //}


    if (Flag == true) {

        glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, bufferId);
        glBufferData(GL_UNIFORM_BUFFER, weights.size() * sizeof(float), weights.data(), GL_STATIC_DRAW);
        //std::cout<< "SIZEOF --> " << (weights.size() * sizeof(float)) << std::endl;
    }
}


void msmHelper::incrementKernal() {
    blurWidth++;
    updateWeights();
}

void msmHelper::decrementKernal() {
    blurWidth--;
    updateWeights();
}

int  msmHelper::kernalValue() {

    return blurWidth;
}







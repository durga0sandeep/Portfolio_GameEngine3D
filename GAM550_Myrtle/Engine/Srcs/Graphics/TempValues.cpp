#include "pch.h"
#include "Graphics/TempValues.h"

/*
glm::mat4 TempValues::Translate(const float x, const float y, const float z)
{
    glm::mat4 T = glm::mat4(1.0);
    T[3][0] = x;
    T[3][1] = y;
    T[3][2] = z;

    return T;
}
*/

TempValues::TempValues()
{

    
}

void TempValues::setup()
{

    BoxPolygons = new Box();
    FloorPolygons = new Planez(10.0, 10);
    //ScreenQuadPolys = new Quad(1);


    floor = new Object(FloorPolygons, 3, glm::vec3(6 * 16 / 255.0, 5.5 * 16 / 255.0, 3 * 16 / 255.0), glm::vec3(0.0, 0.0, 0.0), 3.0f, NULL);//texture
    podium = new Object(BoxPolygons, 3, glm::vec3(6 * 16 / 255.0, 5.5 * 16 / 255.0, 3 * 16 / 255.0), glm::vec3(0.03), 3.0f, NULL);//texture

    ScreenQuad = new Object(ScreenQuadPolys, 3);

    objectRoot = new Object(NULL, 0);
    DS_Root = new Object(NULL, 0);
   

    // @@ Initialize interactive viewing variables here. (spin, tilt, ry, front back, ...)
    spin = 0.0f;  // Spin angle
    tilt = 30.0f; // Tilt angle
    tx = 0.0f;    // Translation in x-axis
    ty = 0.0f;    // Translation in y-axis
    zoom = 25.0f; // Zoom factor 25.0f
    rx = ry * width / height; // Recalculate rx based on aspect ratio
    ry = 0.4f;    // Fixed vertical frustum height
    front = 0.1f; // Front clipping plane 0.5f
    back = 5000.0f; // Back clipping plane 5000.0f;

    //std::cout << "TEMP VALUES" << std::endl;

    lightSpin = 150.0;
    lightTilt = -45.0;
    lightDist = 100.0;

    lightPos = glm::vec3(lightDist * cos(lightSpin * rad) * sin(lightTilt * rad),
        lightDist * sin(lightSpin * rad) * sin(lightTilt * rad),
        lightDist * cos(lightTilt * rad));


    diffuseColor = glm::vec3(0.3f, 0.2f, 0.1f);
    specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    shininess = 120.0f;

    Light = glm::vec3(3.0, 3.0, 3.0);//vec3(0.1, 0.1, 0.1);//
    Ambient = glm::vec3(0.2, 0.2, 0.2);//vec3(0.1, 0.1, 0.1);//vec3(0.2, 0.2, 0.2);

    z_near = 20.0f;
    z_far = 100.0f;

    worldUp = glm::vec3(0.0f, 0.0f, 1.0f);

    lightDir = glm::normalize(lightPos);// 0 -LightPos
    // World up direction (standard "up" is positive Y-axis)
    worldUp = glm::vec3(0.0f, 0.0f, 1.0f);  // World "up" direction (Y-axis)
    // Compute the right direction (perpendicular to both lightDir and worldUp)
    rightDir = glm::normalize(glm::cross(worldUp, lightDir));  // Right direction
    // Compute the up direction (perpendicular to both lightDir and rightDir)
    upDir = glm::normalize(glm::cross(lightDir, rightDir));  // Up direction
    //
    L_ViewMatrix = LookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), worldUp);

    WorldView = Translate(tx, ty, -zoom) * Rotate(0, tilt - 90) * Rotate(2, spin);
    WorldProj = Perspective(rx, ry, front, back);

    Rx = 40 / lightDist;
    Ry = 40 / lightDist;

    L_ProjectionMatrix = Perspective(Rx, Ry, front, back / 5);//back -> 5000

    B = Translate(0.5, 0.5, 0.5) * Scale(0.5, 0.5, 0.5);
    ShadowMatrix = B * L_ProjectionMatrix * L_ViewMatrix;

    WorldInverse = glm::inverse(WorldView);

    //setWorldInverse(WorldInverse);

    width = 750;
    height = 750;

    objectId = 3;


    objectRoot->add(floor, Translate(0.0, 0.0, 0.02));
    objectRoot->add(podium, Translate(0.0, 0, 0));

    DS_Root->add(ScreenQuad, 3);
}



const float pi = 3.14159f;
glm::mat4 TempValues::Rotate(const int i, const float theta)
{
    glm::mat4 R = glm::mat4(1.0f);
    float CosAng = glm::cos(theta * pi / 180);
    float SinAng = glm::sin(theta * pi / 180);

    //theta is angle sent
    if (i == 0) //X Rotation
    {
        R[1][1] = CosAng;
        R[2][1] = -SinAng;
        R[1][2] = SinAng;
        R[2][2] = CosAng;
    }
    else if (i == 1)//Y Rotation
    {
        R[0][0] = CosAng;
        R[2][0] = SinAng;
        R[0][2] = -SinAng;
        R[2][2] = CosAng;
    }
    else if (i == 2)//Z Rotation
    {
        R[0][0] = CosAng;
        R[1][0] = -SinAng;
        R[0][1] = SinAng;
        R[1][1] = CosAng;
    }

    return R;
}

// Return a scale matrix
glm::mat4 TempValues::Scale(const float x, const float y, const float z)
{
    glm::mat4 S = glm::mat4(1.0f);
    S[0][0] = x;
    S[1][1] = y;
    S[2][2] = z;
    return S;
}

// Return a translation matrix
glm::mat4 TempValues::Translate(const float x, const float y, const float z)
{
    glm::mat4 T = glm::mat4(1.0);
    T[3][0] = x;
    T[3][1] = y;
    T[3][2] = z;

    return T;
}

// Returns a perspective projection matrix
glm::mat4 TempValues::Perspective(const float rx, const float ry,
    const float front, const float back)
{
    glm::mat4 P(0.0);
    P[0][0] = 1 / rx;
    P[1][1] = 1 / ry;
    P[2][2] = -((back + front) / (back - front));
    P[3][2] = -((2 * (front * back) / (back - front)));
    P[2][3] = -1;

    return P;
}


//
//re-defined it  
glm::mat4 TempValues::LookAt(const glm::vec3 Eye, const glm::vec3 Center, const glm::vec3 Up)
{
    //Camera Forward
    glm::vec3 V = glm::normalize(Center - Eye);
    //Right Vector
    glm::vec3 A = glm::normalize(glm::cross(V, Up));
    //Camera Up
    glm::vec3 B = glm::cross(A, V);

    //glm::mat4 T = Translate(-Eye.x, -Eye.y, -Eye.z);//Translating the Eye to Origin

    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-Eye.x, -Eye.y, -Eye.z));  // Translate Eye to Origin

    /*//row major
    glm::mat4 Rotation = glm::mat4( A.x, A.y, A.z, 0.0f,
                                    B.x, B.y, B.z, 0.0f,
                                   -V.x, -V.y, -V.z, 0.0f,
                                   0.0f, 0.0f, 0.0f, 1.0f);
    */

    //Column Major
    glm::mat4 Rotation = glm::mat4(A.x, B.x, -V.x, 0.0f,
        A.y, B.y, -V.y, 0.0f,
        A.z, B.z, -V.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);


    glm::mat4 LookAt = Rotation * T;

    return LookAt;

}






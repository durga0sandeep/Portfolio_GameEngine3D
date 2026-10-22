#version 330 core

layout(location = 0) in vec2 aPos;       // Quad vertex position (e.g., -0.5 to 0.5)
layout(location = 1) in vec2 aUV;        // Quad UV (0 to 1)

layout(location = 2) in vec3 iPosition;  // Instance: particle position
layout(location = 3) in float iSize;     // Instance: particle size
layout(location = 4) in int iFrameIndex; // Instance: animation frame
layout(location = 5) in int iTotalFrames;// Instance: total frames in atlas

uniform mat4 view;
uniform mat4 proj;

out vec2 vUV;

void main()
{
    // Compute atlas layout
    int cols = int(ceil(sqrt(iTotalFrames)));
    int rows = int(ceil(float(iTotalFrames) / float(cols)));

    int col = iFrameIndex % cols;
    int row = iFrameIndex / cols;

    vec2 atlasScale = vec2(1.0 / float(cols), 1.0 / float(rows));
    vec2 uvOffset = vec2(float(col), float(row)) * atlasScale;

    vUV = uvOffset + aUV * atlasScale;

    // Extract camera right and up vectors from view matrix
    vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camUp    = vec3(view[0][1], view[1][1], view[2][1]);

    // Billboard offset in world space
    vec3 offset = (aPos.x * camRight + aPos.y * camUp) * iSize;

    vec4 worldPos = vec4(iPosition + offset, 1.0);
    gl_Position = proj * view * worldPos;
}
#version 310 es
//from https://learnopengl.com/Lighting/Basic-Lighting
//and https://github.com/emeiri/ogldev/blob/master/tutorial25_youtube/skinning.vs
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 LocalPos0;
flat out ivec4 BoneIDs0;
out vec4 Weights0;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out highp vec3 FragPos; 
const int MAX_BONES = 34;

uniform mat4 gBones[MAX_BONES];

void main()
{
     mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];

    vec4 PosL = BoneTransform * vec4(Position, 1.0);
    mat4 mvp = projection * view * model;
    gl_Position = mvp* vec4(PosL.xyz,1.0);//replacing posL with Position works
    FragPos=vec3(Position);
    TexCoord0 = TexCoord;
    Normal0 = Normal;
    LocalPos0 = Position;
    BoneIDs0 = BoneIDs;
    Weights0 = Weights;
}
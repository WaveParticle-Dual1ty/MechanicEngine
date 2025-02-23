#version 450 core

layout(location = 0) in vec2 InPosition;
layout(location = 1) in vec2 InTexcoord;

layout(push_constant) uniform PushConsts
{
    mat4 ProjectMat;
} g_PushConsts;

layout(location = 0) out vec2 VS_Texcoord;

void main()
{
    //gl_Position = vec4(InPosition, 0, 1);
    gl_Position = g_PushConsts.ProjectMat * vec4(InPosition, 0, 1);

    VS_Texcoord = InTexcoord;
}
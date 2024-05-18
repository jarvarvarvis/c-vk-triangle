#version 450

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Color;

layout (location = 0) out vec3 vertex_color;

layout (set = 0, binding = 0) uniform camera_buffer
{
    mat4 mvp;
} CameraBuffer;

layout (push_constant) uniform constants
{
	mat4 render_matrix;
} PushConstants;

void main() {
    mat4 transformation_matrix = PushConstants.render_matrix * CameraBuffer.mvp;
	gl_Position = transformation_matrix * vec4(v_Position, 1.0f);
    vertex_color = v_Color;
}

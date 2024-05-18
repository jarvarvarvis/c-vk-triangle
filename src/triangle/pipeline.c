#include "pipeline.h"

#include "push_constants.h"

#include "mesh.h"

#include "../vulkan/shader/module.h"
#include "../vulkan/pipeline/builder.h"
#include "../vulkan/pipeline/layout_builder.h"
#include "../vulkan/pipeline/vertex_input.h"

#include "../vulkan/common.h"

int vkt_create_triangle_pipeline_layout(VktEngine *engine, VktTrianglePipelineLayoutArgs *args, VkPipelineLayout *layout) {
    VktPipelineLayoutBuilder builder = vkt_pipeline_layout_builder_new();

    vkt_pipeline_layout_builder_push_push_constant(&builder, 0, sizeof(VktTrianglePushConstants), VK_SHADER_STAGE_VERTEX_BIT);

    // Use descriptor set if it was supplied in the arguments
    if (args->descriptor_set_layout != NULL) {
        vkt_pipeline_layout_builder_push_descriptor_set_layout(&builder, args->descriptor_set_layout);
    }

    VKT_CHECK(vkt_pipeline_layout_builder_build_layout(&engine->vk_context, &builder, layout));
    vkt_pipeline_layout_builder_destroy(&builder);

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_triangle_pipeline(VktEngine *engine, VkPipelineLayout layout, VkPipeline *pipeline) {
    VktPipelineBuilder builder = vkt_pipeline_builder_new();

    vkt_pipeline_builder_set_viewport_from_extent(&builder, engine->render_image_extent);
    vkt_pipeline_builder_set_scissor_from_extent(&builder, engine->render_image_extent);

    vkt_pipeline_builder_push_dynamic_state(&builder, VK_DYNAMIC_STATE_VIEWPORT);
    vkt_pipeline_builder_push_dynamic_state(&builder, VK_DYNAMIC_STATE_SCISSOR);

    vkt_pipeline_builder_set_pipeline_layout(&builder, layout);

    VkShaderModule vert_module;
    VKT_CHECK(vkt_load_shader_module_from_file(&engine->vk_context, "resources/shaders/triangle.vert.spv", &vert_module));
    VkShaderModule frag_module;
    VKT_CHECK(vkt_load_shader_module_from_file(&engine->vk_context, "resources/shaders/triangle.frag.spv", &frag_module));

    vkt_pipeline_builder_push_shader_stage(&builder, VK_SHADER_STAGE_VERTEX_BIT, vert_module);
    vkt_pipeline_builder_push_shader_stage(&builder, VK_SHADER_STAGE_FRAGMENT_BIT, frag_module);

    VktVertexInputDescription triangle_input_description = vkt_triangle_mesh_get_input_description();
    vkt_pipeline_builder_set_vertex_input_state_from_description(&builder, &triangle_input_description);

    vkt_pipeline_builder_set_input_assembly_state(&builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    vkt_pipeline_builder_set_rasterization_state(&builder, VK_POLYGON_MODE_FILL);
    vkt_pipeline_builder_set_multisampling_state(&builder, VK_SAMPLE_COUNT_1_BIT);
    vkt_pipeline_builder_set_color_blend_attachment_state(&builder);
    vkt_pipeline_builder_set_depth_stencil_state(&builder, true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

    int result = vkt_pipeline_builder_build_pipeline(&engine->vk_context, &builder, engine->present_context.main_render_pass, pipeline);
    vkt_pipeline_builder_destroy(&builder);

    vkt_destroy_shader_module(&engine->vk_context, vert_module);
    vkt_destroy_shader_module(&engine->vk_context, frag_module);

    vkt_destroy_vertex_input_description(&triangle_input_description);

    return result;
}

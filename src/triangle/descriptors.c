#include "descriptors.h"

#include "../vulkan/common.h"

#include "../vulkan/descriptors/descriptors.h"
#include "../vulkan/descriptors/layout_builder.h"
#include "../vulkan/descriptors/pool_builder.h"
#include "../vulkan/descriptors/update_builder.h"
#include <vulkan/vulkan_core.h>

int vkt_create_triangle_descriptors(VktEngine *engine, size_t uniform_sets, VktTriangleDescriptors *descriptors) {
    if (uniform_sets == 0) {
        return VKT_GENERIC_FAILURE;
    }

    // Create descriptor set layout
    VktDescriptorSetLayoutBuilder layout_builder = vkt_descriptor_set_layout_builder_new();

    // We have one uniform buffer at binding index 0, available in the vertex shader
    vkt_descriptor_set_layout_builder_push_uniform_buffer_binding(&layout_builder, 0, VK_SHADER_STAGE_VERTEX_BIT);

    VKT_CHECK(vkt_descriptor_set_layout_builder_build_layout(&engine->vk_context, &layout_builder, &descriptors->set_layout));
    vkt_descriptor_set_layout_builder_destroy(&layout_builder);

    // Create descriptor pool
    VktDescriptorPoolBuilder pool_builder = vkt_descriptor_pool_builder_new();

    // Create a descriptor pool that can hold the specified amount of uniform buffers
    vkt_descriptor_pool_builder_push_size(&pool_builder, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniform_sets);

    VKT_CHECK(vkt_descriptor_pool_builder_build_pool(&engine->vk_context, &pool_builder, &descriptors->descriptor_pool));
    vkt_descriptor_pool_builder_destroy(&pool_builder);

    // Allocate specified amount of uniform descriptor data
    descriptors->uniform_descriptors_count = uniform_sets;
    descriptors->uniform_descriptors = malloc(sizeof(VktTriangleUniformDescriptors) * descriptors->uniform_descriptors_count);

    // Update builder to associate buffer with the descriptor set
    VktDescriptorSetUpdateBuilder update_builder = vkt_descriptor_set_update_builder_new();

    for (size_t i = 0; i < uniform_sets; ++i) {
        // Allocate the buffer
        VKT_CHECK(vkt_allocate_buffer_for_uploads(
            &engine->vk_context,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            sizeof(VktTriangleGlobalUniformData),
            &descriptors->uniform_descriptors[i].buffer
        ));

        // Allocate the descriptor set
        VKT_CHECK(vkt_allocate_descriptors_from_pool(
            &engine->vk_context,
            descriptors->descriptor_pool,
            &descriptors->set_layout, 1,
            &descriptors->uniform_descriptors[i].set
        ));

        // Associate the buffer with the descriptor set
        vkt_descriptor_set_update_builder_push_buffer_write(
            &update_builder,
            descriptors->uniform_descriptors[i].buffer.buffer,
            sizeof(VktTriangleGlobalUniformData),
            descriptors->uniform_descriptors[i].set,
            0,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );
    }

    // Finally do all the updates and destroy the update builder
    vkt_descriptor_set_update_builder_update_sets(&engine->vk_context, &update_builder);
    vkt_descriptor_set_update_builder_destroy(&update_builder);

    return VKT_GENERIC_SUCCESS;
}

void vkt_triangle_descriptors_upload_global_data(VktEngine *engine, VktTriangleDescriptors *descriptors, size_t uniforms_idx, VktTriangleGlobalUniformData *uniform_data) {
    vkt_upload_to_buffer(
        &engine->vk_context,
        &descriptors->uniform_descriptors[uniforms_idx].buffer,
        uniform_data,
        sizeof(VktTriangleGlobalUniformData)
    );
}

void vkt_triangle_descriptors_bind_set(VktEngine *engine, VkPipelineLayout pipeline_layout, VktTriangleDescriptors *descriptors, size_t uniforms_idx) {
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);
    vkCmdBindDescriptorSets(
        frame->main_command_buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline_layout,
        0,
        1,
        &descriptors->uniform_descriptors[uniforms_idx].set,
        0,
        NULL
    );
}

void vkt_destroy_triangle_descriptors(VktEngine *engine, VktTriangleDescriptors *descriptors) {
    for (size_t i = 0; i < descriptors->uniform_descriptors_count; ++i) {
        vkt_destroy_buffer(&engine->vk_context, &descriptors->uniform_descriptors[i].buffer);
    }

    free(descriptors->uniform_descriptors);
    vkt_destroy_descriptor_set_layout(&engine->vk_context, descriptors->set_layout);

    // Destroying the descriptor pool will deallocate all the descriptor sets
    vkt_destroy_descriptor_pool(&engine->vk_context, descriptors->descriptor_pool);
}

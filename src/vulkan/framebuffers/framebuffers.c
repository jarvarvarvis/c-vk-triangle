#include "framebuffers.h"

#include <string.h>

#include "../common.h"

int vkt_create_framebuffers(
    VktVulkanContext *context,
    VktSwapchainImages *swapchain_images,
    VkRenderPass render_pass,
    VkExtent2D image_extent,
    VktFramebuffers *framebuffers
) {
    memset(framebuffers, 0, sizeof(VktFramebuffers));

    framebuffers->framebuffer_count = swapchain_images->image_count;
    framebuffers->framebuffers = malloc(sizeof(VkFramebuffer) * framebuffers->framebuffer_count);

    // Initialize framebuffer create info
    VkFramebufferCreateInfo framebuffer_create_info;
    memset(&framebuffer_create_info, 0, sizeof(VkFramebufferCreateInfo));

    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_create_info.pNext = NULL;

    framebuffer_create_info.renderPass = render_pass;
    framebuffer_create_info.attachmentCount = 1;

    framebuffer_create_info.width = image_extent.width;
    framebuffer_create_info.height = image_extent.height;
    framebuffer_create_info.layers = 1;

    // Create framebuffer for each image in the swapchain
    for (uint32_t i = 0; i < framebuffers->framebuffer_count; ++i) {
        framebuffer_create_info.pAttachments = &swapchain_images->image_views[i];
        VKT_CHECK(vkCreateFramebuffer(
            context->logical_device.vk_device,
            &framebuffer_create_info,
            NULL,
            &framebuffers->framebuffers[i]
        ));
    }

    c_log(C_LOG_SEVERITY_DEBUG, "Created %d framebuffers (one for each image in the swapchain)", framebuffers->framebuffer_count);
    printf("\n");

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_framebuffers(VktVulkanContext *context, VktFramebuffers *framebuffers) {
    for (uint32_t i = 0; i < framebuffers->framebuffer_count; ++i) {
        vkDestroyFramebuffer(context->logical_device.vk_device, framebuffers->framebuffers[i], NULL);
    }
    free(framebuffers->framebuffers);
}

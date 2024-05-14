#include "framebuffers.h"

#include "../common.h"

#include <string.h>

int vkt_create_framebuffers(
    VktVulkanContext *context,
    VktSwapchainImages *swapchain_images,
    VktDepthImage *depth_image,
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

    framebuffer_create_info.width = image_extent.width;
    framebuffer_create_info.height = image_extent.height;
    framebuffer_create_info.layers = 1;

    // Create framebuffer for each image in the swapchain
    for (uint32_t i = 0; i < framebuffers->framebuffer_count; ++i) {
        // Set attachments for this framebuffer (the swapchain image and the depth image)
        VkImageView attachments[] = {
            swapchain_images->image_views[i],
            depth_image->image_view
        };
        framebuffer_create_info.pAttachments = attachments;
        framebuffer_create_info.attachmentCount = 2;

        VKT_CHECK(vkCreateFramebuffer(
            context->logical_device.vk_device,
            &framebuffer_create_info,
            NULL,
            &framebuffers->framebuffers[i]
        ));
    }

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_framebuffers(VktVulkanContext *context, VktFramebuffers *framebuffers) {
    for (uint32_t i = 0; i < framebuffers->framebuffer_count; ++i) {
        vkDestroyFramebuffer(context->logical_device.vk_device, framebuffers->framebuffers[i], NULL);
    }
    free(framebuffers->framebuffers);
}

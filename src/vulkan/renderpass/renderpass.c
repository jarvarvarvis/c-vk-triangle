#include "renderpass.h"

#include "../common.h"

#include <string.h>

VkAttachmentDescription vkt_create_default_renderpass_color_attachment(VkFormat format) {
    VkAttachmentDescription color_attachment;
    memset(&color_attachment, 0, sizeof(VkAttachmentDescription));

    color_attachment.format = format;

    // 1 sample
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // Clear when this attachment is loaded
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

    // Keep the attachment stored when the renderpass ends
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    // We don't care about stencil
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    // The starting layout is irrelevant
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    // After the renderpass ends, the image needs to be in a layout ready for presentation
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    return color_attachment;
}

int vkt_create_default_renderpass(
    VktVulkanContext *context,
    VkFormat format,
    VkRenderPass *render_pass
) {
    VkAttachmentDescription color_attachment = vkt_create_default_renderpass_color_attachment(format);

    // Create reference to the color attachment
    VkAttachmentReference color_attachment_ref;
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Create 1 subpass that references the color attachment
    VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));

    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    // The image life will go something like this:

    // UNDEFINED
    // -> RenderPass Begins
    // -> Subpass 0 begins (Transition to Attachment Optimal)
    // -> Subpass 0 renders
    // -> Subpass 0 ends
    // -> Renderpass Ends (Transitions to Present Source)

    VkRenderPassCreateInfo render_pass_info;
    memset(&render_pass_info, 0, sizeof(VkRenderPassCreateInfo));

    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    // Add the color attachment
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;

    // Add the subpass
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

    // Finally create the render pass
    VKT_CHECK(vkCreateRenderPass(context->logical_device.vk_device, &render_pass_info, NULL, render_pass));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_renderpass(VktVulkanContext *context, VkRenderPass renderpass) {
    vkDestroyRenderPass(context->logical_device.vk_device, renderpass, NULL);
}

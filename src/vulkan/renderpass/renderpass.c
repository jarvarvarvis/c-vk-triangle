#include "renderpass.h"

#include "../common.h"

#include <string.h>

VkAttachmentDescription vkt_create_default_renderpass_color_attachment(VkFormat format) {
    VkAttachmentDescription color_attachment;
    memset(&color_attachment, 0, sizeof(VkAttachmentDescription));

    color_attachment.format = format;

    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    return color_attachment;
}

VkAttachmentDescription vkt_create_default_renderpass_depth_attachment(VkFormat format) {
    VkAttachmentDescription depth_attachment;
    memset(&depth_attachment, 0, sizeof(VkAttachmentDescription));

    depth_attachment.format = format;

    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    return depth_attachment;
}

int vkt_create_default_renderpass(
    VktVulkanContext *context,
    VkFormat color_format,
    VkFormat depth_format,
    VkRenderPass *render_pass
) {
    VkAttachmentDescription color_attachment = vkt_create_default_renderpass_color_attachment(color_format);
    VkAttachmentDescription depth_attachment = vkt_create_default_renderpass_depth_attachment(depth_format);

    // Create reference to the color attachment
    VkAttachmentReference color_attachment_ref;
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Create reference to the depth attachment
    VkAttachmentReference depth_attachment_ref;
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Create 1 subpass that references the color attachment
    VkSubpassDescription subpass;
    memset(&subpass, 0, sizeof(VkSubpassDescription));

    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;

    // Color attachment subpass dependency
    VkSubpassDependency color_subpass_dependency;
    color_subpass_dependency.dependencyFlags = 0;
    color_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    color_subpass_dependency.dstSubpass = 0;
    color_subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    color_subpass_dependency.srcAccessMask = 0;
    color_subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    color_subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Depth attachment subpass dependency
    VkSubpassDependency depth_subpass_dependency;
    depth_subpass_dependency.dependencyFlags = 0;
    depth_subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    depth_subpass_dependency.dstSubpass = 0;
    depth_subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_subpass_dependency.srcAccessMask = 0;
    depth_subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_subpass_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // Setup render pass info
    VkRenderPassCreateInfo render_pass_info;
    memset(&render_pass_info, 0, sizeof(VkRenderPassCreateInfo));

    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    // Set attachments
    VkAttachmentDescription pass_attachments[] = { color_attachment, depth_attachment };
    render_pass_info.attachmentCount = 2;
    render_pass_info.pAttachments = pass_attachments;

    // Add the subpass
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

    // Set subpass dependencies
    VkSubpassDependency subpass_dependencies[] = { color_subpass_dependency, depth_subpass_dependency };
    render_pass_info.dependencyCount = 2;
    render_pass_info.pDependencies = subpass_dependencies;

    // Finally create the render pass
    VKT_CHECK(vkCreateRenderPass(context->logical_device.vk_device, &render_pass_info, NULL, render_pass));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_renderpass(VktVulkanContext *context, VkRenderPass renderpass) {
    vkDestroyRenderPass(context->logical_device.vk_device, renderpass, NULL);
}

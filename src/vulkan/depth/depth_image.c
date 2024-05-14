#include "depth_image.h"

#include "../common.h"

int vkt_create_depth_image(VktVulkanContext *context, VkExtent2D initial_extent, VktDepthImage *depth_image) {
    memset(depth_image, 0, sizeof(VktDepthImage));

    VkExtent3D depth_image_extent;
    depth_image_extent.width = initial_extent.width;
    depth_image_extent.height = initial_extent.height;
    depth_image_extent.depth = 1;

    // Hardcoded 32-bit float depth format
    depth_image->format = VK_FORMAT_D32_SFLOAT;

    // Create the image
    VkImageCreateInfo image_info = vkt_get_2d_image_create_info(
        depth_image->format, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depth_image_extent
    );
    VKT_CHECK(vkt_allocate_gpu_only_image(context, image_info, &depth_image->image));

    // Create the image view
    VkImageViewCreateInfo image_view_info = vkt_get_2d_image_view_create_info(
        depth_image->format, depth_image->image.image, VK_IMAGE_ASPECT_DEPTH_BIT
    );
    VKT_CHECK(vkt_create_image_view(context, image_view_info, &depth_image->image_view));

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_depth_image(VktVulkanContext *context, VktDepthImage *depth_image) {
    vkt_destroy_image_view(context, depth_image->image_view);
    vkt_destroy_image(context, &depth_image->image);
}

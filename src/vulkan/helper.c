#include "helper.h"

VkViewport vkt_helper_viewport_from_extent(VkExtent2D extent) {
    VkViewport viewport;
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = extent.width;
    viewport.height = extent.height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    return viewport;
}

VkRect2D vkt_helper_rect2d_from_extent(VkExtent2D extent) {
    VkRect2D rect;
    rect.offset.x = 0;
    rect.offset.y = 0;
    rect.extent = extent;

    return rect;
}

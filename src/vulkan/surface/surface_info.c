#include "surface_info.h"
#include "surface.h"

#include "../common.h"

#include <stdlib.h>

int vkt_query_surface_info(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VktSurfaceInfo *info
) {
    VKT_CHECK(vkt_query_supported_surface_formats(context, surface, &info->surface_formats, &info->surface_format_count));
    VKT_CHECK(vkt_query_surface_present_modes(context, surface, &info->present_modes, &info->present_mode_count));

    VKT_CHECK(vkt_query_surface_capabilities(context, surface, &info->surface_capabilities));

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_surface_info(VktSurfaceInfo *info) {
    free(info->surface_formats);
    free(info->present_modes);
}

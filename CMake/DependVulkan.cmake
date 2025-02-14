# Depend Vulkan

if(WIN32)
    set(VULKAN_PATH ${PROJ_DEPENDS_DIR}/VulkanSDK/Windows)

    set(VULKAN_INCLUDE_DIR ${VULKAN_PATH}/Include)
    set(VULKAN_LIB_DIR ${VULKAN_PATH}/Lib)
    set(VULKAN_BIN_DIR ${VULKAN_PATH}/Bin)
    set(VULKAN_LIBS_DEBUG
        ${VULKAN_LIB_DIR}/vulkan-1.lib
        ${VULKAN_LIB_DIR}/shaderc_sharedd.lib
        ${VULKAN_LIB_DIR}/spirv-cross-cored.lib
        ${VULKAN_LIB_DIR}/spirv-cross-glsld.lib
        ${VULKAN_LIB_DIR}/SDL2.lib
    )
    set(VULKAN_LIBS_RELEASE
        ${VULKAN_LIB_DIR}/vulkan-1.lib
        ${VULKAN_LIB_DIR}/shaderc_shared.lib
        ${VULKAN_LIB_DIR}/spirv-cross-core.lib
        ${VULKAN_LIB_DIR}/spirv-cross-glsl.lib
        ${VULKAN_LIB_DIR}/SDL2.lib
    )

    set(VULKAN_FILE_TO_COPY_DEBUG
        ${VULKAN_BIN_DIR}/shaderc_sharedd.dll
    )

    set(VULKAN_FILE_TO_COPY_RELEASE
        ${VULKAN_BIN_DIR}/shaderc_sharedd.dll
    )

    set(VULKAN_DESTINATION_DIR_DEBUG "${PROJ_OUTPUT_DIR_DEBUG}")
    set(VULKAN_DESTINATION_DIR_RELEASE "${PROJ_OUTPUT_DIR_RELEASE}")
    file(MAKE_DIRECTORY ${VULKAN_DESTINATION_DIR_DEBUG})
    file(MAKE_DIRECTORY ${VULKAN_DESTINATION_DIR_RELEASE})
    file(COPY ${VULKAN_FILE_TO_COPY_DEBUG} DESTINATION ${VULKAN_DESTINATION_DIR_DEBUG})
    file(COPY ${VULKAN_FILE_TO_COPY_RELEASE} DESTINATION ${VULKAN_DESTINATION_DIR_RELEASE})
elseif(APPLE)
    set(VULKAN_PATH ${PROJ_DEPENDS_DIR}/VulkanSDK/Apple)
    set(VULKAN_INCLUDE_DIR ${VULKAN_PATH}/include)
    set(VULKAN_LIB_DIR ${VULKAN_PATH}/lib)
    set(VULKAN_BIN_DIR ${VULKAN_PATH}/bin)
    set(VULKAN_LIBS_DEBUG
        ${VULKAN_LIB_DIR}/libvulkan.1.dylib
        ${VULKAN_LIB_DIR}/libshaderc_shared.1.dylib
        ${VULKAN_LIB_DIR}/libspirv-cross-core.a
        ${VULKAN_LIB_DIR}/libspirv-cross-glsl.a
    )
    set(VULKAN_LIBS_RELEASE
        ${VULKAN_LIB_DIR}/libvulkan.1.dylib
        ${VULKAN_LIB_DIR}/libshaderc_shared.1.dylib
        ${VULKAN_LIB_DIR}/libspirv-cross-core.a
        ${VULKAN_LIB_DIR}/libspirv-cross-glsl.a
    )
else()
    message(FATAL_ERROR "Unsuport platform Unknown")
endif()

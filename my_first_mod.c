#include <android/log.h>
#include "module/module_core.h"
#include "patchlib/method.h"
#include "tef_api.h"

#define LOG_TAG "MyFirstMod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static const module_info_t g_module_info = {
    .pkg_id = "com.QEllipsis.myfirstmod",
    .name = "My First Test",
    .author = "QEllipsis",
    .version = "1.0.0",
    .version_code = 1,
    .api_version = 1,
    .plugin_dependencies_sizes = 0,
    .plugin_dependencies = NULL
};

static bool test_module_init(module_entry_t *entry) {
    LOGI("Module initialized successfully");
    return true;
}

static bool test_module_cleanup(module_entry_t *entry) {
    return true;
}

static void test_module_hot_reload(module_entry_t *entry) {}

static const module_info_t *test_module_get_info(void) {
    return &g_module_info;
}

static const module_ops_t g_module_ops = {
    .init_module = test_module_init,
    .cleanup_module = test_module_cleanup,
    .hot_reload = test_module_hot_reload,
    .get_info = test_module_get_info
};

API_EXPORT const module_ops_t * API_CALL module_create(void) {
    LOGI("module_create() called");
    return &g_module_ops;
}

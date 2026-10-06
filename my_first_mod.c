#include <android/log.h>
#include "module/module_core.h"
#include "patchlib/method.h"
#include "tef_api.h"

#define LOG_TAG "MyFirstMod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/* 模块信息 */
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

static module_entry_t *g_module_entry = NULL;

/* 定义 Hook 前缀函数（修正1：改为编译器期望的4个参数） */
bool my_hook_prefix(void *method, void **result, const patch_method_signature_t *sig, void *user_data) {
    LOGI("我的 Hook 被触发了！");
    return false; // 返回 false 表示不阻止原函数执行
}

/* 初始化模块 */
static bool test_module_init(module_entry_t *entry) {
    LOGI("Initializing module: %s", entry->info->name);
    g_module_entry = entry;

    LOGI("Module initialized successfully");
    LOGI("Private directory: %s", entry->private_dir);
    LOGI("Logs directory: %s", entry->logs_dir);

    /* 尝试安装 Hook（修正2：删掉多余的最后一个参数，只传3个） */
    patch_hook_id_t hook_id = patchlib_install_prepost_hook(
        NULL,           // 目标函数句柄（暂时传 NULL）
        my_hook_prefix, // 前缀 Hook 函数
        NULL            // 后缀 Hook（可选）
    );

    if (hook_id == PATCH_HOOK_INVALID_ID) {
        LOGE("Hook 安装失败");
    } else {
        LOGI("Hook 安装成功，ID: %d", hook_id);
    }
    return true;
}

/* 清理模块 */
static bool test_module_cleanup(module_entry_t *entry) {
    g_module_entry = NULL;
    return true;
}

/* 热重载 */
static void test_module_hot_reload(module_entry_t *entry) {}

/* 获取模块信息 */
static const module_info_t *test_module_get_info(void) {
    return &g_module_info;
}

/* 模块操作函数表（修正3：去掉字段名的下划线） */
static const module_ops_t g_module_ops = {
    .init_module = test_module_init,
    .cleanup_module = test_module_cleanup,
    .hot_reload = test_module_hot_reload,
    .get_info = test_module_get_info
};

/* 模块入口函数 */
API_EXPORT const module_ops_t * API_CALL module_create(void) {
    LOGI("module_create() called");
    return &g_module_ops;
}

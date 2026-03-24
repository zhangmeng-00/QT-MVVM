#pragma once

/*
 * Tags - Topic 标签定义
 * ============================================================
 * 所有业务相关的 Tag 统一在这里定义
 * 使用方式：
 *   Subscribe(TAG_USER_SCORE, ...);
 *   Publish(TAG_USER_SCORE, value);
 */

// ==== 用户模块 ====
#define TAG_USER_SCORE         "user/score"
#define TAG_USER_LEVEL         "user/level"
#define TAG_USER_LOGIN         "user/login"
#define TAG_USER_LOGOUT        "user/logout"
#define TAG_USER_LOGGED_IN     "user/logged_in"
#define TAG_USER_NAME          "user/name"
#define TAG_USER_MODE          "user/mode"
#define TAG_USER_COUNT         "user/count"
#define TAG_USER_PUBLISH_SCORE "user/publish_score"

// ==== 传感器模块 ====
#define TAG_SENSOR_TEMPERATURE "sensor/temperature"
#define TAG_SENSOR_TARGET_TEMP "sensor/target_temperature"
#define TAG_SENSOR_GAIN         "sensor/gain"
#define TAG_SENSOR_PUBLISH_TEMP "sensor/publish_temperature"

// ==== 日志系统 ====
#define TAG_LOGGING            "user/logging"
#define TAG_LOG_LIST           "user/logList"

// ==== Modbus 模块宏 ====
// 使用方式：TAG_MODBUS_STATE(pid_pressure, 0, value)
// 输出：pid_pressure/0/state/value
#define TAG_MODBUS_STATE(module, instance, key) #module "/" #instance "/state/" #key
#define TAG_MODBUS_CMD(module, instance, key)   #module "/" #instance "/cmd/" #key
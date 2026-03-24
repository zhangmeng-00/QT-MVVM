#pragma once

#include "AppConfig.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QDebug>

/*
 * ConfigLoader - 配置文件加载器
 * ============================================================
 * 从 JSON/INI 文件加载配置
 *
 * 支持格式：
 *   - JSON: config.json
 *   - INI:  config.ini
 *
 * 使用方式：
 *   AppConfig config = ConfigLoader::loadFromJsonFile("config/app.json");
 */
class ConfigLoader {
public:
    // 从 JSON 文件加载
    static AppConfig loadFromJsonFile(const QString& filePath) {
        AppConfig config;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "ConfigLoader: Cannot open file" << filePath;
            return config;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "ConfigLoader: Invalid JSON file" << filePath;
            return config;
        }

        QJsonObject root = doc.object();
        return parseJson(root);
    }

    // 从 JSON 字符串加载
    static AppConfig loadFromJson(const QString& json) {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "ConfigLoader: Invalid JSON string";
            return AppConfig();
        }
        return parseJson(doc.object());
    }

    // 从 INI 文件加载（使用 QSettings）
    static AppConfig loadFromIniFile(const QString& filePath) {
        AppConfig config;

        QSettings settings(filePath, QSettings::IniFormat);

        // Modbus 配置
        config.modbus.host = settings.value("Modbus/Host", "127.0.0.1").toString();
        config.modbus.port = settings.value("Modbus/Port", 502).toInt();
        config.modbus.timeout = settings.value("Modbus/Timeout", 1000).toInt();
        config.modbus.pollInterval = settings.value("Modbus/PollInterval", 200).toInt();

        // 日志配置
        config.logging.minLevel = settings.value("Logging/MinLevel", "Info").toString();
        config.logging.maxCount = settings.value("Logging/MaxCount", 1000).toInt();
        config.logging.databasePath = settings.value("Logging/DatabasePath", "logs.db").toString();

        // 通用配置
        config.appName = settings.value("General/AppName", "MediatorMVAQt").toString();
        config.version = settings.value("General/Version", "1.0.0").toString();

        return config;
    }

    // 保存到 JSON 文件
    static bool saveToJsonFile(const AppConfig& config, const QString& filePath) {
        QJsonObject root = toJson(config);

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "ConfigLoader: Cannot write to file" << filePath;
            return false;
        }

        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

private:
    static AppConfig parseJson(const QJsonObject& root) {
        AppConfig config;

        // Modbus 配置
        if (root.contains("modbus")) {
            QJsonObject modbus = root["modbus"].toObject();
            config.modbus.host = modbus["host"].toString("127.0.0.1");
            config.modbus.port = modbus["port"].toInt(502);
            config.modbus.timeout = modbus["timeout"].toInt(1000);
            config.modbus.pollInterval = modbus["pollInterval"].toInt(200);
        }

        // 日志配置
        if (root.contains("logging")) {
            QJsonObject logging = root["logging"].toObject();
            config.logging.minLevel = logging["minLevel"].toString("Info");
            config.logging.maxCount = logging["maxCount"].toInt(1000);
            config.logging.databasePath = logging["databasePath"].toString("logs.db");
        }

        // 通用配置
        if (root.contains("general")) {
            QJsonObject general = root["general"].toObject();
            config.appName = general["appName"].toString("MediatorMVAQt");
            config.version = general["version"].toString("1.0.0");
        }

        return config;
    }

    static QJsonObject toJson(const AppConfig& config) {
        QJsonObject root;

        // Modbus
        QJsonObject modbus;
        modbus["host"] = config.modbus.host;
        modbus["port"] = config.modbus.port;
        modbus["timeout"] = config.modbus.timeout;
        modbus["pollInterval"] = config.modbus.pollInterval;
        root["modbus"] = modbus;

        // Logging
        QJsonObject logging;
        logging["minLevel"] = config.logging.minLevel;
        logging["maxCount"] = config.logging.maxCount;
        logging["databasePath"] = config.logging.databasePath;
        root["logging"] = logging;

        // General
        QJsonObject general;
        general["appName"] = config.appName;
        general["version"] = config.version;
        root["general"] = general;

        return root;
    }
};

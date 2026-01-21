#include "ModbusMasterModel.h"
#include "ModbusModuleBase.h"

/*
 * 构造函数
 */
ModbusMasterModel::ModbusMasterModel(QObject* parent)
    : QObject(parent)
{
    /*
     * 将定时器 timeout 信号
     * 连接到 poll() 槽函数
     */
    connect(&m_pollTimer,
            &QTimer::timeout,
            this,
            &ModbusMasterModel::poll);
}

void ModbusMasterModel::startTcp(
    const QString& host,
    int port)
{
    /*
     * 创建 Modbus TCP Client
     *
     * 注意：
     * - 只创建一次
     * - 所有模块共用这个连接
     */
    m_client = new QModbusTcpClient(this);

    /*
     * 设置 PLC 网络参数
     */
    m_client->setConnectionParameter(
        QModbusDevice::NetworkAddressParameter,
        host);

    m_client->setConnectionParameter(
        QModbusDevice::NetworkPortParameter,
        port);

    /*
     * 发起 TCP 连接
     */
    m_client->connectDevice();

    /*
     * 启动轮询定时器
     *
     * 示例：200ms 轮询一次
     */
    m_pollTimer.start(200);
}

void ModbusMasterModel::addModule(
    ModbusModuleBase* module)
{
    /*
     * 将模块加入轮询列表
     *
     * 同时把 Master 指针注入给模块，
     * 使模块具备写寄存器的能力
     */
    module->setMaster(this);
    m_modules << module;
}

void ModbusMasterModel::poll()
{
    /*
     * 核心轮询逻辑
     *
     * 每一轮：
     * - 顺序遍历所有模块
     * - 向 PLC 发送读取请求
     *
     * 注意：
     * - 这里不关心模块内容
     * - 只负责调度
     */
    for (auto* module : m_modules) {

        // 由模块提供“我要读哪一段寄存器”
        QModbusDataUnit unit = module->buildReadUnit();

        /*
         * 发送 Modbus 读请求
         *
         * unitId 在你的系统中是固定的
         * （同一个 PLC）
         */
        if (auto* reply =
            m_client->sendReadRequest(unit, /*unitId=*/1)) {

            /*
             * 异步处理读完成
             */
            connect(reply,
                    &QModbusReply::finished,
                    this,
                    [=]() {
                        /*
                         * 将 Modbus 响应交给模块解析
                         */
                        module->handleReadResult(
                            reply->result());

                        /*
                         * 释放 reply 对象
                         */
                        reply->deleteLater();
                    });
        }
    }
}

void ModbusMasterModel::writeFloat(
    int unitId,
    int reg,
    double value)
{
    /*
     * 将 double 转换为 float
     * 再拆成两个 16 位寄存器
     */
    float f = static_cast<float>(value);
    quint32 raw;
    std::memcpy(&raw, &f, sizeof(float));

    QModbusDataUnit unit(
        QModbusDataUnit::HoldingRegisters,
        reg - 1,   // QtModbus 使用 0-based 地址
        2);

    unit.setValue(0, raw >> 16);
    unit.setValue(1, raw & 0xFFFF);

    /*
     * 发送写请求
     */
    m_client->sendWriteRequest(unit, unitId);
}

void ModbusMasterModel::writeBit(
    int unitId,
    int reg,
    int bit,
    bool on)
{
    /*
     * 写 bit 的正确方式：
     * - 读取当前 word
     * - 修改目标 bit
     * - 写回整个 word
     */
    QModbusDataUnit unit(
        QModbusDataUnit::HoldingRegisters,
        reg - 1,
        1);

    quint16 value = unit.value(0);

    if (on)
        value |= (1 << bit);
    else
        value &= ~(1 << bit);

    unit.setValue(0, value);

    m_client->sendWriteRequest(unit, unitId);
}

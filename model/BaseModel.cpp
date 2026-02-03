#include "BaseModel.h"
#include <QThread>

BaseModel::BaseModel(QObject* parent, bool useSeparateThread)
    : ActorObserve(parent)
{
    if (useSeparateThread) {
        // 创建并启动单独的线程
        m_thread = new QThread(this);
        // 将当前对象移到新线程
        moveToThread(m_thread);
        // 启动线程
        m_thread->start();
    }
}

BaseModel::~BaseModel()
{
    if (m_thread && m_thread->isRunning()) {
        // 请求线程退出
        m_thread->quit();
        // 等待线程结束
        m_thread->wait();
        // 线程会被父对象（this）销毁，所以不需要手动delete
    }
}

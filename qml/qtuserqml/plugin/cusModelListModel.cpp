#include <QQmlApplicationEngine>
#include <QQmlContext>
#include<QString>
#include <QVariantMap>
#include <QDebug>
#include "cusModelListModel.h"

#pragma execution_character_set("utf-8")
namespace qtuser_qml
{
CusModelListModel::CusModelListModel(QObject* parent)
    :QAbstractListModel(parent)
{

}

CusModelListModel::CusModelListModel(const CusModelListModel& model)
{

}

void CusModelListModel::addItem(QObject *item)
{
    m_Items.push_back(item);
    FileInfo* fi = new FileInfo(item);
}

void CusModelListModel::delItem(QObject *item)
{

}

QObject* CusModelListModel::getItem(int row)
{
    return new QObject();
}

QModelIndex CusModelListModel::getRowIndex(QObject *obj)
{
    int row = 0;
    return createIndex(row, 0);
}

int CusModelListModel::itemCount()
{
    return m_Items.count();
}

int CusModelListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_FileInfoList.count();
}

QVariant CusModelListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_FileInfoList.count())
        return QVariant();

    const FileInfo &fInfo = m_FileInfoList[index.row()];
    switch (role)
    {
    case File_Name:
        return fInfo.fileName();
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> CusModelListModel::roleNames() const
{
    QHash<int, QByteArray> hash;
    hash[File_Name] = "fileName";

    return hash;
}

bool CusModelListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QAbstractListModel::setData(index, value, role);
}

void CusModelListModel::addModelData(const FileInfo &info)
{
    m_FileInfoList.push_back(info);
}

QString FileInfo::fileName() const
{
    return m_FileName;
}

void FileInfo::setFileName(const QString &fileName)
{
    if(fileName == m_FileName)
        return;
    m_FileName = fileName;
    emit fileNameChanged();
}

FileInfo::FileInfo(QObject* parent, QObject* item)
    :QObject(parent)
{

}

FileInfo::FileInfo(const FileInfo &fileInfo)
{

}

FileInfo::~FileInfo()
{

}
}

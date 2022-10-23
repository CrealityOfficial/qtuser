#include <QQmlApplicationEngine>
#include <QQmlContext>
#include<QString>
#include <QVariantMap>
#include <QDebug>
//#include "external/data/modeln.h"
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

void CusModelListModel::deleteItem(int index)
{

}

void CusModelListModel::deleteModel(QList<int> indexs)
{
    foreach (auto index, indexs)
    {
        removeRow(index);
    }
    emit rowChanged(m_Items.count());
}

void CusModelListModel::addItem(QObject *item)
{
    m_Items.push_back(item);
    FileInfo* fi = new FileInfo();
    fi->setFileName(item->objectName());
    fi->setIsChosed(false);
    fi->setModelVisible(true);

    addModelData(fi);
    emit rowChanged(m_Items.count());
}

bool CusModelListModel::delItem(QObject *item)
{
    foreach (auto data, m_Items) {
        if(data->objectName() == item->objectName())
        {
            m_Items.removeOne(data);
            delModel(item->objectName());
            return true;
        }
    }
    emit rowChanged(m_Items.count());
    return false;
}

QObject* CusModelListModel::getItem(int row)
{
    return m_Items.at(row);
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

void CusModelListModel::slotModelSelectionChanged()
{
    //getKernel()->modelSelector()->selectionms();
}

void CusModelListModel::choseChanged(QList<int> chosedItems)
{
    //全部重置false
    foreach (auto item, m_FileInfoList) {
        item->setIsChosed(false);
    }

    //选中项置为true
    foreach (auto itemIndex, chosedItems) {
        m_FileInfoList.at(itemIndex)->setIsChosed(true);
        //        refreshItem(itemIndex, 0);
    }
}

void CusModelListModel::visibleChanged(int index, bool visible)
{
    m_FileInfoList.at(index)->setModelVisible(visible);
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

    FileInfo* fInfo = m_FileInfoList[index.row()];
    switch (role)
    {
    case File_Name:
        return fInfo->fileName();
        break;
    case File_Visible:
        return fInfo->visible();
        break;
    case File_Checked:
        return fInfo->IsChosed();
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> CusModelListModel::roleNames() const
{
    QHash<int, QByteArray> hash;
    hash[File_Name] = "fileName";
    hash[File_Visible] = "fileVisible";
    hash[File_Checked] = "fileChecked";

    return hash;
}

bool CusModelListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QAbstractListModel::setData(index, value, role);
}

void CusModelListModel::addModelData(FileInfo* info)
{
    beginInsertRows(QModelIndex(), m_FileInfoList.count(), m_FileInfoList.count());
    m_FileInfoList.push_back(info);
    endInsertRows();
}

bool CusModelListModel::delModel(QString modelName)
{
    int index = 0;
    for(auto data : m_FileInfoList)
    {
        if(data->fileName() == modelName)
        {
            removeRow(index);
            return true;
        }
        ++index;
    }
    return false;
}

void CusModelListModel::removeRow(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_FileInfoList.removeAt(index);
    m_Items.removeAt(index);
    endRemoveRows();
}

QString FileInfo::fileName() const
{
    std::string str = m_FileName.toStdString();
    return m_FileName;
}

void FileInfo::setFileName(const QString &fileName)
{
    if(fileName == m_FileName)
        return;
    m_FileName = fileName;
    emit fileNameChanged();
}

bool FileInfo::visible() const
{
    return m_Visible;
}

void FileInfo::setModelVisible(bool visible)
{
    if (m_Visible == visible)
        return;
    m_Visible = visible;
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

bool FileInfo::IsChosed() const
{
    return m_IsChosed;
}

void FileInfo::setIsChosed(bool newIsChosed)
{
    if (m_IsChosed == newIsChosed)
        return;
    m_IsChosed = newIsChosed;
    emit IsChosedChanged();
}

}

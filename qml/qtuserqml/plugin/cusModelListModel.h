#ifndef CUSWIFILISTMODEL_H
#define CUSWIFILISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "qtuserqml/qtuserqmlexport.h"

namespace qtuser_qml
{
class FileInfo: public QObject
{
    Q_OBJECT
public:

    QString fileName() const;
    void setFileName(const QString& fileName);

    FileInfo(QObject* parent = nullptr, QObject* item = nullptr);
    FileInfo(const FileInfo& fileInfo);
    ~FileInfo();

signals:
    void fileNameChanged();

private:
    QString m_FileName;
};

class QTUSER_QML_API CusModelListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FileInfoRoles {
        File_Name  = Qt::UserRole + 1,
        File_Size
    };

    CusModelListModel(QObject* parent = nullptr);
    CusModelListModel(const CusModelListModel& model);
    //CusModelListModel();
    void addItem(QObject* item);
    void delItem(QObject* item);
    QObject* getItem(int row);
    QModelIndex getRowIndex(QObject* obj);
    int itemCount();
    void addModelData(const FileInfo& info);         //新增表数据
    void refreshItem(int row, int column); //刷新单项数据
public slots:
signals:

protected:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    QList<QObject*> m_Items;
    QList<FileInfo> m_FileInfoList;
};
}
#endif // CUSWIFILISTMODEL_H

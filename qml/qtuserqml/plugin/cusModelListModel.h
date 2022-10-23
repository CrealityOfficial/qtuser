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
    Q_PROPERTY(bool IsChosed READ IsChosed WRITE setIsChosed NOTIFY IsChosedChanged)
public:

    QString fileName() const;
    void setFileName(const QString& fileName);

    bool visible() const;
    void setModelVisible(bool visible);

    FileInfo(QObject* parent = nullptr, QObject* item = nullptr);
    FileInfo(const FileInfo& fileInfo);
    ~FileInfo();

    bool IsChosed() const;
    void setIsChosed(bool newIsChosed);

signals:
    void fileNameChanged();

    void IsChosedChanged();

private:
    QString m_FileName;
    bool m_Visible;
    bool m_IsChosed;
};

class QTUSER_QML_API CusModelListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FileInfoRoles {
        File_Name  = Qt::UserRole + 1,
        File_Visible,
        File_Checked,
        File_Size
    };

    CusModelListModel(QObject* parent = nullptr);
    CusModelListModel(const CusModelListModel& model);

    Q_INVOKABLE void deleteItem(int index);
    Q_INVOKABLE void deleteModel(QList<int> indexs);
    Q_INVOKABLE void choseChanged(QList<int> chosedItems);
    Q_INVOKABLE void visibleChanged(int index, bool visible);

    void addItem(QObject* item);
    bool delItem(QObject* item);
    QObject* getItem(int row);
    QModelIndex getRowIndex(QObject* obj);
    int itemCount();
public slots:
    void slotModelSelectionChanged();
signals:
    void rowChanged(int row);
protected:
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    void refreshItem(int row, int column); //刷新单项数据
    void addModelData(FileInfo* info);         //新增表数据
    bool delModel(QString modelName);
    void removeRow(int index);
    QList<QObject*> m_Items;
    QList<FileInfo*> m_FileInfoList;
};
}
#endif // CUSWIFILISTMODEL_H

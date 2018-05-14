#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include <QList>

class Role: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int roleID READ roleID)
    Q_PROPERTY(QString role READ role)

public:
    Role(QObject *parent = nullptr);

    int roleID() const;
    QString role() const;

    QList<QString> getRolePermissions() const;

    Q_INVOKABLE bool hasPermission(const QString permStr) const;
    Q_INVOKABLE bool addUserRole(const QString username, const QString roleName);

    Q_INVOKABLE void initGuestRole();

signals:
    void roleIdSet(const int roleID);
    void roleSet(const QString role);
    void rolePermissionsSet();
    void userRoleAdded(const int uid, const int rid);

public slots:
    void setRoleIdFromUsername(const QString username);
    void setRoleFromRoleId(const int roleID);
    void setRolePermissions(const int roleID);

private:
    QList<QString> m_permissions;
    int m_roleID;
    QString m_role;
};

#endif // ROLE_H

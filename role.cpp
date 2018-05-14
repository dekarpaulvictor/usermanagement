#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "role.h"

Role::Role(QObject *parent)
    : QObject(parent)
{
}

QList<QString> Role::getRolePermissions() const
{
    return m_permissions;
}

int Role::roleID() const
{
    return m_roleID;
}

QString Role::role() const
{
    return m_role;
}

void Role::setRoleIdFromUsername(const QString username)
{
    // Get roleID from database given a username.

    qDebug() << "username from rbacusermanager object: " << username;

    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare(QLatin1String("SELECT DISTINCT role_id "
                            "FROM user_roles ur JOIN user_credentials uc "
                            "ON uc.id = ur.user_id "
                            "WHERE uc.username = :username"));
    q.bindValue(":username", username);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect())
        {
            while (q.next()) {
                m_roleID = q.value(0).toInt();
                qDebug() << "Role Id from setRoleIdFromUsername function: " << m_roleID;
            }
            emit roleIdSet(m_roleID);
            setRoleFromRoleId(m_roleID);
            setRolePermissions(m_roleID);
        }
        else
        {
            qDebug() << "There was an error executing query. Unable to set roleID from given username.";
        }
    }
    else
    {
        qDebug() << q.lastError();
    }
}

void Role::setRoleFromRoleId(const int roleID)
{
    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare(QLatin1String("SELECT role FROM roles "
                            "WHERE id = :roleID"));
    q.bindValue(":roleID", roleID);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect())
        {
            while (q.next()) {
                m_role = q.value(0).toString();
                qDebug() << "Role from setRoleFromRoleId function: " << m_role;
            }
            emit roleSet(m_role);
        }
        else
        {
            qDebug() << "There was an error executing query. Unable to set role from roleID.";
        }
    }
    else
    {
        qDebug() << q.lastError();
    }
}

void Role::setRolePermissions(const int roleID)
{
    // Fetch and store role permissions from database using set roleID
    QSqlQuery q;
    q.setForwardOnly(true);
    q.prepare(QLatin1String("SELECT permission FROM permissions "
                            "WHERE id IN (SELECT DISTINCT permission_id FROM role_permissions "
                            "WHERE role_id = :roleID)"));
    q.bindValue(":roleID", roleID);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect())
        {
            while (q.next()) {
                m_permissions.append(q.value(0).toString());
            }
            emit rolePermissionsSet();
        }
        else
        {
            qDebug() << "There was an error executing query. Unable to populate permissions container.";
        }
    }
    else
    {
        qDebug() << q.lastError();
    }
}

bool Role::hasPermission(const QString permStr) const
{
    return m_permissions.contains(permStr);
}

bool Role::addUserRole(const QString username, const QString roleName)
{
    int uID = -1;
    int rID = -1;

    // Query user_id using given username
    QSqlQuery q;

    q.prepare(QLatin1String("SELECT id FROM users WHERE id IN ( "
                            "SELECT DISTINCT id FROM user_credentials WHERE username = :username)"));
    q.bindValue(":username", username);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect())
        {
            while (q.next()) {
                uID = q.value(0).toInt();
                qDebug() << "UID" << uID;
            }
        }
        else
        {
            qDebug() << "There was an error executing query. ";
            qDebug() << "Unable to fetch user_id using given username:" << username;
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    q.finish();

    // Query role_id using given role name
    q.prepare(QLatin1String("SELECT id FROM roles "
                            "WHERE role = :rolename"));
    q.bindValue(":rolename", roleName);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect())
        {
            while (q.next()) {
                rID = q.value(0).toInt();
                qDebug() << "RID" << rID;
            }
        }
        else
        {
            qDebug() << "There was an error executing query. Unable to fetch role_id using given role name:" << roleName;
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    q.finish();

    // Insert user_id, role_id pair to user_roles table, if both > 0
    if (uID > 0 && rID > 0)
    {
        q.prepare(QLatin1String("INSERT INTO user_roles (user_id, role_id) "
                                "VALUES (:user_id, :role_id)"));
        q.bindValue(":user_id", uID);
        q.bindValue(":role_id", rID);
        if (q.exec())
        {
            qDebug() << "user/role mapping created successfully.";
            emit userRoleAdded(uID, rID);
            return true;
        }
        else
        {
            qDebug() << q.lastError();
            return false;
        }
    }
    else
    {
        qDebug() << "Cannot create user/role mapping. Unknown error.";
        return false;
    }

    return false;
}

void Role::initGuestRole()
{
    const QString guestRole = "guest";

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT id FROM roles "
                            "WHERE role = :rolename"));
    q.bindValue(":rolename", guestRole);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect())
        {
            while (q.next()) {
                m_roleID = q.value(0).toInt();
                emit roleIdSet(m_roleID);
                setRoleFromRoleId(m_roleID);
                setRolePermissions(m_roleID);
            }
        }
        else
        {
            qDebug() << "There was an error executing query. Unable to fetch role_id.";
        }
    }
    else
    {
        qDebug() << q.lastError();
    }
}

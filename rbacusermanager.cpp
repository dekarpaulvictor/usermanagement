#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "rbacusermanager.h"

RBACUserManager::RBACUserManager(User *parent)
    : User(parent)
{
    m_role = new Role(this);
    m_session = new Session(this);

    connect(this, &RBACUserManager::authenticated, m_role, [=]() {
        m_role->setRoleIdFromUsername(this->username());
    });

    connect(m_role, &Role::rolePermissionsSet, this, [=]() {
        m_session->setUsername(this->username());
        m_session->setRole(m_role->role());
        m_session->start();
    });

//    connect(this, &RBACUserManager::authenticated, m_role, &Role::setRoleIdFromUsername);
//    connect(m_role, &Role::rolePermissionsSet, m_session, &Session::start);
}

Role *RBACUserManager::role() const
{
    return m_role;
}

void RBACUserManager::setRole(Role *role)
{
    if (role != m_role)
    {
        m_role = role;
    }
}

Session *RBACUserManager::session() const
{
    return m_session;
}

void RBACUserManager::setSession(Session *session)
{
    if (session != m_session)
    {
        m_session = session;
    }
}

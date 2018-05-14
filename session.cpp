#include <QDebug>

#include "session.h"

Session::Session(QObject *parent)
    : QObject(parent), m_username(QString()), m_role(QString()), m_loggedIn(false)
{

}

QString Session::username() const
{
    return m_username;
}

void Session::setUsername(const QString username)
{
    if (m_username != username)
    {
        m_username = username;
        emit usernameChanged(username);
    }
}

bool Session::loggedIn() const
{
    return m_loggedIn;
}

void Session::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;
}

bool Session::switchedRoles() const
{
    return m_switchedRoles;
}

void Session::setSwitchedRoles(bool switchedRoles)
{
    m_switchedRoles = switchedRoles;
}

QString Session::selectedRole() const
{
    return m_selectedRole;
}

void Session::setSelectedRole(const QString &selectedRole)
{
    m_selectedRole = selectedRole;
}

int Session::selectedRoleID() const
{
    return m_selectedRoleID;
}

void Session::setSelectedRoleID(int selectedRoleID)
{
    m_selectedRoleID = selectedRoleID;
}

QString Session::role() const
{
    return m_role;
}

void Session::setRole(const QString role)
{
    if (m_role != role)
    {
        m_role = role;
        emit roleChanged(role);
    }
}

// Session starter
void Session::start()
{

    m_loggedIn = true;
    qDebug() << "Session started. Logged in: " << loggedIn();
}

void Session::stop()
{
    m_loggedIn = false;
    qDebug() << "Session stopped. Logged in: " << loggedIn();
}

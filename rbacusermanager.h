#ifndef RBACUSERMANAGER_H
#define RBACUSERMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>

#include "user.h"
#include "role.h"
#include "session.h"

class RBACUserManager : public User
{
    Q_OBJECT
    Q_PROPERTY(Role *role READ role WRITE setRole)
    Q_PROPERTY(Session *session READ session WRITE setSession)

public:
    explicit RBACUserManager(User *parent = nullptr);

    Role *role() const;
    void setRole(Role *role);

    Session *session() const;
    void setSession(Session *session);

private:
    Role *m_role;
    Session *m_session;
};

#endif // RBACUSERMANAGER_H

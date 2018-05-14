#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class Session : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(bool loggedIn READ loggedIn WRITE setLoggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(bool switchedRoles READ switchedRoles WRITE setSwitchedRoles NOTIFY switchedRolesChanged)
    Q_PROPERTY(QString selectedRole READ selectedRole WRITE setSelectedRole)
    Q_PROPERTY(int selectedRoleID READ selectedRoleID WRITE setSelectedRoleID)

public:
    explicit Session(QObject *parent = nullptr);

    QString username() const;
    void setUsername(const QString username);
    QString role() const;
    void setRole(const QString role);

    bool loggedIn() const;
    void setLoggedIn(bool loggedIn);

    bool switchedRoles() const;
    void setSwitchedRoles(bool switchedRoles);

    QString selectedRole() const;
    void setSelectedRole(const QString &selectedRole);

    int selectedRoleID() const;
    void setSelectedRoleID(int selectedRoleID);

signals:
    void usernameChanged(const QString username);
    void roleChanged(const QString role);
    void loggedInChanged();
    void sessionStarted();
    void sessionStopped();
    void switchedRolesChanged();

public slots:
    void start();
    void stop();

private:
    QString m_username;
    QString m_role;

    bool m_loggedIn;

    // For keeping track of adopted roles mid-session
    bool m_switchedRoles;
    QString m_selectedRole;
    int m_selectedRoleID;
};

#endif // SESSION_H

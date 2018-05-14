#ifndef USER_H
#define USER_H

#include <QObject>
#include <sodium.h>

// External functions from the libsodium library for use in generating and verifying password
// hashes
extern "C" {
    int sodium_init();
    int crypto_pwhash_str(char out[], const char * const passwd, unsigned long long passwdlen, unsigned long long opslimit, size_t memlimit);
    int crypto_pwhash_str_verify(const char str[], const char * const passwd, unsigned long long passwdlen);
}

class User : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString gender READ gender WRITE setGender NOTIFY genderChanged)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString passStr READ passStr WRITE setPassStr NOTIFY passStrChanged)
    Q_PROPERTY(QString regDate READ regDate)
    Q_PROPERTY(QString deregDate READ deregDate)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)

public:
    explicit User(QObject *parent = nullptr);
    QString name() const;
    void setName(const QString &n);
    QString gender() const;
    void setGender(const QString &g);
    QString phone() const;
    void setPhone(const QString &p);
    QString email() const;
    void setEmail(const QString &e);
    QString username() const;
    void setUsername(const QString &u);
    QString passStr() const;
    void setPassStr(const QString &pass);
    QString regDate() const;
    QString deregDate() const;
    QString status() const;
    void setStatus(const QString &s);

    Q_INVOKABLE bool userExists(const QString &username);
    Q_INVOKABLE bool addUser();
    Q_INVOKABLE bool updateUser();
    Q_INVOKABLE bool getUser(const QString &username);
    Q_INVOKABLE bool deleteUser(const QString &username);
    Q_INVOKABLE bool resetPassword(const QString &username, const QString &pass);

signals:
    void nameChanged(const QString &n);
    void genderChanged(const QString &g);
    void phoneChanged(const QString &p);
    void emailChanged(const QString &e);
    void usernameChanged(const QString &u);
    void passStrChanged(const QString &pass);
    void statusChanged(const QString &s);

    void authenticated(const QString username);

public slots:
    bool authenticate(const QString &username, const QString &pass);

private:
    int m_userId;
    QString m_name, m_gender, m_phone, m_email, m_username, m_passStr;
    QString m_regDate, m_deregDate, m_status;

    QByteArray genHash(const QString &pass);
    bool verifyHash(const QByteArray &hash, const QString &pass);
};

#endif // USER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QLatin1String>
#include <QDebug>

#include "user.h"

User::User(QObject *parent) : QObject(parent)
{
    // Initialize libsodium
    if (sodium_init() == -1)
    {
        qDebug() << "Could not initiliaze libsodium. Exiting.";
    }
}

// Getters

QString User::name() const
{
    return m_name;
}

QString User::gender() const
{
    return m_gender;
}

QString User::phone() const
{
    return m_phone;
}

QString User::email() const
{
    return m_email;
}

QString User::username() const
{
    return m_username;
}

QString User::passStr() const
{
    return m_passStr;
}

QString User::regDate() const
{
    return m_regDate;
}

QString User::deregDate() const
{
    return m_deregDate;
}

QString User::status() const
{
    return m_status;
}

// Setters

void User::setName(const QString &n)
{
    if (n != m_name)
    {
        m_name = n;
        emit nameChanged(n);
    }
}

void User::setGender(const QString &g)
{
    if (g != m_gender)
    {
        m_gender = g;
        emit genderChanged(g);
    }
}

void User::setPhone(const QString &p)
{
    if (p != m_phone)
    {
        m_phone = p;
        emit phoneChanged(p);
    }
}

void User::setEmail(const QString &e)
{
    if (e != m_email)
    {
        m_email = e;
        emit emailChanged(e);
    }
}

void User::setUsername(const QString &u)
{
    if (u != m_username)
    {
        m_username = u;
        emit usernameChanged(u);
    }
}

void User::setPassStr(const QString &pass)
{
    if (pass != m_passStr)
    {
        m_passStr = pass;
        emit passStrChanged(pass);
    }
}

void User::setStatus(const QString &s)
{
    if (s != m_status)
    {
        m_status = s;
        emit statusChanged(s);
    }
}

// private functions

QByteArray User::genHash(const QString &pass)
{
    QByteArray p = pass.toLocal8Bit();

    char hashedP[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(hashedP, p, p.size(), crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
    {
        qDebug() << "Could not generate hash. Out of memory error!";
    }
    QByteArray hp(hashedP);
    return hp;
}

bool User::verifyHash(const QByteArray &hash, const QString &pass)
{
    QByteArray p = pass.toLocal8Bit();
    if (crypto_pwhash_str_verify(hash, p, p.size()) == 0)
        return true;

    return false;

}

bool User::userExists(const QString &username)
{
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT username FROM user_credentials "
                            "WHERE username = :username"));
    q.bindValue(":username", username);
    if (q.exec())
    {
        if (q.isActive() && q.isSelect()) {
            q.next();
            if (q.isValid())
            {
                if (q.value(0).toString() == username)
                {

                    qDebug() << "User:" << username << "exists.";
                    return true;
                }
                else {
                    qDebug() << "User:" << username << ". No such user.";
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        qDebug() << "Error:" << q.lastError();
        return false;
    }

}

bool User::addUser() // a.k.a registerUser()
{
    QString hash = QString(genHash(m_passStr));

    QSqlDatabase::database().transaction();

    QSqlQuery firstQ;
    firstQ.prepare(QLatin1String("INSERT INTO users (name, gender, phone, email, "
                                 "registered_date, status) VALUES (:name, :gender, "
                                 ":phone, :email, :registered_date, :status)"));
    firstQ.bindValue(":name", m_name);
    firstQ.bindValue(":gender", m_gender);
    firstQ.bindValue(":phone", m_phone);
    firstQ.bindValue(":email", m_email);
    firstQ.bindValue(":registered_date", "now()");
    firstQ.bindValue(":status", m_status);

    if (firstQ.exec())
    {
        qDebug() << "First add query successful.";
    }
    else
    {
        qDebug() << "First add query failed. " << firstQ.lastError();
    };

    QSqlQuery secondQ;
    secondQ.prepare(QLatin1String("INSERT INTO user_credentials (username, password_hash) "
                    "VALUES (:username, :password_hash)"));
    secondQ.bindValue(":username", m_username);
    secondQ.bindValue(":password_hash", hash);

    if (secondQ.exec())
    {
        qDebug() << "Second add query successful.";
    }
    else
    {
        qDebug() << "Second add query failed. " << secondQ.lastError();
    }

    firstQ.finish();
    secondQ.finish();

    if (!QSqlDatabase::database().commit())
    {
        qDebug() << QSqlDatabase::database().lastError();
        return false;
    }

    return true;

}

bool User::updateUser()
{
    QString hash = QString(genHash(m_passStr));

    QSqlDatabase::database().transaction();

    QSqlQuery firstQ;
    firstQ.prepare(QLatin1String("UPDATE users "
                                 "SET phone = :phone, email = :email "
                                 "WHERE id = :userId"));
    firstQ.bindValue(":phone", m_phone);
    firstQ.bindValue(":email", m_email);
    firstQ.bindValue(":userId", m_userId);

    if (firstQ.exec())
    {
        qDebug() << "First update query successful.";
    }
    else
    {
        qDebug() << "First update query failed. " << firstQ.lastError();
    }

    QSqlQuery secondQ;
    secondQ.prepare(QLatin1String("UPDATE user_credentials "
                                  "SET username = :username, password_hash = :password_hash "
                                  "WHERE id = :userId"));
    secondQ.bindValue(":username", m_username);
    secondQ.bindValue(":password_hash", hash);
    secondQ.bindValue(":userId", m_userId);

    if (secondQ.exec())
    {
        qDebug() << "Second update query successful.";
    }
    else
    {
        qDebug() << "Second update query failed. " << secondQ.lastError();
    }

    firstQ.finish();
    secondQ.finish();

    if (!QSqlDatabase::database().commit())
    {
        qDebug() << QSqlDatabase::database().lastError();
        return false;
    }

    return true;
}

bool User::getUser(const QString &username)
{
    QSqlDatabase::database().transaction();

    QSqlQuery firstQ;
    firstQ.prepare(QLatin1String("SELECT id, username FROM user_credentials "
                                 "WHERE username = :username"));
    firstQ.bindValue(":username", username);

    if (firstQ.exec())
    {
        while (firstQ.next())
        {
            m_userId = firstQ.value(0).toUInt();
            m_username = firstQ.value(1).toString();
        }
        qDebug() << "First get query successful.";
    }
    else
    {
        qDebug() << "Could not execute first get query. Error: " << firstQ.lastError();
    }

    QSqlQuery secondQ;
    secondQ.prepare(QLatin1String("SELECT name, gender, phone, email, registered_date, "
                                 "deregistered_date, status "
                                 "FROM users "
                                 "WHERE id = :userId"));
    secondQ.bindValue(":userId", m_userId);

    if (secondQ.exec())
    {

        while (secondQ.next())
        {   m_name = secondQ.value(0).toString();
            m_gender = secondQ.value(1).toString();
            m_phone = secondQ.value(2).toString();
            m_email = secondQ.value(3).toString();
            m_regDate = secondQ.value(4).toString();
            m_deregDate = secondQ.value(5).toString();
            m_status = secondQ.value(6).toString();
        }
        qDebug() << "Second get query successful.";

    }
    else
    {
        qDebug() << "Could not execute second get query. Error: " << secondQ.lastError();
    }

    firstQ.finish();
    secondQ.finish();

    if (!QSqlDatabase::database().commit())
    {
        qDebug() << QSqlDatabase::database().lastError();
        return false;
    }

    return true;
}

bool User::deleteUser(const QString &username)
{
    QSqlQuery q;
    q.prepare(QLatin1String("DELETE FROM users WHERE id IN "
                            "(SELECT DISTINCT(id) FROM user_credentials "
                            "WHERE username = :username)"));
    q.bindValue(":username", username);
    if (q.exec())
    {
        qDebug() << "User:" << username << "deleted successfully";
        return true;
    }
    else
    {
        qDebug() << "Could not delete user:" << username << "Error:" << q.lastError();
        return false;
    }
}

bool User::authenticate(const QString &username, const QString &pass)
{
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT password_hash FROM user_credentials "
                            "WHERE username = :username"));
    q.bindValue(":username", username);
    if (q.exec())
    {
        QString hash;
        while (q.next()) {
            hash = q.value(0).toString();
        }
        if (verifyHash(hash.toLocal8Bit(), pass))
        {
            if (getUser(username))
            {
                emit authenticated(username);
                return true;
            }
            else
            {
                qDebug() << "User authentication succeeded, but user data could not be loaded.";
                return false;
            }
        }
        else
        {
            qDebug() << "User authentication failed. Wrong credentials given.";
            return false;
        }

    }
    else
    {
        qDebug() << q.lastError();
        return false;
    }
}

bool User::resetPassword(const QString &username, const QString &pass)
{
    QString hash = QString(genHash(pass));

    QSqlQuery q;
    q.prepare(QLatin1String("UPDATE user_credentials "
                            "SET password_hash = :password_hash "
                            "WHERE username = :username"));
    q.bindValue(":password_hash", hash);
    q.bindValue(":username", username);

    if (q.exec())
    {
        qDebug() << "Password reset successful.";
        return true;
    }
    else
    {
        qDebug() << "Password reset failed. Error: " << q.lastError();
        return false;
    }
}

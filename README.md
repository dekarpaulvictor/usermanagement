# RBAC-enabled User Management System

This application can serve as a starting point for a Role Based Access Control app using Qt/C++ and QML. It has classes that can be used, directly or inherited, to register users, reset passwords, update user data, assign roles, delete user accounts, manage permissions and implement a rudimentary session management system.

## Requirements

- Qt >= 5.7
- Qt Quick Controls 2
- [libsodium](https://download.libsodium.org/doc/) - A modern, easy-to-use software library for encryption, decryption, signatures, password hashing and more. Check out [this](https://download.libsodium.org/doc/installation/) part of the documentation to learn how to build it for your particular system.

## Installation

I use the 64-bit version of Ubuntu 16.04 as my main development box. The instructions here could work for a different linux distro or operating system with a few modifications.

1. Clone this repository anywhere in your home folder, for example:

```bash
cd ~/projects/qt
git clone https://github.com/dekarpaulvictor/usermanagement.git
```

2. Open the usermanagement.pro project file in Qt Creator
3. Build and run

## Help and Improvement

If you have any suggestions on how this app can be improved please let me know, or better yet, you can create a branch and contribute to the development. I would greatly appreciate help especially with documenting the code.


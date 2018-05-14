/*
    Sample app that can be used as a starting point for implementing a Role-based
    Access Control System.
    
    Author: Victor Paul
    Comments: You can comment out various sections of the following code to test
    different features of the program.
*/


import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ModioBurn.Tools 1.0

ApplicationWindow {
    width: 720
    height: 640
    visible: true
    title: qsTr("Generic Tests")

    UserManager {
        id: userManager
    }


    ColumnLayout {
        anchors.fill: parent

        TextField {
            id: nameInput
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Name"
        }

        ComboBox {
            id: genderSelect
            anchors.top: nameInput.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["Male", "Female", "Other"]
        }

        TextField {
            id: phoneInput
            anchors.top: genderSelect.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Phone"
        }

        TextField {
            id: emailInput
            anchors.top: phoneInput.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "E-mail[optional]"
        }

        TextField {
            id: usernameInput
            anchors.top: emailInput.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Username"
            onEditingFinished: function() {
                if (userManager.userExists(usernameInput.text))
                {
                    console.log("Username taken. Choose another username before submitting this data.");
                }
            }
        }

        TextField {
            id: passInput
            anchors.top: usernameInput.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        TextField {
            id: confirmPassInput
            anchors.top: passInput.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Confirm password"
            echoMode: TextInput.Password
        }

        ComboBox {
            id: roleSelect
            anchors.top: confirmPassInput.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["Super-administrator", "Administrator", "Manager", "Attendant", "Customer"]
        }

        CheckBox {
            id: statusSelect
            anchors.top: roleSelect.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Activate"
        }

        Button {
            id: addUserButton
            anchors.top: statusSelect.bottom
            anchors.topMargin: 40
            text: "add user"
            onClicked: function() {
                userManager.name = nameInput.text.toLowerCase();
                userManager.gender = genderSelect.currentText.toLowerCase();
                userManager.phone = phoneInput.text;
                userManager.email = emailInput.text.toLowerCase();
                userManager.username = usernameInput.text.toLowerCase();
                userManager.passStr = passInput.text;
                if (statusSelect.checked)
                    userManager.status = "active";
                else
                    userManager.status = "inactive";

                if(!userManager.userExists(usernameInput.text.toLowerCase()))
                {
                    if (userManager.addUser())
                    {
                        if (userManager.role.addUserRole(usernameInput.text.toLowerCase(), roleSelect.currentText.toLowerCase()))
                        {
                            console.log("User added successfully");
                        }
                        else
                        {
                            console.log("Failed to add user. Unknown reason. Try again.");
                        }
                    }
                    else
                    {
                        console.log("Failed to add user. Unknown reason. Try again.");
                    }
                }
                else
                {
                    console.log("Cannot add user. Username taken. Choose a different username.");
                }
            }
        }

    }



//    ColumnLayout {
//        anchors.fill: parent

//        Text {
//            id: name
//            anchors.top: parent.top
//            anchors.topMargin: 20
//            anchors.horizontalCenter: parent.horizontalCenter
//        }

//        TextField {
//            id: phoneInput
//            anchors.top: name.bottom
//            anchors.topMargin: 20
//            anchors.horizontalCenter: parent.horizontalCenter
//        }

//        TextField {
//            id: emailInput
//            anchors.top: phoneInput.bottom
//            anchors.topMargin: 20
//            anchors.horizontalCenter: parent.horizontalCenter
//        }

//        TextField {
//            id: passInput
//            anchors.top: emailInput.bottom
//            anchors.topMargin: 20
//            anchors.horizontalCenter: parent.horizontalCenter
//            placeholderText: "Password"
//            echoMode: TextInput.Password
//        }

//        TextField {
//            id: confirmPassInput
//            anchors.top: passInput.bottom
//            anchors.topMargin: 20
//            anchors.horizontalCenter: parent.horizontalCenter
//            placeholderText: "Confirm password"
//            echoMode: TextInput.Password
//        }

//        Button {
//            id: editUserButton
//            anchors.top: confirmPassInput.bottom
//            anchors.topMargin: 40
//            text: "update user"
//            onClicked: function() {
//                user.phone = phoneInput.text;
//                user.email = emailInput.text;
//                user.passStr = passInput.text;

//                if(!user.updateUser())
//                {
//                    console.log("Failed to update user");
//                }
//                else
//                {
//                    console.log("User edited successfully");
//                }
//            }
//        }

//        Component.onCompleted: function() {
//            if (user.userExists("vpdweb") && user.getUser("vpdweb"))
//            {
//                name.text = user.name;
//                phoneInput.placeholderText = user.phone;
//                emailInput.placeholderText = user.email;

//                console.log("user data gotten successfully")
//            }
//            else
//            {
//                console.log("Could not get user vpdweb");
//            }

//        }



//    }


//    TextField {

//        property bool exists: false

//        id: usernameInput
//        anchors.top: parent.top
//        anchors.topMargin: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        placeholderText: "Username"
//        onEditingFinished: function() {
//            if (!userManager.userExists(usernameInput.text))
//            {
//                exists = false;
//                console.log(usernameInput.text + " does not exist. You can't authenticate a non-existent user.");
//            }
//            else{
//                exists = true;
//            }
//        }
//    }

//    TextField {
//        id: passInput
//        anchors.top: usernameInput.bottom
//        anchors.topMargin: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        placeholderText: "Password"
//        echoMode: TextInput.Password
//    }

//    Button {
//        id: authenticateUserButton
//        anchors.top: passInput.bottom
//        anchors.topMargin: 40
//        text: "authenticate"
//        onClicked: function() {
//            if (userManager.authenticate(usernameInput.text.toLowerCase(), passInput.text))
//            {
//                console.log("user authentication successful. Printing out data...");
//                console.log("Role ID: " + userManager.role.roleID);
//                console.log("Role: " + userManager.role.role);
//                if (userManager.role.hasPermission("delete attendant"))
//                {
//                    console.log("Role can delete attendant!");
//                }
//                else
//                {
//                    console.log("Role can't delete attendant!");
//                }
//            }
//            else
//                console.log("user authentication failed.");
//        }
//        enabled: usernameInput.exists
//    }



//    TextField {

//        property bool exists: false

//        id: usernameInput
//        anchors.top: parent.top
//        anchors.topMargin: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        placeholderText: "Username"
//        onEditingFinished: function() {
//            if (!user.userExists(usernameInput.text))
//            {
//                exists = false;
//                console.log("You cannot reset a non-existent user's password.");
//            }
//            else{
//                exists = true;
//            }
//        }
//    }

//    TextField {
//        id: passInput
//        anchors.top: usernameInput.bottom
//        anchors.topMargin: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        placeholderText: "Password"
//        echoMode: TextInput.Password
//    }

//    Button {
//        id: addUserButton
//        anchors.top: passInput.bottom
//        anchors.topMargin: 40
//        text: "reset password"
//        onClicked: function() {
//            if (!user.resetPassword(usernameInput.text, passInput.text))
//                console.log("Password reset failed.");
//            else
//                console.log("Password reset successful.");
//        }
//        enabled: usernameInput.exists
//    }

//    ColumnLayout {
//        anchors.fill: parent
//        TextField {
//            property bool userExists: false

//            id: usernameInput
//            anchors.top: parent.top
//            anchors.topMargin: 20
//            anchors.horizontalCenter: parent.horizontalCenter
//            placeholderText: "Username"
//            onEditingFinished: function()
//            {
//                if (!user.userExists(usernameInput.text))
//                {
//                    userExists = false;
//                    console.log("User doesn't exist. You cannot delete a non-existent user")
//                }
//                else
//                {
//                    userExists = true;
//                }
//            }
//        }

//        Button {
//            id: addUserButton
//            anchors.top: usernameInput.bottom
//            anchors.topMargin: 40
//            text: "delete user"
//            enabled: usernameInput.userExists
//            onClicked: user.deleteUser(usernameInput.text)
//        }

//    }



}

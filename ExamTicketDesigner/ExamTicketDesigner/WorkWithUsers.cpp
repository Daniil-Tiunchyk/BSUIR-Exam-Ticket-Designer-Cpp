#include "WorkWithUsers.h"
#include "Protection.h"


// ������� ����������� ������ ������������ � ����������
void registerUser(User users[], int& usersCount) {
	const int maxUsers = 100;
	User newUser;
	string confirmPassword;
	bool validInput;
    cout << "����������� ������ ������������\n";

    // ��������� ����� ������������
    do {
        validInput = true;
        cout << "\n������� ��� ������������: ";
        cin >> newUser.username;

        for (int i = 0; i < usersCount; ++i) {
            if (users[i].username == newUser.username) {
                cout << "��� ������������ ��� ����������, ���������� ��� ���\n";
                validInput = false;
                break;
            }
        }
    } while (!validInput);

    // ��������� ������
    do {
        cout << "������� ������: ";
        newUser.password=inputPassword();
        cout << "\n����������� ������: ";
        confirmPassword = inputPassword();

        if (newUser.password != confirmPassword) {
            cout << "������ �� ���������, ���������� ��� ���\n";
        }
    } while (newUser.password != confirmPassword);

    newUser.role = 1;
    newUser.password = doHashNow(newUser.password);

    users[usersCount++] = newUser;

    saveUsers(users, usersCount);

    cout << "\n������������ ������� ���������������\n";
    Sleep(2000);
    system("cls");
}

// ������� ����������� ������������ � ����������
int loginUser(User users[], int usersCount, int& onlineUser) {
    string inputUsername, password;
    int loginAttempts = 3;
    while (loginAttempts > 0) {
        cout << "������� ��� ������������: ";
        cin >> inputUsername;
        cout << "������� ������: ";
        password = inputPassword();
        password = doHashNow(password);
        for (int i = 0; i < usersCount; ++i) {
            if (users[i].username == inputUsername && users[i].password == password) {
                cout << "\n�������� �����������\n\n";
                onlineUser = i;
                return users[i].role;
            }
        }

        cout << "\n�������� ��� ������������ ��� ������. �������� �������: " << --loginAttempts << endl;
    }
    cout << "����������� �� �������. ����������, ���������� �����\n";
    return 0;
}

// ������� ���������� ������������� � ����
void saveUsers(User users[], int usersCount) {
    ofstream usersFile(usersFilename);
    if (!usersFile) {
        cerr << "������ �������� ����� �������������\n";
        return;
    }

    for (int i = 0; i < usersCount; ++i) {
        usersFile << users[i].username << " " << users[i].password << " " << users[i].role << endl;
    }

    usersFile.close();
}

// ������� �������� ������������� �� �����
void loadUsers(User users[], int& usersCount) {
    ifstream usersFile(usersFilename);
    if (!usersFile) {
        cerr << "������ �������� ����� �������������. �������� ������ �����...\n";
        ofstream newUsersFile(usersFilename);
        newUsersFile.close();
        usersCount = 0;
        return;
    }

    usersCount = 0;
    while (usersFile >> users[usersCount].username >> users[usersCount].password >> users[usersCount].role) {
        ++usersCount;
    }

    usersFile.close();
}

// �������, ����������� �������� ��������������
void adminActions(User users[], int usersCount, int onlineUser) {
    loadUsers(users, usersCount);

    int choice;
    do {
        system("cls");
        cout << "\n\t\t����:\n\t1. ������ � ��������\n\t2. ���������� ��������������\n\t3. �����\n\t\t>>> ";
        choice=protection(1,3);
        switch (choice) {
        case 1:
            userActions(users, usersCount, onlineUser);
            break;
        case 2:
            manageUsers(users, usersCount, onlineUser);
            break;
        case 3:
            cout << "�����\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 3);
}

// ������� ��������� ����� ������������ � ������
void changeUserInfo(User users[], int& onlineUser, int usersCount) {
    string newUsername, newPassword, confirmPassword;
    bool validInput;

    // ��������� ������ ����� ������������
    do {
        validInput = true;
        cout << "\n������� ����� ��� ������������: ";
        cin >> newUsername;

        for (int i = 0; i < usersCount; ++i) {
            if (i != onlineUser && users[i].username == newUsername) {
                cout << "��� ������������ ��� ����������, ���������� ��� ���\n";
                validInput = false;
                break;
            }
        }
    } while (!validInput);

    // ��������� ������ ������
    do {
        cout << "������� ����� ������: ";
        newPassword = inputPassword();
        cout << "\n����������� ����� ������: ";
        confirmPassword = inputPassword();

        if (newPassword != confirmPassword) {
            cout << "������ �� ���������, ���������� ��� ���\n";
        }
    } while (newPassword != confirmPassword);

    // ���������� ���������
    users[onlineUser].username = newUsername;
    users[onlineUser].password = doHashNow(newPassword);
    saveUsers(users, usersCount);

    cout << "\n���� ���������� ������� ���������\n";
    Sleep(2000);
    system("cls");
}

// ������� ���������� �������������� (������ ��� ��������������)
void manageUsers(User users[], int usersCount, int onlineUser) {
    int choice, userId;
    string newUsername, newPassword;
    int newRole;
    do {
        system("cls");
        cout << "\n\t������ �������������:\n"
            << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
            << "| ID | ��� ������������ | ���� � �������  |\n";

        for (int i = 0; i < usersCount; ++i) {
            cout << "|  " << i + 1 << " | " << setw(15) << users[i].username << "  | " << setw(15) << (users[i].role == 1 ? "������������" : "�������������") << " |\n";
        }
        cout << "|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|\n\n"
        << "\n\t\t����:\n\t1. ������������� ������������\n\t2. ������� ������������\n\t3. ������� ������ �������� ��������\n\t4. �����\n\t\t>>>";
        choice=protection(1,4);

        switch (choice) {
        case 1:
            // �������������� ������������
            cout << "������� ID ������������ ��� ��������������: ";
            userId=protection(1,100);
            userId--;
            if (userId >= 0 && userId < usersCount) {
                cout << "������� ����� ��� ������������: ";
                cin >> newUsername;
                cout << "������� ����� ������: ";
                cin >> newPassword;
                cout << "������� ����� ���� (1 - ������������, 2 - �������������): ";
                newRole=protection(1,2);

                users[userId].username = newUsername;
                users[userId].password = newPassword;
                users[userId].role = newRole;
                saveUsers(users, usersCount);

                cout << "������������ ������� ��������������\n";
                Sleep(2000);
                system("cls");
            }
            else {
                cout << "������������ � ��������� ID �� ������\n";
            }
            break;
        case 2:
            // �������� ������������
            char confirmation;
            cout << "������� ID ������������ ��� ��������: ";
            userId=protection(1,100);
            userId--;
            cout << "�� �������, ��� ������ ������� ������������? (y/n): ";
            cin >> confirmation;
            if (confirmation == 'y' || confirmation == 'Y') 
                if (onlineUser != userId) {
                    if (userId >= 0 && userId < usersCount) {
                        for (int i = userId; i < usersCount - 1; ++i) {
                            users[i] = users[i + 1];
                        }
                        --usersCount;
                        saveUsers(users, usersCount);
                        cout << "������������ ������� ������\n";
                        Sleep(2000);
                    }
                    else {
                        cout << "������������ � ��������� ID �� ������\n";
                    }
                }
            else {
                cout << "�� �� ������ ������� ����\n";
                Sleep(2000);
            }
            break;
        case 3:
            changeUserInfo(users, onlineUser, usersCount);
            break;
        case 4:
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 4);
}



// ������� �������� ������������ � ��������������
void deleteUser(User users[], int& usersCount, int& onlineUser) {
    int userId;
    char confirmation;

    cout << "������� ID ������������ ��� ��������: ";
    userId = protection(1, 100);
    userId--;

    if (userId >= 0 && userId < usersCount) {
        cout << "�� �������, ��� ������ ������� ������������? (y/n): ";
        cin >> confirmation;
        if (confirmation == 'y' || confirmation == 'Y') {
            for (int i = userId; i < usersCount - 1; ++i) {
                users[i] = users[i + 1];
            }
            --usersCount;
            if (onlineUser == userId) {
                onlineUser = -1;
            }
            saveUsers(users, usersCount);
            cout << "������������ ������� ������\n";
        }
        else {
            cout << "�������� ��������\n";
        }
    }
    else {
        cout << "������������ � ��������� ID �� ������\n";
    }
    Sleep(2000);
}
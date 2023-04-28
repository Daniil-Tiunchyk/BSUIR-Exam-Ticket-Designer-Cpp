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

    cout << "������������ ������� ���������������\n";
    Sleep(2000);
    system("cls");
}

// ������� ����������� ������������ � ����������
int loginUser(User users[], int usersCount) {
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
                return users[i].role;
            }
        }

        cout << "�������� ��� ������������ ��� ������. �������� �������: " << --loginAttempts << endl;
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
void adminActions(User users[], int usersCount) {
    system("cls");
    loadUsers(users, usersCount);

    int choice;
    do {
        cout << "�������� ��������:\n\t1. ������ � ��������\n\t2. ���������� ��������������\n\t3. �����\n\t\t>>>";
        choice=protection(1,3);
        switch (choice) {
        case 1:
            userActions();
            break;
        case 2:
            manageUsers(users, usersCount);
            break;
        case 3:
            cout << "�����\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 3);
}

// ������� ���������� �������������� (������ ��� ��������������)
void manageUsers(User users[], int usersCount) {
    int choice, userId;
    string newUsername, newPassword;
    int newRole;
    do {
        cout << "�������� ��������:\n1. ����������� ������ �������������\n2. ������������� ������������\n3. ������� ������������\n4. �����\n";
        choice=protection(1,4);

        switch (choice) {
        case 1:
            cout << "������ �������������:\n";
            for (int i = 0; i < usersCount; ++i) {
                cout << "ID ������������: " << i << ", ��� ������������: " << users[i].username << ", ����: " << (users[i].role == 1 ? "������������" : "�������������") << endl;
            }
            break;
        case 2:
            // �������������� ������������
            cout << "������� ID ������������ ��� ��������������: ";
            userId=protection(1,100);
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
        case 3:
            // �������� ������������
            cout << "������� ID ������������ ��� ��������: ";
            userId=protection(1,100);
            if (userId >= 0 && userId < usersCount) {
                for (int i = userId; i < usersCount - 1; ++i) {
                    users[i] = users[i + 1];
                }
                --usersCount;
                saveUsers(users, usersCount);
                cout << "������������ ������� ������\n";
            }
            else {
                cout << "������������ � ��������� ID �� ������\n";
            }
            break;
        case 4:
            cout << "�����...\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 4);
}
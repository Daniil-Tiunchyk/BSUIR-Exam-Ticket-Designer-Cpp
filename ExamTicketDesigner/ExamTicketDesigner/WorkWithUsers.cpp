#include "WorkWithUsers.h"
#include "Protection.h"


// Функция регистрации нового пользователя с валидацией
void registerUser(User users[], int& usersCount) {
	const int maxUsers = 100;
	User newUser;
	string confirmPassword;
	bool validInput;
    cout << "Регистрация нового пользователя\n";

    // Валидация имени пользователя
    do {
        validInput = true;
        cout << "\nВведите имя пользователя: ";
        cin >> newUser.username;

        for (int i = 0; i < usersCount; ++i) {
            if (users[i].username == newUser.username) {
                cout << "Имя пользователя уже существует, попробуйте еще раз\n";
                validInput = false;
                break;
            }
        }
    } while (!validInput);

    // Валидация пароля
    do {
        cout << "Введите пароль: ";
        newUser.password=inputPassword();
        cout << "\nПодтвердите пароль: ";
        confirmPassword = inputPassword();

        if (newUser.password != confirmPassword) {
            cout << "Пароли не совпадают, попробуйте еще раз\n";
        }
    } while (newUser.password != confirmPassword);

    newUser.role = 1;
    newUser.password = doHashNow(newUser.password);

    users[usersCount++] = newUser;

    saveUsers(users, usersCount);

    cout << "Пользователь успешно зарегистрирован\n";
    Sleep(2000);
    system("cls");
}

// Функция авторизации пользователя с валидацией
int loginUser(User users[], int usersCount) {
    string inputUsername, password;
    int loginAttempts = 3;
    while (loginAttempts > 0) {
        cout << "Введите имя пользователя: ";
        cin >> inputUsername;
        cout << "Введите пароль: ";
        password = inputPassword();
        password = doHashNow(password);
        for (int i = 0; i < usersCount; ++i) {
            if (users[i].username == inputUsername && users[i].password == password) {
                cout << "\nУспешная авторизация\n\n";
                return users[i].role;
            }
        }

        cout << "Неверное имя пользователя или пароль. Осталось попыток: " << --loginAttempts << endl;
    }
    cout << "Авторизация не удалась. Пожалуйста, попробуйте позже\n";
    return 0;
}

// Функция сохранения пользователей в файл
void saveUsers(User users[], int usersCount) {
    ofstream usersFile(usersFilename);
    if (!usersFile) {
        cerr << "Ошибка открытия файла пользователей\n";
        return;
    }

    for (int i = 0; i < usersCount; ++i) {
        usersFile << users[i].username << " " << users[i].password << " " << users[i].role << endl;
    }

    usersFile.close();
}

// Функция загрузки пользователей из файла
void loadUsers(User users[], int& usersCount) {
    ifstream usersFile(usersFilename);
    if (!usersFile) {
        cerr << "Ошибка открытия файла пользователей. Создание нового файла...\n";
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

// Функция, реализующая действия администратора
void adminActions(User users[], int usersCount) {
    system("cls");
    loadUsers(users, usersCount);

    int choice;
    do {
        cout << "Выберите действие:\n\t1. Работа с билетами\n\t2. Управление пользователями\n\t3. Выйти\n\t\t>>>";
        choice=protection(1,3);
        switch (choice) {
        case 1:
            userActions();
            break;
        case 2:
            manageUsers(users, usersCount);
            break;
        case 3:
            cout << "Выход\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 3);
}

// Функция управления пользователями (только для администратора)
void manageUsers(User users[], int usersCount) {
    int choice, userId;
    string newUsername, newPassword;
    int newRole;
    do {
        cout << "Выберите действие:\n1. Просмотреть список пользователей\n2. Редактировать пользователя\n3. Удалить пользователя\n4. Выйти\n";
        choice=protection(1,4);

        switch (choice) {
        case 1:
            cout << "Список пользователей:\n";
            for (int i = 0; i < usersCount; ++i) {
                cout << "ID пользователя: " << i << ", имя пользователя: " << users[i].username << ", роль: " << (users[i].role == 1 ? "пользователь" : "администратор") << endl;
            }
            break;
        case 2:
            // Редактирование пользователя
            cout << "Введите ID пользователя для редактирования: ";
            userId=protection(1,100);
            if (userId >= 0 && userId < usersCount) {
                cout << "Введите новое имя пользователя: ";
                cin >> newUsername;
                cout << "Введите новый пароль: ";
                cin >> newPassword;
                cout << "Введите новую роль (1 - пользователь, 2 - администратор): ";
                newRole=protection(1,2);

                users[userId].username = newUsername;
                users[userId].password = newPassword;
                users[userId].role = newRole;
                saveUsers(users, usersCount);

                cout << "Пользователь успешно отредактирован\n";
                Sleep(2000);
                system("cls");
            }
            else {
                cout << "Пользователь с указанным ID не найден\n";
            }
            break;
        case 3:
            // Удаление пользователя
            cout << "Введите ID пользователя для удаления: ";
            userId=protection(1,100);
            if (userId >= 0 && userId < usersCount) {
                for (int i = userId; i < usersCount - 1; ++i) {
                    users[i] = users[i + 1];
                }
                --usersCount;
                saveUsers(users, usersCount);
                cout << "Пользователь успешно удален\n";
            }
            else {
                cout << "Пользователь с указанным ID не найден\n";
            }
            break;
        case 4:
            cout << "Выход...\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 4);
}
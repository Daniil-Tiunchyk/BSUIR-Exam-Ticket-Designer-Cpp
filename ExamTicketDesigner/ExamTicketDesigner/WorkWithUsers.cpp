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

    cout << "\nПользователь успешно зарегистрирован\n";
    Sleep(2000);
    system("cls");
}

// Функция авторизации пользователя с валидацией
int loginUser(User users[], int usersCount, int& onlineUser) {
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
                onlineUser = i;
                return users[i].role;
            }
        }

        cout << "\nНеверное имя пользователя или пароль. Осталось попыток: " << --loginAttempts << endl;
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
void adminActions(User users[], int usersCount, int onlineUser) {
    loadUsers(users, usersCount);

    int choice;
    do {
        system("cls");
        cout << "\n\t\tМеню:\n\t1. Работа с билетами\n\t2. Управление пользователями\n\t3. Выйти\n\t\t>>> ";
        choice=protection(1,3);
        switch (choice) {
        case 1:
            userActions(users, usersCount, onlineUser);
            break;
        case 2:
            manageUsers(users, usersCount, onlineUser);
            break;
        case 3:
            cout << "Выход\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 3);
}

// Функция изменения имени пользователя и пароля
void changeUserInfo(User users[], int& onlineUser, int usersCount) {
    string newUsername, newPassword, confirmPassword;
    bool validInput;

    // Валидация нового имени пользователя
    do {
        validInput = true;
        cout << "\nВведите новое имя пользователя: ";
        cin >> newUsername;

        for (int i = 0; i < usersCount; ++i) {
            if (i != onlineUser && users[i].username == newUsername) {
                cout << "Имя пользователя уже существует, попробуйте еще раз\n";
                validInput = false;
                break;
            }
        }
    } while (!validInput);

    // Валидация нового пароля
    do {
        cout << "Введите новый пароль: ";
        newPassword = inputPassword();
        cout << "\nПодтвердите новый пароль: ";
        confirmPassword = inputPassword();

        if (newPassword != confirmPassword) {
            cout << "Пароли не совпадают, попробуйте еще раз\n";
        }
    } while (newPassword != confirmPassword);

    // Сохранение изменений
    users[onlineUser].username = newUsername;
    users[onlineUser].password = doHashNow(newPassword);
    saveUsers(users, usersCount);

    cout << "\nВаша информация успешно обновлена\n";
    Sleep(2000);
    system("cls");
}

// Функция управления пользователями (только для администратора)
void manageUsers(User users[], int usersCount, int onlineUser) {
    int choice, userId;
    string newUsername, newPassword;
    int newRole;
    do {
        system("cls");
        cout << "\n\tСписок пользователей:\n"
            << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
            << "| ID | Имя Пользователя | Роль в системе  |\n";

        for (int i = 0; i < usersCount; ++i) {
            cout << "|  " << i + 1 << " | " << setw(15) << users[i].username << "  | " << setw(15) << (users[i].role == 1 ? "пользователь" : "администратор") << " |\n";
        }
        cout << "|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|\n\n"
        << "\n\t\tМеню:\n\t1. Редактировать пользователя\n\t2. Удалить пользователя\n\t3. Сменить данные текущего аккаунта\n\t4. Выйти\n\t\t>>>";
        choice=protection(1,4);

        switch (choice) {
        case 1:
            // Редактирование пользователя
            cout << "Введите ID пользователя для редактирования: ";
            userId=protection(1,100);
            userId--;
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
        case 2:
            // Удаление пользователя
            char confirmation;
            cout << "Введите ID пользователя для удаления: ";
            userId=protection(1,100);
            userId--;
            cout << "Вы уверены, что хотите удалить пользователя? (y/n): ";
            cin >> confirmation;
            if (confirmation == 'y' || confirmation == 'Y') 
                if (onlineUser != userId) {
                    if (userId >= 0 && userId < usersCount) {
                        for (int i = userId; i < usersCount - 1; ++i) {
                            users[i] = users[i + 1];
                        }
                        --usersCount;
                        saveUsers(users, usersCount);
                        cout << "Пользователь успешно удален\n";
                        Sleep(2000);
                    }
                    else {
                        cout << "Пользователь с указанным ID не найден\n";
                    }
                }
            else {
                cout << "Вы не можете удалить себя\n";
                Sleep(2000);
            }
            break;
        case 3:
            changeUserInfo(users, onlineUser, usersCount);
            break;
        case 4:
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 4);
}



// Функция удаления пользователя с подтверждением
void deleteUser(User users[], int& usersCount, int& onlineUser) {
    int userId;
    char confirmation;

    cout << "Введите ID пользователя для удаления: ";
    userId = protection(1, 100);
    userId--;

    if (userId >= 0 && userId < usersCount) {
        cout << "Вы уверены, что хотите удалить пользователя? (y/n): ";
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
            cout << "Пользователь успешно удален\n";
        }
        else {
            cout << "Удаление отменено\n";
        }
    }
    else {
        cout << "Пользователь с указанным ID не найден\n";
    }
    Sleep(2000);
}
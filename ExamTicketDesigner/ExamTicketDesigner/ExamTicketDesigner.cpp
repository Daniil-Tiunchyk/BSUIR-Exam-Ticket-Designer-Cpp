#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

using namespace std;

struct Question {
    string questionText;
    int questionType; // 1 - выбор одного ответа, 2 - выбор нескольких ответов, 3 - вопрос с открытым ответом
    string answers[4];
};

struct ExamTicket {
    int ticketID;
    string subject;
    Question questions[5];
};

struct User {
    string username;
    string password;
    int role; // 1 - пользователь, 2 - администратор
};

const string usersFilename = "users.txt";
const string ticketsFilename = "tickets.txt";
const int maxUsers = 100;
const int maxTickets = 100;
User currentUser;
User users[maxUsers];
int usersCount = 0;
ExamTicket tickets[maxTickets];
int ticketsCount = 0;

void registerUser();
void loginUser();
void saveUsers();
void loadUsers();
void saveTickets();
void loadTickets();
void userActions();
void adminActions();
void createTicket();
void editTicket();
void deleteTicket();
void manageUsers();
void showTickets();

int main() {
    loadUsers();
    loadTickets();

    int choice;
    do {
        cout << "Выберите действие:\n1. Регистрация\n2. Вход\n3. Выход\n";
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser();
            break;
        case 3:
            cout << "Выход...\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 3);

    return 0;
}
// Функция регистрации нового пользователя с валидацией
void registerUser() {
    User newUser;
    string confirmPassword;
    bool validInput;

    cout << "Регистрация нового пользователя\n";

    // Валидация имени пользователя
    do {
        validInput = true;
        cout << "Введите имя пользователя: ";
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
        cin >> newUser.password;
        cout << "Подтвердите пароль: ";
        cin >> confirmPassword;

        if (newUser.password != confirmPassword) {
            cout << "Пароли не совпадают, попробуйте еще раз\n";
        }
    } while (newUser.password != confirmPassword);

    cout << "Введите роль (1 - пользователь, 2 - администратор): ";
    cin >> newUser.role;

    users[usersCount++] = newUser;
    newUser.password = sha256(newUser.password);

    saveUsers();

    cout << "Пользователь успешно зарегистрирован\n";
}

// Функция авторизации пользователя с валидацией
void loginUser() {
    string inputUsername, inputPassword;
    int loginAttempts = 3;

    while (loginAttempts > 0) {
        cout << "Введите имя пользователя: ";
        cin >> inputUsername;
        cout << "Введите пароль: ";
        cin >> inputPassword;

        for (int i = 0; i < usersCount; ++i) {
            if (users[i].username == inputUsername && users[i].password == sha256(inputPassword)) {
                currentUser = users[i];
                cout << "Успешная авторизация\n";
                return;
            }
        }

        cout << "Неверное имя пользователя или пароль. Осталось попыток: " << --loginAttempts << endl;
    }

    cout << "Авторизация не удалась. Пожалуйста, попробуйте позже\n";
    exit(0);
}

// Функция сохранения пользователей в файл
void saveUsers() {
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
void loadUsers() {
    ifstream usersFile(usersFilename);

    if (!usersFile) {
        cerr << "Ошибка открытия файла пользователей\n";
        return;
    }

    usersCount = 0;
    while (usersFile >> users[usersCount].username >> users[usersCount].password >> users[usersCount].role) {
        ++usersCount;
    }

    usersFile.close();
}
// Функция сохранения экзаменационных билетов в файл
void saveTickets() {
    ofstream ticketsFile(ticketsFilename);

    if (!ticketsFile) {
        cerr << "Ошибка открытия файла с билетами\n";
        return;
    }

    for (int i = 0; i < ticketsCount; ++i) {
        ticketsFile << tickets[i].ticketID << " " << tickets[i].subject << endl;
        for (int j = 0; j < 5; ++j) {
            ticketsFile << tickets[i].questions[j].questionText << " " << tickets[i].questions[j].questionType << endl;
            for (int k = 0; k < 4; ++k) {
                ticketsFile << tickets[i].questions[j].answers[k] << endl;
            }
        }
    }

    ticketsFile.close();
}

// Функция загрузки экзаменационных билетов из файла
void loadTickets() {
    ifstream ticketsFile(ticketsFilename);

    if (!ticketsFile) {
        cerr << "Ошибка открытия файла с билетами\n";
        return;
    }

    ticketsCount = 0;
    while (ticketsFile >> tickets[ticketsCount].ticketID >> tickets[ticketsCount].subject) {
        for (int j = 0; j < 5; ++j) {
            ticketsFile >> tickets[ticketsCount].questions[j].questionText >> tickets[ticketsCount].questions[j].questionType;
            for (int k = 0; k < 4; ++k) {
                ticketsFile >> tickets[ticketsCount].questions[j].answers[k];
            }
        }
        ++ticketsCount;
    }

    ticketsFile.close();
}

// Функция, реализующая действия пользователя
void userActions() {
    int choice;
    do {
        cout << "Выберите действие:\n1. Создать билет\n2. Редактировать билет\n3. Удалить билет\n4. Показать билеты\n5. Поиск билетов\n6. Выйти\n";
        cin >> choice;

        switch (choice) {
        case 1:
            createTicket();
            break;
        case 2:
            editTicket();
            break;
        case 3:
            deleteTicket();
            break;
        case 4:
            showTickets();
            break;
        case 5:
            searchTickets(); // Вызов функции поиска
            break;
        case 6:
            cout << "Выход...\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 6);
}
// Функция, реализующая действия администратора
void adminActions() {
    int choice;
    do {
        cout << "Выберите действие:\n1. Создать билет\n2. Редактировать билет\n3. Удалить билет\n4. Показать билеты\n5. Поиск билетов\n6. Управление пользователями\n7. Выйти\n";
        cin >> choice;

        switch (choice) {
        case 1:
            createTicket();
            break;
        case 2:
            editTicket();
            break;
        case 3:
            deleteTicket();
            break;
        case 4:
            showTickets();
            break;
        case 5:
            searchTickets(); // Вызов функции поиска
            break;
        case 6:
            manageUsers();
            break;
        case 7:
            cout << "Выход...\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 7);
}

// Функция создания экзаменационного билета
void createTicket() {
    ExamTicket newTicket;
    cout << "Введите ID билета: ";
    cin >> newTicket.ticketID;
    cout << "Введите предмет: ";
    cin.ignore();
    getline(cin, newTicket.subject);

    for (int i = 0; i < 5; ++i) {
        cout << "Введите текст вопроса " << i + 1 << ": ";
        getline(cin, newTicket.questions[i].questionText);
        cout << "Введите тип вопроса (1 - выбор одного ответа, 2 - выбор нескольких ответов, 3 - вопрос с открытым ответом): ";
        cin >> newTicket.questions[i].questionType;
        cin.ignore();
        if (newTicket.questions[i].questionType != 3) {
            for (int j = 0; j < 4; ++j) {
                cout << "Введите вариант ответа " << j + 1 << ": ";
                getline(cin, newTicket.questions[i].answers[j]);
            }
        }
    }

    tickets[ticketsCount++] = newTicket;
    saveTickets();

    cout << "Экзаменационный билет успешно создан\n";
}

// Функция редактирования экзаменационного билета
void editTicket() {
    int ticketID;
    cout << "Введите ID билета для редактирования: ";
    cin >> ticketID;

    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            // Введите новые данные для билета и сохраните их (аналогично функции createTicket)
            // ...
            saveTickets();
            cout << "Экзаменационный билет успешно отредактирован\n";
            return;
        }
    }

    cout << "Билет с указанным ID не найден\n";
}

// Функция удаления экзаменационного билета
void deleteTicket() {
    int ticketID;
    cout << "Введите ID билета для удаления: ";
    cin >> ticketID;
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            for (int j = i; j < ticketsCount - 1; ++j) {
                tickets[j] = tickets[j + 1];
            }
            --ticketsCount;
            saveTickets();
            cout << "Экзаменационный билет успешно удален\n";
            return;
        }
    }

    cout << "Билет с указанным ID не найден\n";
}

// Функция управления пользователями (только для администратора)
void manageUsers() {
    int choice, userId;
    string newUsername, newPassword;
    int newRole;

    do {
        cout << "Выберите действие:\n1. Просмотреть список пользователей\n2. Редактировать пользователя\n3. Удалить пользователя\n4. Выйти\n";
        cin >> choice;

        switch (choice) {
        case 1:
            // Вывод списка пользователей и их ролей
            cout << "Список пользователей:\n";
            for (int i = 0; i < usersCount; ++i) {
                cout << "ID пользователя: " << i << ", имя пользователя: " << users[i].username << ", роль: " << (users[i].role == 1 ? "пользователь" : "администратор") << endl;
            }
            break;
        case 2:
            // Редактирование пользователя
            cout << "Введите ID пользователя для редактирования: ";
            cin >> userId;
            if (userId >= 0 && userId < usersCount) {
                cout << "Введите новое имя пользователя: ";
                cin >> newUsername;
                cout << "Введите новый пароль: ";
                cin >> newPassword;
                cout << "Введите новую роль (1 - пользователь, 2 - администратор): ";
                cin >> newRole;

                users[userId].username = newUsername;
                users[userId].password = newPassword;
                users[userId].role = newRole;
                saveUsers();

                cout << "Пользователь успешно отредактирован\n";
            }
            else {
                cout << "Пользователь с указанным ID не найден\n";
            }
            break;
        case 3:
            // Удаление пользователя
            cout << "Введите ID пользователя для удаления: ";
            cin >> userId;
            if (userId >= 0 && userId < usersCount) {
                for (int i = userId; i < usersCount - 1; ++i) {
                    users[i] = users[i + 1];
                }
                --usersCount;
                saveUsers();

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


// Функция отображения списка экзаменационных билетов
void showTickets() {
    for (int i = 0; i < ticketsCount; ++i) {
        cout << "ID билета: " << tickets[i].ticketID << ", предмет: " << tickets[i].subject << endl;
        for (int j = 0; j < 5; ++j) {
            cout << "Вопрос " << j + 1 << ": " << tickets[i].questions[j].questionText << ", тип вопроса: " << tickets[i].questions[j].questionType << endl;
            if (tickets[i].questions[j].questionType != 3) {
                cout << "Варианты ответов:" << endl;
                for (int k = 0; k < 4; ++k) {
                    cout << tickets[i].questions[j].answers[k] << endl;
                }
            }
        }
        cout << "---------------------------------------" << endl;
    }
}

string sha256(const string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Final(hash, &sha256);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}
void searchTickets() {
    string subject;
    cout << "Введите предмет для поиска билетов: ";
    cin.ignore();
    getline(cin, subject);

    bool found = false;
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].subject == subject) {
            found = true;
            cout << "ID билета: " << tickets[i].ticketID << ", предмет: " << tickets[i].subject << endl;
        }
    }

    if (!found) {
        cout << "Экзаменационные билеты по заданному предмету не найдены." << endl;
    }
}

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
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


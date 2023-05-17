#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "makeHash.h"
#include <Windows.h>
#include "WorkWithTickets.h"
#include "WorkWithUsers.h"
#include "Protection.h"


using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int maxUsers = 100;
    User users[maxUsers];
    int usersCount = 0;

    int choice, ind, onlineUser = 0;
    do {
        system("cls");
        cout << "\n\t\tМеню\n\t1. Регистрация\n\t2. Вход\n\t3. Выход\n\t\t>>> ";
        choice = protection(1,3);
        loadUsers(users, usersCount);
        switch (choice) {
        case 1:
            registerUser(users, usersCount);
            break;
        case 2:
            ind = loginUser(users, usersCount, onlineUser);
            if (ind == 1) userActions(users, usersCount, onlineUser);
            if (ind == 2) adminActions(users, usersCount, onlineUser);

            break;
        case 3:
            break;
        default:
            cout << "\tНеверный выбор, попробуйте еще раз\n\n";
        }
    } while (choice != 3);

    return 0;
}

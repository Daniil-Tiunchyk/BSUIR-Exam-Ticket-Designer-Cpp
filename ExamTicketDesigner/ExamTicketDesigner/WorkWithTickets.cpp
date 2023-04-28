#include "WorkWithTickets.h"
#include "Protection.h"

// Функция, реализующая действия пользователя
void userActions() {
    system("cls");
    int choice;
    const int maxTickets = 100;
    int ticketsCount = 0;
    ExamTicket tickets[maxTickets];
    loadTickets(ticketsFilename, tickets, ticketsCount);

    do {
        cout << "Меню:\n\t1. Создать билет\n\t2. Редактировать билет\n\t3. Удалить билет\n\t4. Показать билеты\n\t5. Поиск билетов\n\t6. Выйти\n\t\t>>> ";
        choice = protection(1,6);

        switch (choice) {
        case 1:
            createTicket(tickets, ticketsCount);
            break;
        case 2:
            editTicket(tickets, ticketsCount);
            break;
        case 3:
            deleteTicket(tickets, ticketsCount);
            break;
        case 4:
            showTickets(tickets, ticketsCount);
            break;
        case 5:
            searchTickets(tickets, ticketsCount);
            break;
        case 6:
            cout << "Выход\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 6);
}

// Функция создания экзаменационного билета
void createTicket(ExamTicket tickets[], int& ticketsCount) {
    ExamTicket newTicket;
    cout << "Введите номер билета: ";
    newTicket.ticketID = protection(1,100);
    cout << "Введите предмет: ";
    cin.ignore();
    getline(cin, newTicket.subject);

    for (int i = 0; i < 5; ++i) {
        cout << "\nВведите текст вопроса " << i + 1 << ": ";
        getline(cin, newTicket.questions[i].questionText);
        cout << "Введите тип вопроса (1 - выбор одного ответа, 2 - выбор нескольких ответов, 3 - вопрос с открытым ответом): ";
        newTicket.questions[i].questionType = protection(1,3);
        cin.ignore();
        if (newTicket.questions[i].questionType != 3) {
            for (int j = 0; j < 4; ++j) {
                cout << "Введите вариант ответа " << j + 1 << ": ";
                    getline(cin, newTicket.questions[i].answers[j]);
            }
        }
    }

    tickets[ticketsCount++] = newTicket;
        saveTickets(ticketsFilename, tickets, ticketsCount);

        cout << "Экзаменационный билет успешно создан\n";
}
// Функция редактирования экзаменационного билета
void editTicket(ExamTicket tickets[], int ticketsCount) {
    int ticketID;
    cout << "Введите номер билета для редактирования: ";
    ticketID=protection(1,100);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            // редактирование билета
            saveTickets(ticketsFilename, tickets, ticketsCount);
            cout << "Экзаменационный билет успешно отредактирован\n";
            return;
        }
    }

    cout << "Билет с указанным ID не найден\n";
}
// Функция удаления экзаменационного билета
void deleteTicket(ExamTicket tickets[], int& ticketsCount) {
    int ticketID;
    cout << "Введите номер билета для удаления: ";
    ticketID=protection(1,100);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            for (int j = i; j < ticketsCount - 1; ++j) {
                tickets[j] = tickets[j + 1];
            }
            --ticketsCount;
            saveTickets(ticketsFilename, tickets, ticketsCount);
            cout << "Экзаменационный билет успешно удален\n";
            return;
        }
    }
    cout << "Билет с указанным ID не найден\n";
}

// Функция отображения списка экзаменационных билетов
void showTickets(const ExamTicket tickets[], int ticketsCount) {
    for (int i = 0; i < ticketsCount; ++i) {
        cout << "Номер билета: " << tickets[i].ticketID << ", Предмет: " << tickets[i].subject << endl;
        for (int j = 0; j < 5; ++j) {
            cout << "Вопрос " << j + 1 << ": " << tickets[i].questions[j].questionText << ", тип вопроса: " << tickets[i].questions[j].questionType << endl;
            if (tickets[i].questions[j].questionType != 3) {
                cout << "Варианты ответов:" << endl;
                for (int k = 0; k < 4; ++k) {
                    cout << tickets[i].questions[j].answers[k] << endl;
                }
            }
        }
        cout << "_______________________________" << endl;
    }
}

void searchTickets(const ExamTicket tickets[], int ticketsCount) {
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
// Функция сохранения экзаменационных билетов в файл
void saveTickets(const string& ticketsFilename, ExamTicket tickets[], int ticketsCount) {
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
void loadTickets(const string& ticketsFilename, ExamTicket tickets[], int& ticketsCount) {
    ifstream ticketsFile(ticketsFilename);
    if (!ticketsFile) {
        cerr << "Ошибка открытия файла с билетами. Создание нового файла...\n";
        ofstream newTicketsFile(ticketsFilename);
        newTicketsFile.close();
        ticketsCount = 0;
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

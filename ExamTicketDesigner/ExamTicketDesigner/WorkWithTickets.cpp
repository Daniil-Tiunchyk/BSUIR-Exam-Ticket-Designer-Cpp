#include "WorkWithTickets.h"
#include "Protection.h"

// Функция, реализующая действия пользователя
void userActions() {
    int choice;
    const int maxTickets = 100;
    int ticketsCount = 0;
    ExamTicket tickets[maxTickets];
    loadTickets(ticketsFilename, tickets, ticketsCount);
 
    do {
        system("cls");
        cout << "\n\t\tМеню:\n\t1. Создать билет\n\t2. Редактировать билет\n\t3. Удалить билет\n\t4. Показать билеты\n\t5. Поиск билетов\n\t6. Отсортировать билеты\n\t7. Выйти\n\t\t>>> ";
        choice = protection(1,7);

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
            sortTickets(tickets, ticketsCount);
            break;
         case7:
            cout << "Выход\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 7);
}

// Функция создания экзаменационного билета
void createTicket(ExamTicket tickets[], int& ticketsCount) {
    ExamTicket newTicket;
    cout << "Введите номер билета: ";
    newTicket.ticketID = protection(1,100);
    cout << "Введите предмет: ";
    cin >> newTicket.subject;
    cin.ignore();
    for (int i = 0; i < 5; ++i) {
        cout << "\nВведите текст вопроса " << i + 1 << ": ";
        getline(cin, newTicket.questions[i].questionText);
        cout << "Введите тип вопроса (1 - выбор одного ответа, 2 - выбор нескольких ответов, 3 - вопрос с открытым ответом): ";
        newTicket.questions[i].questionType = protection(1,3);
        if (newTicket.questions[i].questionType != 3) {
            for (int j = 0; j < 4; ++j) {
                cout << "\tВведите вариант ответа " << j + 1 << ": ";
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
    ticketID = protection(1, 100);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            cout << "Введите новый предмет: ";
            cin.ignore();
            getline(cin, tickets[i].subject);

            for (int j = 0; j < 5; ++j) {
                cout << "Введите новый текст вопроса " << j + 1 << ": ";
                getline(cin, tickets[i].questions[j].questionText);

                cout << "Введите новый тип вопроса " << j + 1 << ": ";
                tickets[i].questions[j].questionType = protection(1, 5);

                for (int a = 0; a < 4; ++a) {
                    cout << "Введите новый ответ " << a + 1 << " для вопроса " << j + 1 << ": ";
                    getline(cin, tickets[i].questions[j].answers[a]);
                }
            }

            Sleep(2000);
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
        cout << "\n\tСписок билетов:\n"
            << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
            << "| Номер билета " << setw(15) << " | " << setw(15) << " Предмет |\n"
            << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";

        for (int i = 0; i < ticketsCount; ++i) {
            cout << "| " << setw(15) << left << "№" << tickets[i].ticketID << " | " << setw(15) << left << tickets[i].subject << "|\n";
            for (int j = 0; j < 5; ++j) {
                cout << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
                    << "| Вопрос " << j + 1 << ": " << setw(15) << left << tickets[i].questions[j].questionText << ", тип вопроса: " << tickets[i].questions[j].questionType << endl;
                if (tickets[i].questions[j].questionType != 3) {
                    cout << "\tВарианты ответов:\n";
                    for (int k = 0; k < 4; ++k) {
                        cout << "\t\t" << tickets[i].questions[j].answers[k] << "\n";
                    }
                }
            }
            cout << "|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|\n\n";
        }
        cout << "Введите любое число, чтобы продолжить\n\t>>> ";
        system("pause");
}


void searchTickets(const ExamTicket tickets[], int ticketsCount) {
    string subject;
    cout << "Введите предмет для поиска билетов: ";
    //cin.ignore();
    getline(cin, subject);
    bool found = false;
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].subject == subject) {
            found = true;
            cout << "Номер билета: " << tickets[i].ticketID << ", предмет: " << tickets[i].subject << endl;
        }
    }

    if (!found) {
        cout << "Экзаменационные билеты по заданному предмету не найдены.\n";
    }
    system("pause");
}
void sortTickets(ExamTicket tickets[], int ticketsCount) {
    for (int i = 0; i < ticketsCount - 1; ++i) {
        for (int j = 0; j < ticketsCount - i - 1; ++j) {
            if (tickets[j].subject > tickets[j + 1].subject) {
                ExamTicket temp = tickets[j];
                tickets[j] = tickets[j + 1];
                tickets[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < ticketsCount; ++i) {
        cout << "Номер билета: " << tickets[i].ticketID << ", Предмет: " << tickets[i].subject << endl;
        for (int j = 0; j < 5; ++j) {
            cout << "Вопрос " << j + 1 << ": " << tickets[i].questions[j].questionText << ", тип вопроса: " << tickets[i].questions[j].questionType << endl;
            if (tickets[i].questions[j].questionType != 3) {
                cout << "Варианты ответов:\n";
                for (int k = 0; k < 4; ++k) {
                    cout << tickets[i].questions[j].answers[k] << endl;
                }
            }
        }
        cout << "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n";
    }
    system("pause");
}
void writeQuestion(ofstream& out, const Question& question) {
    out << question.questionText << endl;
    out << question.questionType << endl;
    for (const string& answer : question.answers) {
        out << answer << endl;
    }
}

void readQuestion(ifstream& in, Question& question) {
    getline(in, question.questionText);
    in >> question.questionType;
    in.ignore();
    for (string& answer : question.answers) {
        getline(in, answer);
    }
}

void writeExamTicket(ofstream& out, const ExamTicket& ticket) {
    out << ticket.ticketID << endl;
    out << ticket.subject << endl;
    for (const Question& question : ticket.questions) {
        writeQuestion(out, question);
    }
}

void readExamTicket(ifstream& in, ExamTicket& ticket) {
    in >> ticket.ticketID;
    in.ignore();
    getline(in, ticket.subject);
    for (Question& question : ticket.questions) {
        readQuestion(in, question);
    }
}

// Функция сохранения массива ExamTicket в файл
void saveTickets(const string& filename, ExamTicket tickets[], int ticketsCount) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Не удалось открыть файл для записи: " << filename << endl;
        return;
    }

    out << ticketsCount << endl;
    for (int i = 0; i < ticketsCount; ++i) {
        writeExamTicket(out, tickets[i]);
    }

    out.close();
}

// Функция загрузки массива ExamTicket из файла
void loadTickets(const string& filename, ExamTicket tickets[], int& ticketsCount) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Не удалось открыть файл для чтения: " << filename << endl;
        return;
    }

    in >> ticketsCount;
    in.ignore();
    for (int i = 0; i < ticketsCount; ++i) {
        readExamTicket(in, tickets[i]);
    }

    in.close();
}
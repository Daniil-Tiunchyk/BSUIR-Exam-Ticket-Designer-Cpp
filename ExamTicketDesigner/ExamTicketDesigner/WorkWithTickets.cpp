#include "WorkWithTickets.h"
#include "Protection.h"

// Функция, реализующая действия пользователя
void userActions(User users[], int usersCount, int onlineUser) {
    int choice;
    const int maxTickets = 100;
    int ticketsCount = 0;
    ExamTicket tickets[maxTickets];
    loadTickets(ticketsFilename, tickets, ticketsCount);
 
    do {
        system("cls");
        cout << "\n\t\tМеню:\n\t1. Создать билет\n\t2. Редактировать билет\n\t3. Редактировать вопрос в билете\n\t4. Удалить билет\n\t5. Показать билеты\n\t6. Поиск билетов\n\t7. Отсортировать билеты\n\t8. Сохранить билеты\n\t9. Сменить данные текущего аккаунта\n\t10. Выйти\n\t\t>>> ";
        choice = protection(1, 10);

        switch (choice) {
        case 1:
            createTicket(tickets, ticketsCount);
            break;
        case 2:
            editTicket(tickets, ticketsCount);
            break;
        case 3:
            showTickets(tickets, ticketsCount);
            editQuestionInTicket(tickets, ticketsCount);
            break;
        case 4:
            deleteTicket(tickets, ticketsCount);
            break;
        case 5:
            showTickets(tickets, ticketsCount);
            break;
        case 6:
            searchTickets(tickets, ticketsCount);
            break;
        case 7: 
            sortTickets(tickets, ticketsCount);
            break;
        case 8:
            saveTickets(ticketsFilename, tickets, ticketsCount);
            cout << "Билеты успешно сохранены.\n";
            break;
        case 9:
            changeUserInfo(users, onlineUser, usersCount);
            break;
        case 10:
            cout << "Выход\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 10);
}

// Вспомогательная функция для перемешивания вопросов
void shuffleQuestions(Question questions[], int questionsCount) {
    for (int i = 0; i < questionsCount - 1; ++i) {
        int j = i + rand() % (questionsCount - i);
        swap(questions[i], questions[j]);
    }
}

void createTicket(ExamTicket tickets[], int& ticketsCount) {
    int choice;
    cout << "\t1. Генерация одного билета\n\t2. Ввод списка вопросов\n\t3. Назад\n\t\t>>> ";
    choice = protection(1, 3);

    switch (choice) {
    case 1: {
        ExamTicket newTicket;
        cout << "Введите номер билета: ";
        newTicket.ticketID = protection(1, 100);
        cout << "Введите предмет: ";
        cin >> newTicket.subject;
        cin.ignore();
        for (int i = 0; i < 5; ++i) {
            cout << "\nВведите текст вопроса " << i + 1 << ": ";
            getline(cin, newTicket.questions[i].questionText);
            cout << "Введите тип вопроса (1 - выбор одного ответа, 2 - выбор нескольких ответов, 3 - вопрос с открытым ответом): ";
            newTicket.questions[i].questionType = protection(1, 3);
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
        system("pause");
        break;
    }
    case 2: {
        Question questions[100];
        int questionsCount = 0;

        cout << "Введите вопросы (напишите \"Стоп\" для завершения):\n";
        while (true) {
            string input;
            getline(cin, input);

            if (input == "Стоп") break;

            // Сохраняем вопрос
            questions[questionsCount].questionText = input;

            cout << "Введите тип вопроса (1 - выбор одного ответа, 2 - выбор нескольких ответов, 3 - вопрос с открытым ответом): ";
            questions[questionsCount].questionType = protection(1, 3);
            if (questions[questionsCount].questionType != 3) {
                for (int j = 0; j < 4; ++j) {
                    cout << "\tВведите вариант ответа " << j + 1 << ": ";
                    getline(cin, questions[questionsCount].answers[j]);
                }
            }
            questionsCount++;
        }

        // Перемешиваем вопросы
        shuffleQuestions(questions, questionsCount);

        // Создаем билеты
        while (questionsCount >= 5) {
            ExamTicket newTicket;
            newTicket.ticketID = ticketsCount + 1;
            cout << "Введите предмет для билета " << newTicket.ticketID << ": ";
            cin >> newTicket.subject;
            cin.ignore();

            // Заполняем билет вопросами
            for (int i = 0; i < 5; ++i) {
                newTicket.questions[i] = questions[questionsCount - 5 + i];
            }
            tickets[ticketsCount++] = newTicket;
            saveTickets(ticketsFilename, tickets, ticketsCount);
            questionsCount -= 5;
        }

        // Если остались вопросы, выводим их на экран
        if (questionsCount > 0) {
            cout << "Остались невостребованные вопросы:\n";
            for (int i = 0; i < questionsCount; ++i) {
                cout << "\tВопрос " << i + 1 << ": " << questions[i].questionText << "\n";
            }
        }
        cout << "\nЭкзаменационные билеты успешно созданы\n";
        system("pause");
        break;
    }
    case 3:
        return;
    }
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
                cout << "| Вопрос " << j + 1 << ": " << setw(15) << left << tickets[i].questions[j].questionText << ", тип вопроса: " << tickets[i].questions[j].questionType << endl;
                if (tickets[i].questions[j].questionType != 3) {
                    cout << "\tВарианты ответов:\n";
                    for (int k = 0; k < 4; ++k) {
                        cout << "\t\t" << tickets[i].questions[j].answers[k] << "\n";
                    }
                }
            }
            cout << "|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|\n\n\n";
        }
        system("pause");
}


void searchTickets(const ExamTicket tickets[], int ticketsCount) {
    int choice;
    cout << "\t\tВыберите параметр для поиска: \n";
    cout << "\t1. Предмет\n";
    cout << "\t2. Номер билета\n";
    cout << "\t3. Тип вопроса\n";
    cin >> choice;
    cin.ignore();

    bool found = false;

    switch (choice) {
    case 1: {
        string subject;
        cout << "Введите предмет для поиска билетов: ";
        getline(cin, subject);
        for (int i = 0; i < ticketsCount; ++i) {
            if (tickets[i].subject == subject) {
                found = true;
                cout << "Номер билета: " << tickets[i].ticketID << ", предмет: " << tickets[i].subject << endl;
            }
        }
        break;
    }
    case 2: {
        int ticketID;
        cout << "Введите номер билета для поиска: ";
        cin >> ticketID;
        for (int i = 0; i < ticketsCount; ++i) {
            if (tickets[i].ticketID == ticketID) {
                found = true;
                cout << "Номер билета: " << tickets[i].ticketID << ", предмет: " << tickets[i].subject << endl;
            }
        }
        break;
    }
    case 3: {
        int questionType;
        cout << "Введите тип вопроса для поиска: ";
        cin >> questionType;
        for (int i = 0; i < ticketsCount; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (tickets[i].questions[j].questionType == questionType) {
                    found = true;
                    cout << "Номер билета: " << tickets[i].ticketID << ", предмет: " << tickets[i].subject << ", тип вопроса: " << tickets[i].questions[j].questionType << endl;
                }
            }
        }
        break;
    }
    default: {
        cout << "Неверный выбор, попробуйте еще раз\n";
        return searchTickets(tickets, ticketsCount);
    }
    }

    if (!found) {
        cout << "Экзаменационные билеты по заданному критерию не найдены.\n";
    }
    system("pause");
}

void sortTickets(ExamTicket tickets[], int ticketsCount) {
    int choice;
    do {
        system("cls");
        cout << "\n\t\tМеню сортировок:\n\t1. Сортировка по номеру билета\n\t2. Сортировка по номеру билета в обратном порядке\n\t3. Сортировка по предмету\n\t4. Выйти\n\t\t>>> ";
        choice = protection(1, 4);

        switch (choice) {
        case 1:
            for (int i = 0; i < ticketsCount - 1; ++i) {
                for (int j = 0; j < ticketsCount - i - 1; ++j) {
                    if (tickets[j].ticketID > tickets[j + 1].ticketID) {
                        ExamTicket temp = tickets[j];
                        tickets[j] = tickets[j + 1];
                        tickets[j + 1] = temp;
                    }
                }
            }
            showTickets(tickets, ticketsCount);
            break;
        case 2:
            for (int i = 0; i < ticketsCount - 1; ++i) {
                for (int j = 0; j < ticketsCount - i - 1; ++j) {
                    if (tickets[j].ticketID < tickets[j + 1].ticketID) {
                        ExamTicket temp = tickets[j];
                        tickets[j] = tickets[j + 1];
                        tickets[j + 1] = temp;
                    }
                }
            }
            showTickets(tickets, ticketsCount);
            break;
        case 3:
            for (int i = 0; i < ticketsCount - 1; ++i) {
                for (int j = 0; j < ticketsCount - i - 1; ++j) {
                    if (tickets[j].subject > tickets[j + 1].subject) {
                        ExamTicket temp = tickets[j];
                        tickets[j] = tickets[j + 1];
                        tickets[j + 1] = temp;
                    }
                }
            }
            showTickets(tickets, ticketsCount);
            break;
        case 4:
            cout << "Выход\n";
            break;
        default:
            cout << "Неверный выбор, попробуйте еще раз\n";
        }
    } while (choice != 4);

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

void editQuestionInTicket(ExamTicket tickets[], int ticketsCount) {
    int ticketID, questionNumber;
    cout << "Введите номер билета, в котором хотите редактировать вопрос: ";
    ticketID = protection(1, 100);
    cout << "Введите номер вопроса, который хотите редактировать (1-5): ";
    questionNumber = protection(1, 5);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            cout << "Введите новый текст вопроса: ";
            cin.ignore();
            getline(cin, tickets[i].questions[questionNumber - 1].questionText);

            cout << "Введите новый тип вопроса: ";
            tickets[i].questions[questionNumber - 1].questionType = protection(1, 5);

            for (int j = 0; j < 4; ++j) {
                cout << "Введите новый ответ " << j + 1 << ": ";
                getline(cin, tickets[i].questions[questionNumber - 1].answers[j]);
            }

            cout << "Вопрос успешно отредактирован.\n";
            return;
        }
    }

    cout << "Билет с указанным ID не найден\n";
}

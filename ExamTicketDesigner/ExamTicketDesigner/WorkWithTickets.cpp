#include "WorkWithTickets.h"
#include "Protection.h"

// �������, ����������� �������� ������������
void userActions(User users[], int usersCount, int onlineUser) {
    int choice;
    const int maxTickets = 100;
    int ticketsCount = 0;
    ExamTicket tickets[maxTickets];
    loadTickets(ticketsFilename, tickets, ticketsCount);
 
    do {
        system("cls");
        cout << "\n\t\t����:\n\t1. ������� �����\n\t2. ������������� �����\n\t3. ������������� ������ � ������\n\t4. ������� �����\n\t5. �������� ������\n\t6. ����� �������\n\t7. ������������� ������\n\t8. ��������� ������\n\t9. ������� ������ �������� ��������\n\t10. �����\n\t\t>>> ";
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
            cout << "������ ������� ���������.\n";
            break;
        case 9:
            changeUserInfo(users, onlineUser, usersCount);
            break;
        case 10:
            cout << "�����\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 10);
}

// ��������������� ������� ��� ������������� ��������
void shuffleQuestions(Question questions[], int questionsCount) {
    for (int i = 0; i < questionsCount - 1; ++i) {
        int j = i + rand() % (questionsCount - i);
        swap(questions[i], questions[j]);
    }
}

void createTicket(ExamTicket tickets[], int& ticketsCount) {
    int choice;
    cout << "\t1. ��������� ������ ������\n\t2. ���� ������ ��������\n\t3. �����\n\t\t>>> ";
    choice = protection(1, 3);

    switch (choice) {
    case 1: {
        ExamTicket newTicket;
        cout << "������� ����� ������: ";
        newTicket.ticketID = protection(1, 100);
        cout << "������� �������: ";
        cin >> newTicket.subject;
        cin.ignore();
        for (int i = 0; i < 5; ++i) {
            cout << "\n������� ����� ������� " << i + 1 << ": ";
            getline(cin, newTicket.questions[i].questionText);
            cout << "������� ��� ������� (1 - ����� ������ ������, 2 - ����� ���������� �������, 3 - ������ � �������� �������): ";
            newTicket.questions[i].questionType = protection(1, 3);
            if (newTicket.questions[i].questionType != 3) {
                for (int j = 0; j < 4; ++j) {
                    cout << "\t������� ������� ������ " << j + 1 << ": ";
                    getline(cin, newTicket.questions[i].answers[j]);
                }
            }
        }

        tickets[ticketsCount++] = newTicket;
        saveTickets(ticketsFilename, tickets, ticketsCount);

        cout << "��������������� ����� ������� ������\n";
        system("pause");
        break;
    }
    case 2: {
        Question questions[100];
        int questionsCount = 0;

        cout << "������� ������� (�������� \"����\" ��� ����������):\n";
        while (true) {
            string input;
            getline(cin, input);

            if (input == "����") break;

            // ��������� ������
            questions[questionsCount].questionText = input;

            cout << "������� ��� ������� (1 - ����� ������ ������, 2 - ����� ���������� �������, 3 - ������ � �������� �������): ";
            questions[questionsCount].questionType = protection(1, 3);
            if (questions[questionsCount].questionType != 3) {
                for (int j = 0; j < 4; ++j) {
                    cout << "\t������� ������� ������ " << j + 1 << ": ";
                    getline(cin, questions[questionsCount].answers[j]);
                }
            }
            questionsCount++;
        }

        // ������������ �������
        shuffleQuestions(questions, questionsCount);

        // ������� ������
        while (questionsCount >= 5) {
            ExamTicket newTicket;
            newTicket.ticketID = ticketsCount + 1;
            cout << "������� ������� ��� ������ " << newTicket.ticketID << ": ";
            cin >> newTicket.subject;
            cin.ignore();

            // ��������� ����� ���������
            for (int i = 0; i < 5; ++i) {
                newTicket.questions[i] = questions[questionsCount - 5 + i];
            }
            tickets[ticketsCount++] = newTicket;
            saveTickets(ticketsFilename, tickets, ticketsCount);
            questionsCount -= 5;
        }

        // ���� �������� �������, ������� �� �� �����
        if (questionsCount > 0) {
            cout << "�������� ���������������� �������:\n";
            for (int i = 0; i < questionsCount; ++i) {
                cout << "\t������ " << i + 1 << ": " << questions[i].questionText << "\n";
            }
        }
        cout << "\n��������������� ������ ������� �������\n";
        system("pause");
        break;
    }
    case 3:
        return;
    }
}

// ������� �������������� ���������������� ������
void editTicket(ExamTicket tickets[], int ticketsCount) {
    int ticketID;
    cout << "������� ����� ������ ��� ��������������: ";
    ticketID = protection(1, 100);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            cout << "������� ����� �������: ";
            cin.ignore();
            getline(cin, tickets[i].subject);

            for (int j = 0; j < 5; ++j) {
                cout << "������� ����� ����� ������� " << j + 1 << ": ";
                getline(cin, tickets[i].questions[j].questionText);

                cout << "������� ����� ��� ������� " << j + 1 << ": ";
                tickets[i].questions[j].questionType = protection(1, 5);

                for (int a = 0; a < 4; ++a) {
                    cout << "������� ����� ����� " << a + 1 << " ��� ������� " << j + 1 << ": ";
                    getline(cin, tickets[i].questions[j].answers[a]);
                }
            }

            Sleep(2000);
            saveTickets(ticketsFilename, tickets, ticketsCount);
            cout << "��������������� ����� ������� ��������������\n";
            return;
        }
    }

    cout << "����� � ��������� ID �� ������\n";
}

// ������� �������� ���������������� ������
void deleteTicket(ExamTicket tickets[], int& ticketsCount) {
    int ticketID;
    cout << "������� ����� ������ ��� ��������: ";
    ticketID=protection(1,100);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            for (int j = i; j < ticketsCount - 1; ++j) {
                tickets[j] = tickets[j + 1];
            }
            --ticketsCount;
            saveTickets(ticketsFilename, tickets, ticketsCount);
            cout << "��������������� ����� ������� ������\n";
            return;
        }
    }
    cout << "����� � ��������� ID �� ������\n";
}

// ������� ����������� ������ ��������������� �������
void showTickets(const ExamTicket tickets[], int ticketsCount) {
        cout << "\n\t������ �������:\n"
            << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
            << "| ����� ������ " << setw(15) << " | " << setw(15) << " ������� |\n"
            << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";

        for (int i = 0; i < ticketsCount; ++i) {
            cout << "| " << setw(15) << left << "�" << tickets[i].ticketID << " | " << setw(15) << left << tickets[i].subject << "|\n";
            for (int j = 0; j < 5; ++j) {
                cout << "| ������ " << j + 1 << ": " << setw(15) << left << tickets[i].questions[j].questionText << ", ��� �������: " << tickets[i].questions[j].questionType << endl;
                if (tickets[i].questions[j].questionType != 3) {
                    cout << "\t�������� �������:\n";
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
    cout << "\t\t�������� �������� ��� ������: \n";
    cout << "\t1. �������\n";
    cout << "\t2. ����� ������\n";
    cout << "\t3. ��� �������\n";
    cin >> choice;
    cin.ignore();

    bool found = false;

    switch (choice) {
    case 1: {
        string subject;
        cout << "������� ������� ��� ������ �������: ";
        getline(cin, subject);
        for (int i = 0; i < ticketsCount; ++i) {
            if (tickets[i].subject == subject) {
                found = true;
                cout << "����� ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << endl;
            }
        }
        break;
    }
    case 2: {
        int ticketID;
        cout << "������� ����� ������ ��� ������: ";
        cin >> ticketID;
        for (int i = 0; i < ticketsCount; ++i) {
            if (tickets[i].ticketID == ticketID) {
                found = true;
                cout << "����� ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << endl;
            }
        }
        break;
    }
    case 3: {
        int questionType;
        cout << "������� ��� ������� ��� ������: ";
        cin >> questionType;
        for (int i = 0; i < ticketsCount; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (tickets[i].questions[j].questionType == questionType) {
                    found = true;
                    cout << "����� ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << ", ��� �������: " << tickets[i].questions[j].questionType << endl;
                }
            }
        }
        break;
    }
    default: {
        cout << "�������� �����, ���������� ��� ���\n";
        return searchTickets(tickets, ticketsCount);
    }
    }

    if (!found) {
        cout << "��������������� ������ �� ��������� �������� �� �������.\n";
    }
    system("pause");
}

void sortTickets(ExamTicket tickets[], int ticketsCount) {
    int choice;
    do {
        system("cls");
        cout << "\n\t\t���� ����������:\n\t1. ���������� �� ������ ������\n\t2. ���������� �� ������ ������ � �������� �������\n\t3. ���������� �� ��������\n\t4. �����\n\t\t>>> ";
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
            cout << "�����\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
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

// ������� ���������� ������� ExamTicket � ����
void saveTickets(const string& filename, ExamTicket tickets[], int ticketsCount) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
        return;
    }

    out << ticketsCount << endl;
    for (int i = 0; i < ticketsCount; ++i) {
        writeExamTicket(out, tickets[i]);
    }

    out.close();
}

// ������� �������� ������� ExamTicket �� �����
void loadTickets(const string& filename, ExamTicket tickets[], int& ticketsCount) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
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
    cout << "������� ����� ������, � ������� ������ ������������� ������: ";
    ticketID = protection(1, 100);
    cout << "������� ����� �������, ������� ������ ������������� (1-5): ";
    questionNumber = protection(1, 5);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            cout << "������� ����� ����� �������: ";
            cin.ignore();
            getline(cin, tickets[i].questions[questionNumber - 1].questionText);

            cout << "������� ����� ��� �������: ";
            tickets[i].questions[questionNumber - 1].questionType = protection(1, 5);

            for (int j = 0; j < 4; ++j) {
                cout << "������� ����� ����� " << j + 1 << ": ";
                getline(cin, tickets[i].questions[questionNumber - 1].answers[j]);
            }

            cout << "������ ������� ��������������.\n";
            return;
        }
    }

    cout << "����� � ��������� ID �� ������\n";
}

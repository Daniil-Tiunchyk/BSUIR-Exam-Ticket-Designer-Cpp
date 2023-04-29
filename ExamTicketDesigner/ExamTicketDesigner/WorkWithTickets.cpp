#include "WorkWithTickets.h"
#include "Protection.h"

// �������, ����������� �������� ������������
void userActions() {
    int choice;
    const int maxTickets = 100;
    int ticketsCount = 0;
    ExamTicket tickets[maxTickets];
    loadTickets(ticketsFilename, tickets, ticketsCount);
 
    do {
        system("cls");
        cout << "\n\t\t����:\n\t1. ������� �����\n\t2. ������������� �����\n\t3. ������� �����\n\t4. �������� ������\n\t5. ����� �������\n\t6. ������������� ������\n\t7. �����\n\t\t>>> ";
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
            cout << "�����\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 7);
}

// ������� �������� ���������������� ������
void createTicket(ExamTicket tickets[], int& ticketsCount) {
    ExamTicket newTicket;
    cout << "������� ����� ������: ";
    newTicket.ticketID = protection(1,100);
    cout << "������� �������: ";
    cin >> newTicket.subject;
    cin.ignore();
    for (int i = 0; i < 5; ++i) {
        cout << "\n������� ����� ������� " << i + 1 << ": ";
        getline(cin, newTicket.questions[i].questionText);
        cout << "������� ��� ������� (1 - ����� ������ ������, 2 - ����� ���������� �������, 3 - ������ � �������� �������): ";
        newTicket.questions[i].questionType = protection(1,3);
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
                cout << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
                    << "| ������ " << j + 1 << ": " << setw(15) << left << tickets[i].questions[j].questionText << ", ��� �������: " << tickets[i].questions[j].questionType << endl;
                if (tickets[i].questions[j].questionType != 3) {
                    cout << "\t�������� �������:\n";
                    for (int k = 0; k < 4; ++k) {
                        cout << "\t\t" << tickets[i].questions[j].answers[k] << "\n";
                    }
                }
            }
            cout << "|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|\n\n";
        }
        cout << "������� ����� �����, ����� ����������\n\t>>> ";
        system("pause");
}


void searchTickets(const ExamTicket tickets[], int ticketsCount) {
    string subject;
    cout << "������� ������� ��� ������ �������: ";
    //cin.ignore();
    getline(cin, subject);
    bool found = false;
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].subject == subject) {
            found = true;
            cout << "����� ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << endl;
        }
    }

    if (!found) {
        cout << "��������������� ������ �� ��������� �������� �� �������.\n";
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
        cout << "����� ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << endl;
        for (int j = 0; j < 5; ++j) {
            cout << "������ " << j + 1 << ": " << tickets[i].questions[j].questionText << ", ��� �������: " << tickets[i].questions[j].questionType << endl;
            if (tickets[i].questions[j].questionType != 3) {
                cout << "�������� �������:\n";
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
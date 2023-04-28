#include "WorkWithTickets.h"
#include "Protection.h"

// �������, ����������� �������� ������������
void userActions() {
    system("cls");
    int choice;
    const int maxTickets = 100;
    int ticketsCount = 0;
    ExamTicket tickets[maxTickets];
    loadTickets(ticketsFilename, tickets, ticketsCount);

    do {
        cout << "����:\n\t1. ������� �����\n\t2. ������������� �����\n\t3. ������� �����\n\t4. �������� ������\n\t5. ����� �������\n\t6. �����\n\t\t>>> ";
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
            cout << "�����\n";
            break;
        default:
            cout << "�������� �����, ���������� ��� ���\n";
        }
    } while (choice != 6);
}

// ������� �������� ���������������� ������
void createTicket(ExamTicket tickets[], int& ticketsCount) {
    ExamTicket newTicket;
    cout << "������� ����� ������: ";
    newTicket.ticketID = protection(1,100);
    cout << "������� �������: ";
    cin.ignore();
    getline(cin, newTicket.subject);

    for (int i = 0; i < 5; ++i) {
        cout << "\n������� ����� ������� " << i + 1 << ": ";
        getline(cin, newTicket.questions[i].questionText);
        cout << "������� ��� ������� (1 - ����� ������ ������, 2 - ����� ���������� �������, 3 - ������ � �������� �������): ";
        newTicket.questions[i].questionType = protection(1,3);
        cin.ignore();
        if (newTicket.questions[i].questionType != 3) {
            for (int j = 0; j < 4; ++j) {
                cout << "������� ������� ������ " << j + 1 << ": ";
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
    ticketID=protection(1,100);
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].ticketID == ticketID) {
            // �������������� ������
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
    for (int i = 0; i < ticketsCount; ++i) {
        cout << "����� ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << endl;
        for (int j = 0; j < 5; ++j) {
            cout << "������ " << j + 1 << ": " << tickets[i].questions[j].questionText << ", ��� �������: " << tickets[i].questions[j].questionType << endl;
            if (tickets[i].questions[j].questionType != 3) {
                cout << "�������� �������:" << endl;
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
    cout << "������� ������� ��� ������ �������: ";
    cin.ignore();
    getline(cin, subject);
    bool found = false;
    for (int i = 0; i < ticketsCount; ++i) {
        if (tickets[i].subject == subject) {
            found = true;
            cout << "ID ������: " << tickets[i].ticketID << ", �������: " << tickets[i].subject << endl;
        }
    }

    if (!found) {
        cout << "��������������� ������ �� ��������� �������� �� �������." << endl;
    }
}
// ������� ���������� ��������������� ������� � ����
void saveTickets(const string& ticketsFilename, ExamTicket tickets[], int ticketsCount) {
    ofstream ticketsFile(ticketsFilename);

    if (!ticketsFile) {
        cerr << "������ �������� ����� � ��������\n";
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

// ������� �������� ��������������� ������� �� �����
void loadTickets(const string& ticketsFilename, ExamTicket tickets[], int& ticketsCount) {
    ifstream ticketsFile(ticketsFilename);
    if (!ticketsFile) {
        cerr << "������ �������� ����� � ��������. �������� ������ �����...\n";
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

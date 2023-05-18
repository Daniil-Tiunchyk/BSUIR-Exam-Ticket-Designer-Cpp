#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <Windows.h>
#include "WorkWithUsers.h"

using namespace std;

const string ticketsFilename = "tickets.txt";

struct Question {
    string questionText;
    int questionType;
    string answers[4];
};

struct ExamTicket {
    int ticketID;
    string subject;
    Question questions[5];
};

void saveTickets(const string& filename, ExamTicket tickets[], int ticketsCount);
void loadTickets(const string& filename, ExamTicket tickets[], int& ticketsCount);
void userActions(User users[], int usersCount, int onlineUser);
void createTicket(ExamTicket tickets[], int& ticketsCount);
void editTicket(ExamTicket tickets[], int ticketsCount);
void deleteTicket(ExamTicket tickets[], int& ticketsCount);
void showTickets(const ExamTicket tickets[], int ticketsCount);
void searchTickets(const ExamTicket tickets[], int ticketsCount);
void sortTickets(ExamTicket tickets[], int ticketsCount);
void editQuestionInTicket(ExamTicket tickets[], int ticketsCount);
void shuffleQuestions(Question questions[], int questionsCount);

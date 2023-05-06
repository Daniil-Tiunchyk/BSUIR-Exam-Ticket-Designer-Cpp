#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <Windows.h>
#include "WorkWithUsers.h"
#include "makeHash.h"
using namespace std;

struct User {
	string username;
	string password;
	int role; // 1 - пользователь, 2 - администратор
};

const string usersFilename = "users.txt";


void loadUsers(User users[], int& usersCount);
void registerUser(User users[], int& usersCount);
int loginUser(User users[], int usersCount, int& onlineUser);
void userActions();
void adminActions(User users[], int usersCount, int onlineUser);
void saveUsers(User users[], int usersCount);
void manageUsers(User users[], int usersCount, int onlineUser);

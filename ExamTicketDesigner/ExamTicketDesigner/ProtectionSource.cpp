#include "Protection.h"

int protection(int a, int b)
{
	int n;
	while (true)
	{
		cin >> n;
		while ((!cin) || (cin.get() != '\n'))
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "\n Пожалуйста, введите численное значение\n\t>>> ";
			cin >> n;
		}
		if (n >= a && n <= b) break; 
		cout << "\n Число должно быть в промежутке от [" << a << " до " << b << "]\n\t>>> ";
	}
	return n; 
}


string inputPassword()
{
	string password;
	int ch = 0;
	while (true)
	{
		ch = _getwch();

		if (ch == 13) 
			break;
		else
			if (ch == 8)
			{
				cout << (char)8 << ' ' << (char)8; 
				if (!password.empty())
					password.erase(password.length() - 1);
			}
			else
			{
				cout << "*";
				password += (char)ch; 
			}
	}
	return password;
}

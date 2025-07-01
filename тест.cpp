#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace std;

void TrimRight(char* s) {
	if (!s || !*s) return;
	char* end = s + strlen(s) - 1;
	char* tmp = nullptr;
	for (end; end >= s; end--) {
		if (*end != ' ') {
			tmp = end;
			break;
		}
	}
	if (tmp) {
		*(tmp + 1) = '\0';
	}
	else {
		*s = '\0';
	}
}

          // Тесты
//вспомогательная функция
bool EqlStr(char* s1, char* s2) {
	if (s1 == nullptr || s2 == nullptr)
		return s1 == s2;
	while (*s1 != '\0' && *s2 != '\0') {
		if (*s1 != *s2)
			return false;
		s1++;
		s2++;
	}
	return *s1 == *s2;
}
//функция вывода строки
void PrintStr(const char* s) {
	if (s == nullptr) {
		cout << "nullptr";
	}
	else {
		cout << "\""<<s<<"\"";
	}
	cout << '\n';
}

void Tests() {
	//Тест 1: строка с пробелами в конце
	try {
		char s[] = "ab   cdab    kpp     ";
		char except[] = "ab   cdab    kpp";
		TrimRight(s);
		if (!EqlStr(s, except)) {
			throw runtime_error("Ожидалось: 'ab   cdab    kpp'");
		}
		cout << "Тест 1:\n";
		PrintStr("ab   cdab    kpp     ");
		PrintStr(s);
		cout << '\n';
	}
	
	catch (const exception& e) {
		cerr << "Test1: " << e.what() << '\n';
	}

	//Тест 2: стргка без пробелов
	try {
		char s[] = "abcd";
		char except[] = "abcd";
		TrimRight(s);
		if (!EqlStr(s, except)) {
			throw runtime_error("Строка не должна меняться");
		}
		cout << "Тест 2:\n";
		PrintStr("abcd");
		PrintStr(s);
		cout << '\n';
	}
	catch (const exception& e) {
		cerr << "Test2: " << e.what() << '\n';
	}

	//Тест3 : все пробелы
	try {
		char s[] = "     ";
		char except[] = "";
		TrimRight(s);
		if (!EqlStr(s, except)) {
			throw runtime_error("Строка должна быть пустой");
		}
		cout << "Тест 3:\n";
		PrintStr("     ");
		PrintStr(s);
		cout << '\n';
	}
	catch (const exception& e) {
		cerr << "Test3: " << e.what() << '\n';
	}

	//Тест4: nullptr
	try {
		TrimRight(nullptr);
		cout << "Тест4: с nullptr корректно\n";
	}
	catch (const exception& e) {
		cerr << "Test4: с nullptr не корректно\n";
	}
}

int main() {
	setlocale(LC_ALL, "ru");
	Tests();
}
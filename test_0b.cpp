#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class UserDataNode {
private:
	string SurName;
	string Name;
	int data;
	UserDataNode* next;
public:
	//Конструктор по умолчанию
	UserDataNode():SurName{},Name{},data{},next{}{}

	//Конструктор по значению
	UserDataNode(string sur_name, string name, int x, UserDataNode* nxt):
		SurName{move(sur_name)}, Name{ move(name) }, data{x}, next{nxt}{}
	UserDataNode(string sur_name, string name,int x):
		SurName{move(sur_name)},Name{move(name)},data{x},next{}{}

	//Правило пяти -> деструктор + явно прописываем методы управления переноса и копирования
	~UserDataNode() = default;

	//Запретим конструктор копирования и присваивания копии
	UserDataNode(const UserDataNode&) = delete;
	UserDataNode& operator=(const UserDataNode&) = delete;

	//Конструктор перемещения
	UserDataNode(UserDataNode&& other) noexcept :
		SurName{ move(other.SurName) }, Name{ move(other.Name)}, data{other.data}, next{other.next} {
		other.data = 0;
		other.next = nullptr;
	}
	//Конструктор перемещения для присваивания
	UserDataNode& operator=(UserDataNode&& other) noexcept {
		if (this == &other) return *this;
		SurName = move(other.SurName);
		Name = move(other.Name);
		data = other.data;
		next = other.next;
		other.data = 0;
		other.next = nullptr;
		return *this;
	}
	//Геттеры, сеттеры
	string getSurName() { return SurName; };
	string getName() { return Name; };
	int getData() { return data; };
	UserDataNode* getNext() { return next; };

	void setNext(UserDataNode* nxt) {
		next = nxt;
	}
};

class UserDataList {
private:
	UserDataNode* head;
	UserDataNode* tail;
	vector<UserDataNode*>getNodePointers();
	void ModifiedList(vector<UserDataNode*>&);
public:
	//Конструктор по умолчанию
	UserDataList() :head{}, tail{} {}

	//Соблюдаем правило пяти для списка
	~UserDataList() {
		UserDataNode* cur = head;
		while (cur) {
			UserDataNode* nxt = cur->getNext();
			delete cur;
			cur = nxt;
		}
		head = nullptr;
	}
	//Пусть список не копируется и не переносится
	UserDataList(UserDataList&) = delete;
	UserDataList& operator=(UserDataList&) = delete;
	UserDataList(UserDataList&& other) = delete;
	UserDataList& operator=(UserDataList&& other) = delete;

	//Основные методы класса
	void additem(string, string, int);
	void readFile(const string&);
	void sortName();
	void sortSurName();
	void sortData();
	void printList(char);
};
//Добавление эл-та в конец 
void UserDataList::additem(string SurName, string Name, int data) {
	UserDataNode* NewNode = new UserDataNode(move(SurName), move(Name), data);
	if (!head) {
		head = NewNode;
		tail = NewNode;
	}
	else {
		tail->setNext(NewNode);
		tail = NewNode;
	}
}
//Формируем массив указателей
vector<UserDataNode*> UserDataList::getNodePointers() {
	vector<UserDataNode*> pointers;
	UserDataNode* cur = head;
	while (cur) {
		pointers.push_back(cur);
		cur = cur->getNext();
	}
	return pointers;
}
//Сортируем по имени
void UserDataList::sortName() {
	auto point = getNodePointers();
	if (point.empty()) {
		head = tail = nullptr;
		return;
	}
	sort(point.begin(),point.end(), [](UserDataNode* a, UserDataNode* b) {
		return a->getName() < b->getName();
		});
	ModifiedList(point);
}
//Сортируем по фамилии
void UserDataList::sortSurName() {
	auto point = getNodePointers();
	sort(point.begin(), point.end(), [](UserDataNode* a, UserDataNode* b) {
		return a->getSurName() < b->getSurName();
		});
	ModifiedList(point);
}
//Сортируем по номеру
void UserDataList::sortData() {
	auto point = getNodePointers();
	sort(point.begin(), point.end(), [](UserDataNode* a, UserDataNode* b) {
		return a->getData() < b->getData();
		});
	ModifiedList(point);
}
//Перестраиваем список после сортировки
void UserDataList::ModifiedList(vector<UserDataNode*>& pointers) {
	if (pointers.empty()) {
		head = nullptr;
		tail = nullptr;
		return;
	}
	head = pointers[0];
	for (size_t i = 0; i < pointers.size() - 1; i++) {
		pointers[i]->setNext(pointers[i + 1]);
	}
	pointers.back()->setNext(nullptr);
	tail = pointers.back();
}
//Выводим результат
void UserDataList::printList(char c) {
	UserDataNode* cur = head;
	while(cur) {
		switch (c) {
		case('1'): cout << cur->getName() << " " << cur->getSurName()
			<< ": " << cur->getData();
			break;
		case('2'): cout << cur->getSurName() << " " << cur->getName()
			<< ": " << cur->getData();
			break;
		case('3'): cout << cur->getData() << ": " << cur->getName()
			<< " " << cur->getSurName();
			break;
		}
		cout << '\n';
		cur = cur->getNext();
	}
}
//Считываем данные с файла
void UserDataList::readFile(const string& filename) {
	ifstream f(filename);
	if (!f.is_open())
		throw runtime_error("Не удалось открыть файл" + filename);
	string line;
	while (getline(f, line)) {
		replace(line.begin(), line.end(), ':', ' ');
		stringstream ss(line);
		string surname, name;
		int data;
		if (ss >> surname >> name >> data)
			additem(surname, name, data);
		else
			cerr << "Неверный формат строки" << line << '\n';
	}
	f.close();
}

int main() {
	setlocale(LC_ALL, "ru");
	UserDataList users;
	users.readFile("InputFile.txt");
	cout << "Для сортировки введите цифры 1 2 или 3\nпо именам - 1\nпо фамилиям - 2\nпо телефонам - 3\n";
	char c;
	cin >> c;
	switch (c) {
	case('1'):users.sortName(); break;
	case('2'):users.sortSurName(); break;
	case('3'):users.sortData(); break;
	}
	users.printList(c);

}
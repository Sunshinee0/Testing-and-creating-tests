#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <vector>
using namespace std;

class EncodeDecode {
public:
	static string Encode(string word) {
		for (auto& letter : word) {
			letter += 5;
		}
		return word;
	}
	static string Decode(string word) {
		for (auto& letter : word) {
			letter -= 5;
		}
		return word;
	}
};

class User {
protected:
	string username;
	string password;
	string FullName;
	string address;
	string phoneNumber;
public:
	User(string username = "", string password = "", string FullName = "", string address = "", string phoneNumber = "") :
		username{ username }, password{ password }, FullName{ FullName }, address{ address }, phoneNumber{ phoneNumber } {}
	virtual void login() = 0;
	virtual void reg() = 0;
};

class Admin : public User {
	bool success;
public:
	Admin(bool success = false) : success{ success } {}

	void reg() {
		cout << "Введите логин: ";
		getline(cin, username);

		cout << "Введите пароль: ";
		getline(cin, password);

		ofstream out("admin.txt");
		if (out.is_open()) {

			out << EncodeDecode::Encode(username) << "," << EncodeDecode::Encode(password) << endl;
			out.close();
		}
		else {
			cout << "Ошибка при открытии файла" << endl;
		}
	}

	void login() {
		do {
			cout << "Логин: ";
			getline(cin, username);
			cout << "Пароль: ";
			getline(cin, password);

			ifstream in("admin.txt");
			string temp;
			while (getline(in, temp)) {
				size_t delimiterPos = 0;
				string token;
				vector<string> userDetails;
				while ((delimiterPos = temp.find(',')) != string::npos) {
					token = temp.substr(0, delimiterPos);
					userDetails.push_back(token);
					temp.erase(0, delimiterPos + 1);
				}
				userDetails.push_back(temp);
				if (userDetails.size() == 2 && userDetails[0] == EncodeDecode::Encode(username) && userDetails[1] == EncodeDecode::Encode(password)) {
					cout << "Успешный вход в систему" << endl;
					success = true;
					break;
				}
			}
			if (!success)
				cout << "Неверный логин или пароль!" << endl << "Попробуйте снова" << endl << endl;
		} while (!success);
	}

	bool isSuccess() const {
		return success;
	}
	void isExit() {
		success = false;
	}

	void result() {
		string testName;
		cout << "Введите название теста, категории или имя пользователя \n(смотря какие результаты хотите посмотреть): ";
		getline(cin, testName);
		system("cls");
		ifstream file("Результаты/" + testName + "_results.txt");
		if (file.is_open()) {
			string temp;
			cout << "Результаты тестa \"" << testName << "\":" << endl << endl;
			while (getline(file, temp)) {
				cout << temp << endl;
			}
			cout << endl;
		}
	}

	void modific() {
		cout << "Введите имя пользователя которого хотите удалить: ";
		string Username;
		getline(cin, Username);

		vector<string> userDetails;
		ifstream in("users.txt");
		string temp;
		while (getline(in, temp)) {
			size_t delimiterPos = 0;
			string token;
			while ((delimiterPos = temp.find(',')) != string::npos) {
				token = temp.substr(0, delimiterPos);
				userDetails.push_back(token);
				temp.erase(0, delimiterPos + 1);
			}
			userDetails.push_back(temp);
			for (int i = 0; i < userDetails.size(); i += 5)
			{
				if (userDetails[i] == EncodeDecode::Encode(Username)) {
					userDetails.erase(userDetails.begin() + i, userDetails.begin() + i + 5);
					break;
				}
			}
		}
		ofstream file1("users.txt", ios::trunc);
		if (file1.is_open()) {
			for (int i = 0; i < userDetails.size(); i++) {
				file1 << userDetails[i];
				if ((i + 1) % 5 != 0)
					file1 << ",";
				else
					file1 << endl;
			}
		}
		file1.close();
	}
};

class Test {
private:
	string section;
	string testName;
	struct Question {
		string questionText;
		string answer;
	};
	vector<Question> questions;
	Admin admin;
public:
	Test(string section = "", string testName = "", Admin admin = false) : section(section), testName(testName), admin{ admin } {}

	void addQuestion(string questionText, string answer) {
		questions.push_back({ questionText, answer });
	}

	void createTest() {
		cout << "Введите название раздела: ";
		getline(cin, section);
		cout << "Введите название теста: ";
		getline(cin, testName);
	}

	void removeQuestion(string questionText) {
		for (auto it = questions.begin(); it != questions.end(); ++it) {
			if (it->questionText == questionText) {
				questions.erase(it);
				cout << "Вопрос \" " << questionText << " \" удален" << endl;
				break;
			}
		}
		cout << endl << "Сохранить изменение в файл? " << endl << "Введите 1 если хотите сохранить данные" << endl;
		int a;
		cin >> a;
		if (a == 1) {
			ofstream file1(section + "/" + testName + "_questions.txt", ios::trunc);
			ofstream file2(section + "/" + testName + "_answers.txt", ios::trunc);
			for (const auto& question : questions) {
				if (file1.is_open() && file2.is_open()) {
					file1 << question.questionText << endl;
					file2 << question.answer << endl;
					cout << "Вопросы и ответы успешно сохранены" << endl;
				}
			}
			file1.close();
			file2.close();
		}
	}

	void saveQuestionsToFile() {
		_mkdir(section.c_str());
		ofstream fileQuestions(section + "/" + testName + "_questions.txt", ios::app);
		ofstream fileAnswers(section + "/" + testName + "_answers.txt", ios::app);
		for (const auto& question : questions) {
			if (fileQuestions.is_open() && fileAnswers.is_open()) {
				fileQuestions << question.questionText << endl;
				fileAnswers << question.answer << endl;
				cout << "Вопросы и ответы успешно сохранены" << endl;
			}
		}
		fileQuestions.close();
		fileAnswers.close();
	}

	void ShowQuestions() {
		cout << "Вопросы и ответы для теста '" << testName << "':" << endl << endl;
		for (const auto& question : questions) {
			cout << "Вопрос: " << question.questionText << endl;
			cout << "Ответ: " << question.answer << endl << endl;
		}
	}

	void importTest(string TestName, string Section) {
		ifstream fileQuestions(Section + "/" + TestName + "_questions.txt");
		ifstream fileAnswers(Section + "/" + TestName + "_answers.txt");
		testName = TestName;
		section = Section;
		string questionText;
		string answerText;
		questions.clear();
		while (getline(fileQuestions, questionText) && getline(fileAnswers, answerText)) {
			questions.push_back({ questionText, answerText });
		}
	}

	string getTestName() const {
		return testName;
	}
	string getSection() const {
		return section;
	}
	const vector<Question>& getQuestions() const {
		return questions;
	}
};


class Guest : public User {
	bool success;
public:
	Guest(bool success = false, string username = "", string password = "", string FullName = "", string address = "", string phoneNumber = "") :
		User(username, password, FullName, address, phoneNumber), success{ success } {}

	bool UsernameUnique(string username) {
		string temp;
		ifstream in("users.txt");
		while (getline(in, temp)) {
			if (temp.find(EncodeDecode::Encode(username) + ",") == 0) {
				cout << "Логин уже используется" << endl;
				return false;
			}
		}
		return true;
	}

	void reg() {
		cout << "Введите ФИО: ";
		getline(cin, FullName);
		cout << "Введите домашний адрес: ";
		getline(cin, address);
		cout << "Введите номер телефона: ";
		getline(cin, phoneNumber);

		do {
			cout << "Введите логин: ";
			getline(cin, username);
		} while (!UsernameUnique(username));

		cout << "Введите пароль: ";
		getline(cin, password);

		ofstream out("users.txt", ios::app);
		if (out.is_open()) {
			out << EncodeDecode::Encode(username) << "," << EncodeDecode::Encode(password) << "," << EncodeDecode::Encode(FullName) << ","
				<< EncodeDecode::Encode(address) << "," << EncodeDecode::Encode(phoneNumber) << endl;
			out.close();
		}
		else {
			cout << "Ошибка при открытии файла" << endl;
		}
	}

	void login() {
		do {
			cout << "Логин: ";
			getline(cin, username);
			cout << "Пароль: ";
			getline(cin, password);

			ifstream in("users.txt");
			string temp;
			while (getline(in, temp)) {
				size_t delimiterPos = 0;
				string token;
				vector<string> userDetails;
				while ((delimiterPos = temp.find(',')) != string::npos) {
					token = temp.substr(0, delimiterPos);
					userDetails.push_back(token);
					temp.erase(0, delimiterPos + 1);
				}
				userDetails.push_back(temp);
				if (userDetails.size() == 5 && userDetails[0] == EncodeDecode::Encode(username) && userDetails[1] == EncodeDecode::Encode(password)) {
					cout << "Успешный вход в систему" << endl;
					success = true;
					break;
				}
			}
			if (!success)
				cout << "Неверный логин или пароль!" << endl << "Попробуйте снова" << endl << endl;
		} while (!success);
	}

	bool isSuccess() const {
		return success;
	}

	void isExit() {
		success = false;
	}

	void takeTest(Test& test) {
		cout << "Прохождение теста: " << endl << endl;

		int score = 0;
		for (const auto& question : test.getQuestions()) {
			cout << "Вопрос: " << question.questionText << endl;
			string userAnswer;
			cout << "Ваш ответ: ";
			getline(cin, userAnswer);
			cout << endl;
			if (userAnswer == question.answer) {
				score++;
			}
		}
		double percentCorrect = ((double)score / test.getQuestions().size()) * 100;
		int grade = percentCorrect / 100 * 12;
		cout << "Ваш результат: " << score << " из " << test.getQuestions().size() << " (" << percentCorrect << "%)" << endl;
		cout << "Ваша оценка: " << (grade != 0 ? grade : 1) << " из 12" << endl;
		string section = test.getSection();
		saveTestResults(section, test.getTestName(), (grade != 0 ? grade : 1));
	}
	void saveTestResults(string section, string testName, int grade) {
		string resul = "Результаты";
		_mkdir(resul.c_str());
		ofstream file(resul + "/" + username + "_results.txt", ios::app);
		if (file.is_open()) {
			file << testName << ": " << grade << " из 12" << endl;
		}
		ofstream file0(resul + "/" + section + "_results.txt", ios::app);

		ofstream file1(resul + "/" + testName + "_results.txt", ios::app);
		if (file1.is_open()) {
			ifstream in("users.txt");
			string temp;
			while (getline(in, temp)) {
				size_t delimiterPos = 0;
				string token;
				vector<string> userDetails;
				while ((delimiterPos = temp.find(',')) != string::npos) {
					token = temp.substr(0, delimiterPos);
					userDetails.push_back(token);
					temp.erase(0, delimiterPos + 1);
				}
				userDetails.push_back(temp);
				if (userDetails.size() == 5 && userDetails[0] == EncodeDecode::Encode(username)) {
					file1 << EncodeDecode::Decode(userDetails[2]) << ": " << grade << " из 12" << endl;
					file0 << testName << ": " << EncodeDecode::Decode(userDetails[2]) << " - " << grade << " из 12" << endl;
				}
			}
		}
	}

	void TestResults() {
		ifstream file("Результаты/" + username + "_results.txt");
		if (file.is_open()) {
			string temp;
			cout << "Результаты тестов для " << username << ":" << endl;
			while (getline(file, temp)) {
				cout << temp << endl;
			}
		}
	}
};

void RegOrLogin(Guest& guest, Admin& admin) {
	system("cls");
	cout << "Выберите аккаунт в который хотите войти: " << endl;
	cout << "1 - Гость" << endl << "2 - Админ" << endl;
	int temp;
	cin >> temp;
	if (temp == 1) {
		cout << endl << "1 - войти в аккаунт" << endl << "2 - зарегестрироваться" << endl;
		int temp1;
		cin >> temp1;
		cin.ignore();
		if (temp1 == 1)
			guest.login();
		else if (temp1 == 2)
			guest.reg();
		else
			cout << "Неверный ввод" << endl;
	}
	if (temp == 2) {
		cout << endl << "1 - войти в аккаунт" << endl << "2 - зарегестрироваться(при повторной регестрации прошлые логин и пароль заменяются)" << endl;
		int temp1;
		cin >> temp1;
		cin.ignore();
		if (temp1 == 1)
			admin.login();
		else if (temp1 == 2)
			admin.reg();
		else
			cout << "Неверный ввод" << endl;
	}
}

void menu() {
	system("cls");
	cout << "1 - войти/зарегестрироваться" << endl;
	cout << "0 - завершить работу" << endl;
}

void menuAdmin() {
	system("cls");
	cout << "(если хотите сохранить тест, не забудьте загрузить его в файл)" << endl;
	cout << "1 - Создать тест" << endl;
	cout << "2 - Добавить вопрос" << endl;
	cout << "3 - Удалить вопрос" << endl;
	cout << "4 - Просмотреть добавленные вопросы" << endl;
	cout << "5 - загрузить вопросы в файл" << endl;
	cout << "6 - импортировать тест" << endl;
	cout << "7 - просмотр результатов" << endl;
	cout << "8 - модификация пользователей" << endl;
	cout << "0 - выйти" << endl;
}

void menuGuest() {
	cout << "1 - пройти тест" << endl;
	cout << "2 - посмотреть результаты" << endl;
	cout << "0 - выйти" << endl;
}

int main()
{
	setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Guest guest;
	Admin admin;
	Test test;
	while (true) {
		menu();
		int x;
		cin >> x;
		cin.ignore();
		if (x == 1) {
			RegOrLogin(guest, admin);
			Sleep(1000);
			system("cls");
			if (admin.isSuccess()) {
				string questText;
				string answer;
				while (true) {
					menuAdmin();
					int temp{};
					cin >> temp;
					cin.ignore();
					if (temp == 1) {
						system("cls");
						test.createTest();
						Sleep(1000);
					}
					else if (temp == 2) {
						system("cls");
						cout << "Введите кол-во вопросов которые хотите добавить: ";
						int a;
						cin >> a;
						cin.ignore();
						for (int i = 0; i < a; i++)
						{
							cout << "Вопрос: ";
							getline(cin, questText);
							cout << "Ответ: ";
							getline(cin, answer);
							test.addQuestion(questText, answer);
						}
						Sleep(1000);
					}
					else if (temp == 3) {
						system("cls");
						cout << "Введите вопрос который хотите удалить: ";
						getline(cin, questText);
						test.removeQuestion(questText);
						Sleep(1000);
					}
					else if (temp == 4) {
						system("cls");
						test.ShowQuestions();
						system("pause");
					}
					else if (temp == 5) {
						system("cls");
						test.saveQuestionsToFile();
						Sleep(1000);
					}
					else if (temp == 6) {
						system("cls");
						string section;
						string testName;
						cout << "Введите название раздела: ";
						getline(cin, section);
						cout << "Введите название теста: ";
						getline(cin, testName);
						test.importTest(testName, section);
					}
					else if (temp == 7) {
						system("cls");
						admin.result();
						system("pause");
					}
					else if (temp == 8) {
						system("cls");
						cout << "1 - зарегестрировать нового пользователя" << endl;
						cout << "2 - удалить пользователя" << endl;
						int a;
						cin >> a;
						cin.ignore();
						Sleep(500);
						if (a == 1) {
							system("cls");
							guest.reg();
							Sleep(1000);
						}
						else if (a == 2) {
							system("cls");
							admin.modific();
							system("pause");
						}
					}
					else if (temp == 0) {
						admin.isExit();
						break;
					}
					else
						cout << "Неверный ввод" << endl;
				}
			}
			if (guest.isSuccess()) {
				while (true) {
					system("cls");
					menuGuest();
					int a;
					cin >> a;
					cin.ignore();
					if (a == 1) {
						system("cls");
						guest.takeTest(test);
						system("pause");
					}
					else if (a == 2) {
						system("cls");
						guest.TestResults();
						system("pause");
					}
					else if (a == 0) {
						guest.isExit();
						break;
					}
					else
						cout << "Неверный ввод" << endl;
				}
			}
		}
		else if (x == 0)
			return 0;
		else
			cout << "Неверный ввод" << endl;
	}
}
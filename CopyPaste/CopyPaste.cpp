// CopyPaste.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Программа копирует искомый файл из всех подпапок указанной папки
// Если файл скопирован из всех подпапок или в созданной директории уже содержатся файлы с задаваемым новым именем, программа ничего не делает

#include "pch.h" // Пока пустой заголовочный файл
#include <iostream> // Консольный ввод-вывод
#include <fstream> // Ввод в файл, вывод из файла
#include <string> // Работа со строками
#include <filesystem> // Работа с файлами, например копирование
#include <windows.h> // Системные команды

using namespace std;
namespace fs = experimental::filesystem;

// Вывести вектор
void v_print(vector<string>& vec)
{
	for (int i = 0; i < vec.size(); i++)
		cout << vec[i] << endl;
}

/* Создать папку, если она не существует. Если какой-нибудь файл с тем же именем находится там же, где
создается папка, к старому имени папки добавляется "folder" неограниченное кол-во раз, пока новое имя будет занято */
void create_dir(string& dir_name)
{
	if (!fs::exists("./" + dir_name))
		fs::create_directory("./" + dir_name);
	else
	{
		if (!fs::is_directory("./" + dir_name))
		{
			dir_name += " folder";
			create_dir(dir_name);
		}
	}
}

int copy_paste(string const& input, string const& file_name)
{
	vector<string> files; // Вектор строк - файлов
	vector<string> dirs; // Папок
	string s; // Вспомогательная переменная, переопределяемая несколько раз
	string name; // Имя файла
	string new_path; // Путь к копируемому файлу
	int num; // Номер файла в виде числа
	size_t found; // Указатель на найденную подстроку
	int flag = 0; // Флаг, свидетельствующий о том, что искомый файл найден
	string dir_name = file_name; // Имя папки, соответствующей имени файла
	string format; // Формат файла

	// Убираем расширение в названии папки
	if (!fs::is_directory(file_name))
	{
		found = file_name.find(".");
		dir_name = file_name.substr(0,found); // Имя папки, соответствующей имени файла, но без его расширения
	}
	else
	{
		cout << file_name << " isn't a file" << endl;
		return 0;
	}

	string path = "./"; // Текущая папка
	
	if (!input.empty()) // Не пустая строка
	{
		if (fs::is_directory(input) && fs::exists(input)) // Если папка существует
		{
			path += input + "/";
			//dir_name += input; // Если необходимо разделять
		}
		else
		{
			cout << endl << "The directory " << input << " doesn't exist" << endl;
			return 0;
		}
	}
	// Если пустая, то считаем, что нужные файлы находятся там же, где эта программа

	cout << "The main directory: " << path << endl;

	// Смотрим, какие папки внутри указанной директории
	for (const auto & p : fs::directory_iterator(path))
	{
		s = p.path().string(); // Путь к объекту, преобразованный в строку

		if (is_directory(p)) // Если папка
		{
			// Выделяем имя папки
			found = s.find_last_of("/\\");
			name = s.substr(found + 1);
			dirs.push_back(name); // Добавить в конец вектора строку - имя папки
		}
	}

	// Из каждой подпапки копируем файл с именем file_name
	for (int i = 0; i < dirs.size(); i++)
	{
		new_path = path + dirs[i] + "/";

		// Ищем в подпапке этот файл
		if (!fs::exists(new_path + file_name))
			cout << "The directory " << new_path << " doesn't have " << file_name << endl;
		else
		{
			cout << "Copying " << file_name << " from the directory " << new_path << endl;

			string new_file_name = input + "_" + dirs[i] + "_" + file_name;

			// Создать папку, соответствующую имени файла, если она не существует
			create_dir(dir_name);

			// Если файла, который копируем, внутри нет, то копируем (иначе вылетит исключение, т.к. перезапись не осуществляется автоматически)
			if (!fs::exists("./" + dir_name + "/" + new_file_name))
				fs::copy(new_path + file_name, "./" + dir_name + "/" + new_file_name);
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	string input;
	int from, to; // Номера (они же названия) начальной и конечной папок, которые нужно обработать
	int answ;
	size_t found; // Указатель на найденную подстроку
	string file_name;

	if (argc == 3)
	{
		input = argv[1];
		file_name = argv[2];
	}
	else
	{
		cout << "Input a directory name (or a range of directories through '-'): ";
		getline(cin, input); // Запрашиваем название папки, из которой необходимо скопировать файлы. getline может принять пустую строку

		cout << "Input a file name (with its format .*): ";
		cin >> file_name;
	}	

	found = input.find("-");
	if (found != -1) // Если найдено тире
	{
		from = stoi(input.substr(0, found));
		to = stoi(input.substr(found + 1));

		for (int i = from; i <= to; i++)
			copy_paste(to_string(i), file_name);
	}
	else
		copy_paste(input, file_name);

	cout << "I've finished all my work" << endl;
	if (argc != 3)
		system("pause");
	return 0;
}
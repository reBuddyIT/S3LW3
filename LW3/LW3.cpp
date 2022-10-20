﻿/***********************************************************************\
*                    кафедра № 304 2 курс 3 семестр информатика			*
*-----------------------------------------------------------------------*
*	Project type : solution												*
*	Project name : matrix												*
*	File name    : matrix.cpp											*
*	Language     : c/c++												*
*	Programmers  : Плоцкий Б.А. Раужев Ю. М.							*
*	Created      :  19/10/22											*
*	Last revision:  /10/22											*
*	Comment(s)   : Вариант № 1											*
* 																		*
*	Для массива из n элементов выполнить сортировку с помощью двух		*
*	указанных методов для:												*
*		1.	заданной произвольным образом последовательности чисел,		*
*		2.	уже отсортированных последовательностей в возрастающем 		*
*		и убывающем порядке (лучший и худший случаи для					*
*		выполнения сортировки).											*
\***********************************************************************/

#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>

using namespace std;

/********************************
*			ДЛЯ КОНСОЛИ			*
********************************/

// убрать комментарий, если нужна
// пошаговая печать массивов
//#define EVERY_STEP_PRINT

// количество таблиц в консоли
#define NUMB_OF_TABLES 10

// заполнение len элементов элементом symb
#define OUT_W(symb, len) fixed << setfill(symb) << setw(len)

/********************************
*			ДЛЯ МАССИВА			*
********************************/
// минимальное и максимальное значение
// для чисел в массиве
#define MIN_VALUE 10
#define MAX_VALUE 2000000

// минимальный и максимальный
// размер массива
#define MIN_ARR_SIZE 1000
#define MAX_ARR_SIZE 20000

/********************************
*	ДЛЯ ПОСЛЕДОВАТЕЛЬНОСТЕЙ		*
********************************/

// шаг чисел при генерации последовательности
#define STEP 1

// максимальное значение при генерации чисел
#define MAX_GENERATE MAX_VALUE

// необходима для функций сортировки
struct help_data
{
	unsigned long num_of_comp;	// число сравнений
	int num_of_swap;	// число перемещений
};

// тип данных NS
typedef chrono::milliseconds TTIME;

/****************************************************************
*              П Р О Т О Т И П Ы   Ф У Н К Ц И Й                *
****************************************************************/

/****************************************************************
*         В С П О М О Г А Т Е Л Ь Н Ы Е  Ф У Н К Ц И Й          *
****************************************************************/

// возвращение случайного значения из 
// интервала (min, max)
template<typename T = int>
T rand_num(T min, T max)
{
	return rand() % (max - min) + min;
}

// копирование массива
template<typename T = int>
void copy_arr(
	T* arr1,	// источник данных
	T* arr2,	// место копирования данных
	int low,	// индекс начального элемента
	int high	// индекс элемента после последнего
)
{
	for (int i = low; i < high; i++)
	{
		arr2[i] = arr1[i];
	}
}

// печать массива в поток
template<typename T = int>
void print_arr(
	T* arr,					// массив
	int low = 0,			// индекс, с которого начинается вывод
	int high = 1,			// индекс, на котром кончается вывод
	ostream& stream = cout	// поток вывода
)
{
	// вывод элементов массива
	for (int i = low; i < high; i++)
	{
		stream << arr[i] << ' ';
	}
	stream << '\n';
}

// ввод и проверка значений
template<typename T = int>
T input_and_check(T _min, T _max,
	const char* welcome_str, const char* err_str)
{
	// размер массива
	T num;

	// вывод сообщения
	cout << welcome_str << "\n";
	cin >> num;

	// если было введено не то
	if (num > _max || num < _min) {
		// если была введена не цифра
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}

		// отчистка консоли
		system("cls");
		cout << err_str << "\n";

		// рекурсивное обращение
		num = input_and_check(_min, _max, welcome_str, err_str);
	}
	return num;
}

/****************************************************************
*	Г Е Н Е Р А Ц И Я   П О С Л Е Д О В А Т Е Л Ь Н О С Т Е Й	*
****************************************************************/

// упорядочная функция по возрастанию
template<typename T = int>
void f1(T* arr, int low, int high)
{
	//k, b - коэффициенты прямой
	T k = MAX_GENERATE / (STEP * (high - low + 1));
	T b = 0;
	double x = 0.0; // координата x

	// заполнение массива 
	for (int i = low; i < high; i++, x += STEP)
	{
		arr[i] = k * x + b;
	}
}

// упорядочная функция по убыванию
template<typename T = int>
void f2(T* arr, int low, int high)
{
	//k, b - коэффициенты прямой
	T k = MAX_GENERATE / (STEP * (high - low + 1));
	T b = 0;
	double x = MAX_GENERATE; // координата x

	// заполнение массива 
	for (int i = low; i < high; i++, x -= STEP)
	{
		arr[i] = k * x + b;
	}
}

// рандомизация значений массива
template<typename T = int>
void randomize_array(T* arr, int low, int high)
{
	for (int i = low; i < high; i++)
	{
		arr[i] = rand_num(MIN_VALUE, MAX_VALUE);
	}
}

// чтение массива
template<typename T = int>
void read_arr(T* arr, int low, int high)
{
	for (int i = low; i < high; i++)
	{
		cin >> arr[i];
	}
}

/****************************************************************
*				C О Р Т И Р О В К А   В Ы Б О Р О М				*
****************************************************************/

template<typename T = int>
void selection_sort(T* arr, int low, int high, help_data& data)
{
#ifdef EVERY_STEP_PRINT
	cout << "\nлевый: " << low << " правый: " << high << " partition: \n";
	cout << "\tдо    изменений: ";
	print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT

	// проходимся по всем элементам массива
	// кроме последнего он уже будет в
	// нужном порядке, когда мы дойдем до него
	for (int i = low; i < high - 1; i++)
	{
		data.num_of_comp++;

		// индекс наименьшего элемента
		int smallestIndex = i;

		// ищем еще более меньший элемент,
		// чем под индексом smallestIndex
		for (int j = i + 1; j < high; j++)
		{
			data.num_of_comp++;

			// если был найдем элемент,
			// который меньше элемнта под
			// индексом smallestIndex, то
			// обновляем smallestIndex
			data.num_of_comp++;
			if (arr[j] < arr[smallestIndex])
			{
				smallestIndex = j;
			}
		}
		data.num_of_comp++;

		// меняем местами текущий элемент и наименьший 
		data.num_of_swap++;
		swap(arr[i], arr[smallestIndex]);

		// если нужна пошаговая печать
#ifdef EVERY_STEP_PRINT
		cout << "\t" << setfill(' ') << setw(3) << i << " с "
			<< setfill(' ') << setw(2) << smallestIndex << " индекс: ";
		print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT
	}
	data.num_of_comp++;
}

/****************************************************************
*				Б Ы С Т Р А Я   С О Р Т И Р О В К А				*
****************************************************************/

// функция перераспределяющая элементы 
template<typename T = int>
int partition(T* arr, int low, int high, help_data& data)
{
#ifdef EVERY_STEP_PRINT
	cout << "\nлевый: " << low << " правый: " << high << " partition: \n";
	cout << "\tдо    изменений: ";
	print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT

	// опорный элемент
	T pivot = arr[high - 1];

	// индекс, в которм в конце алгоритма
	// окажется опорный элемент
	int out_pivot_ind = low;

	for (int i = low; i < high - 1; i++)
	{
		data.num_of_comp++;

		// если текущий элемент меньше опорного
		data.num_of_comp++;
		if (arr[i] <= pivot)
		{
			// меняем местами текущий элемент 
			// и элемент под индексом out_ind
			data.num_of_swap++;
			swap(arr[out_pivot_ind++], arr[i]);
			// если нужна пошаговая печать
#ifdef EVERY_STEP_PRINT
			cout << "\t" << setfill(' ') << setw(3) << i << " с "
				<< setfill(' ') << setw(2) <<
				(out_pivot_ind == 0 ? out_pivot_ind : out_pivot_ind - 1)
				<< " индекс: ";
			print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT
		}
	}
	data.num_of_comp++;

	// ставим опорный элемент в позицию out_pivot_ind
	data.num_of_swap++;
	swap(arr[out_pivot_ind], arr[high - 1]);

#ifdef EVERY_STEP_PRINT
	cout << "\t" << setfill(' ') << setw(3) << high - 1 << " с "
		<< setfill(' ') << setw(2) <<
		(out_pivot_ind == 0 ? out_pivot_ind : out_pivot_ind)
		<< " индекс: ";
	print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT

	// если нужна пошаговая печать
#ifdef EVERY_STEP_PRINT
	cout << "\tпосле  изменен.: ";
	print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT
	return out_pivot_ind;
}

// быстрая сортировка
template<typename T = int>
void quick_sort(T* arr, int low, int high, help_data& data)
{
	/*qsort(arr, high, sizeof(T), [](const void* x1, const void* x2) {
		return (*(int*)x1 - *(int*)x2);
		});*/

	// если минимальный индекс меньше максимального
	data.num_of_comp++;
	if (low < high)
	{
		// переставляем элементы относительно 
		// определенного индекса, возвращаем
		// этот индекс
		int pivot_ind = partition(arr, low, high, data);
	
		// переставляем левую часть массива
		// относительно индекса ключевого элемента
		quick_sort(arr, low, pivot_ind, data);
	
		// переставляем правую часть массива
		// относительно индекса ключевого элемента
		quick_sort(arr, pivot_ind + 1, high, data);
	
		// если нужна пошаговая печать
#ifdef EVERY_STEP_PRINT
		cout << "\nлевый: " << low << " правый: " << high << " quick sort: ";
		print_arr(arr, low, high);
#endif // EVERY_STEP_PRINT
	}
}

/****************************************************************
*					   К О Н С Т А Н Т Ы						*
****************************************************************/

// массив функций генерации последовательности
template<typename T = int>
void(*gen_funcs[])(T* arr, int low, int high) =
{
	f1,
	f2,
	randomize_array,
	read_arr
};

// названия функций генерации
const char gen_f_names[4][12] = {
	"возрастание",
	"убывание",
	"случайная",
	"чтение"
};

// массив функций сортировки
template<typename T = int>
void(*sort_funcs[])(T* arr, int low, int high, help_data& data) =
{
	selection_sort,
	quick_sort
};

// названия функций сортировки
const char sort_f_names[2][8] = {
	"выбором",
	"быстрая"
};

/****************************************************************
*				О С Н О В Н Ы Е   Ф У Н К Ц И И 				*
****************************************************************/

//функция измерения времени работы другой функции
template<typename T>
TTIME measure_time(
	T* arr,
	int size,
	help_data& data,
	void(*sort_funcs)(
		T* arr,
		int low,
		int high,
		help_data& data
		)
)
{
	// начало отсчета времени
	auto begin = chrono::steady_clock::now();

	// вызов функции сортировки
	sort_funcs(arr, 0, size, data);

	// конец отсчета времени
	auto end = chrono::steady_clock::now();

	return chrono::duration_cast<TTIME>(end - begin);
}

// функция позволяет увидеть пошаговую работу
// алгоритма сортировки и выводит число сравнений
// и перестановок
void first_part()
{
	// считывание типа последовательности
	int type_of_gen = input_and_check(1, 3,
		"Выберете способ генерации последовательности:\n\
	1. по возрастанию\n\t2. по убыванию\n\t3. случайная\n",
		"Вводимое значение должно быть 1,2 или 3\n"
	);

	int* arr;		// массив для сортировки
	int* arr_deaf;	// массив с изначальной полседовательностью
	int size = 15;	// размер массивов

	arr = new int[size];	// выделение памяти
	arr_deaf = new int[size];	// выделение памяти

	// генерация последовательности в соответствии с вводом
	gen_funcs<int>[type_of_gen - 1](arr, 0, size);

	// копирование массива. необходимо для вывода
	copy_arr(arr, arr_deaf, 0, size);

	// считывание типа алгоритма сортировки
	int type_of_alg = input_and_check(1, 2,
		"Выберете алгоритм сортировки:\n\
	1. выбором\n\t2. быстрая\n",
		"Вводимое значение должно быть 1 или 2\n"
	);

	// количество сравнений и перестановок
	help_data data = { 0,0 };

	// генерация последовательности в соответствии с вводом
	sort_funcs<int>[type_of_alg - 1](arr, 0, size, data);

	// вывод результатов
	cout << "\n\nНачальный массив: ";
	print_arr(arr_deaf, 0, size);

	cout << "Конечный  массив: ";
	print_arr(arr, 0, size);

	cout << "Число  сравнений: " << data.num_of_comp << '\n';
	cout << "Число  пересылок: " << data.num_of_swap << '\n';

	// очистка памяти
	delete[] arr;
	delete[] arr_deaf;
}

// создание массива с помощю определенной функции
template<typename T>
void generate_arr(T*& arr, int size, int ind_of_gen_func)
{
	// выделение памяти
	arr = new long[size];

	// генерация последовательности
	gen_funcs<long>[ind_of_gen_func](arr, 0, size);
}

// сортирует, выводит массив
template<typename T>
void draw_table(T* arr, int size, int ind_of_gen_func)
{
	// количество сравнений и перестановок
	help_data data = { 0,0 };

	// отрисовка шапки таблицы
	cout << OUT_W('_', 78) << '\n';
	cout << "| ф. генерации | ф. сортировки | размер | время |  сравнения  | перестановки |\n";

	// сортировка и вывод таблицы
	for (int ind_of_sort_func = 0; ind_of_sort_func < 2; ind_of_sort_func++)
	{
		T* arr_copy = new T[size];
		copy_arr(arr, arr_copy, 0, size);

		// измерение времени работы функции
		TTIME elapsed_time = measure_time(arr_copy, size, data, sort_funcs<long>[ind_of_sort_func]);

		// вывод строки таблицы
		cout << "| " << OUT_W(' ', 12) << gen_f_names[ind_of_gen_func]
			<< " | " << OUT_W(' ', 13) << sort_f_names[ind_of_sort_func]
			<< " | " << OUT_W(' ', 6) << size
			<< " | " << OUT_W(' ', 5) << elapsed_time.count()
			<< " | " << OUT_W(' ', 11) << data.num_of_comp
			<< " | " << OUT_W(' ', 12) << data.num_of_swap
			<< " |\n";

		data = { 0,0 };

		delete[] arr_copy;
	}
	cout << OUT_W('-', 78) << '\n';
}

// функция генерирует массивы разными размерами
// и сортирует их двумя алгоритмами сортировки
void second_part()
{
	// считывание генерирующей функции
	int ind_of_gen_func = input_and_check(1, 4,
		"Введите номер генерирующей функции:\n\
	1. по возрастанию\n\t2. по убыванию\n\t3. случайная\n",
		"Значения должны быть 1, 2 или 3\n"
	);
	// индексы должны начинаться с 0
	ind_of_gen_func--;

	// массив для последовательности
	long* arr;

	// 10 раз выполняем генерацию массива и
	// двух сортировок
	for (
		int size = MIN_ARR_SIZE; size <= MAX_ARR_SIZE;
		size += (MAX_ARR_SIZE - MIN_ARR_SIZE) / NUMB_OF_TABLES
		)
	{
		// генерация массива
		generate_arr(arr, size, ind_of_gen_func);

		// печать таблицы
		draw_table(arr, size, ind_of_gen_func);

		// удаление массива
		delete[] arr;
	}
}

/****************************************************************
*                Г Л А В Н А Я   Ф У Н К Ц И Я                  *
****************************************************************/

int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));

	//first_part();

	second_part();

	return 0;
}


/**************** End Of main.cpp File ***************/
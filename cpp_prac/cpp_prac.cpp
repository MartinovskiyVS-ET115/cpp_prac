#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

const int SIZE = 20;
enum gender { m, f };
char gender_name[2][15] = { "male", "female" };

struct birth {
    int time_day;
    int time_month;
    int time_year;
};

struct kid {
    char fam[20];
    char name[20];
    gender pol;
    int high;
    birth date;
};

typedef kid arr_w[SIZE];


void print_kid(const kid& k) {
    cout << k.fam << ' ' << k.name << ' ' << gender_name[k.pol] << ' ' << k.high << " см " << k.date.time_day << "." << k.date.time_month << "." << k.date.time_year << endl;
}

kid* init() {
    kid* kids = new kid[SIZE];
    kids[0] = { "Ivanov", "Ivan", m, 145, {10, 5, 2013} };
    kids[1] = { "Petrova", "Anna", f, 152, {15, 8, 2013} };
    kids[2] = { "Sidorov", "Pavel", m, 138, {20, 12, 2014} };
    kids[3] = { "Smirnova", "Elena", f, 148, {3, 4, 2013} };
    kids[4] = { "Kozlov", "Dmitry", m, 160, {7, 11, 2011} };
    kids[5] = { "Volkova", "Olga", f, 155, {25, 1, 2012} };
    kids[6] = { "Novikov", "Sergey", m, 142, {30, 6, 2015} };
    kids[7] = { "Morozova", "Irina", f, 150, {12, 9, 2013} };
    kids[8] = { "Orlov", "Alexey", m, 157, {18, 3, 2011} };
    kids[9] = { "Sokolova", "Tatyana", f, 153, {5, 7, 2013} };
    kids[10] = { "Popov", "Andrey", m, 143, {9, 5, 2015} };
    kids[11] = { "Lebedeva", "Natalia", f, 146, {22, 2, 2014} };
    kids[12] = { "Kuznetsov", "Mikhail", m, 162, {11, 8, 2010} };
    kids[13] = { "Vasileva", "Svetlana", f, 149, {14, 11, 2013} };
    kids[14] = { "Pavlov", "Vladimir", m, 155, {1, 1, 2016} };
    kids[15] = { "Semenova", "Maria", f, 151, {27, 4, 2012} };
    kids[16] = { "Grigorev", "Evgeny", m, 140, {19, 10, 2017} };
    kids[17] = { "Bogdanova", "Daria", f, 147, {8, 6, 2014} };
    kids[18] = { "Zaytsev", "Nikolay", m, 163, {4, 12, 2010} };
    kids[19] = { "Fedorova", "Galina", f, 144, {16, 9, 2016} };

    return kids;
}

bool write_kids_to_binary(const kid* kids, int size, const string& filename) {
    ofstream fout(filename, ios::binary);
    fout.write(reinterpret_cast<const char*>(kids), size * sizeof(kid));
    fout.close();
    return true;
}

kid* read_kids_from_binary(int& size, const string& filename) {
    ifstream fin(filename, ios::binary | ios::ate);
    streampos file_size = fin.tellg();
    size = file_size / sizeof(kid);
    kid* kids = new kid[size];
    fin.seekg(0, ios::beg);
    fin.read(reinterpret_cast<char*>(kids), file_size);
    fin.close();
    return kids;
}

double avg_h(kid* kids, int size, int current_year) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (kids[i].pol == f) {
            int age = current_year - kids[i].date.time_year;
            if (kids[i].date.time_month > 4 || (kids[i].date.time_month == 4 && kids[i].date.time_day > 16))
                age--;
            if (age == 12) {
                sum += kids[i].high;
                count++;
            }
        }
    }
    return (count > 0) ? (sum / count) : 0.0;
}

void tri_y_boys(kid* kids, int size, kid* result) {
    kid boys[SIZE];
    int boy_count = 0;
    for (int i = 0; i < size; ++i) {
        if (kids[i].pol == m) {
            boys[boy_count++] = kids[i];
        }
    }

    sort(boys, boys + boy_count, [](const kid& a, const kid& b) {
        if (a.date.time_year != b.date.time_year)
            return a.date.time_year > b.date.time_year;
        if (a.date.time_month != b.date.time_month)
            return a.date.time_month > b.date.time_month;
        return a.date.time_day > b.date.time_day;
        });

    int count = min(3, boy_count);
    for (int i = 0; i < count; ++i) {
        result[i] = boys[i];
    }
}

void sort_by_birth(kid* kids, int size) {
    sort(kids, kids + size, [](const kid& a, const kid& b) {
        if (a.date.time_year != b.date.time_year)
            return a.date.time_year < b.date.time_year;
        if (a.date.time_month != b.date.time_month)
            return a.date.time_month < b.date.time_month;
        return a.date.time_day < b.date.time_day;
        });
}

bool by_fam(kid* kids, int size, const char* fam) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(kids[i].fam, fam) == 0) {
            print_kid(kids[i]);
            return true;
        }
    }
    cout << "Ученик с фамилией " << fam << " не найден." << endl;
    return false;
}

bool modify_kid_by_fam(kid* kids, int size, const char* fam) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(kids[i].fam, fam) == 0) {
            cout << "Введите новые данные для " << fam << ":\n";
            cout << "Имя: "; cin >> kids[i].name;
            cout << "Пол (0 - male, 1 - female): "; int g; cin >> g; kids[i].pol = (g == 0 ? m : f);
            cout << "Рост (см): "; cin >> kids[i].high;
            cout << "День рождения: "; cin >> kids[i].date.time_day;
            cout << "Месяц рождения: "; cin >> kids[i].date.time_month;
            cout << "Год рождения: "; cin >> kids[i].date.time_year;
            return true;
        }
    }
    cout << "Ученик с фамилией " << fam << " не найден." << endl;
    return false;
}

void filter_by_month(kid* kids, int size, int month, kid* filtered, int& count) {
    count = 0;
    for (int i = 0; i < size; ++i) {
        if (kids[i].date.time_month == month) {
            filtered[count++] = kids[i];
        }
    }
}

void print_filtered(kid* filtered, int count) {
    if (count == 0) {
        cout << "Нет детей, родившихся в указанном месяце." << endl;
        return;
    }
    cout << "Дети, родившиеся в заданном месяце (" << count << "):" << endl;
    for (int i = 0; i < count; ++i) {
        print_kid(filtered[i]);
    }
}

int main() {
    setlocale(LC_ALL, "Ru");
    kid* kids = init();
    const int CURRENT_YEAR = 2026;

    double avg = avg_h(kids, SIZE, CURRENT_YEAR);
    cout << "Средний рост 12-летних девочек: " << avg << " см" << endl << endl;

    kid youngest_boys[3];
    tri_y_boys(kids, SIZE, youngest_boys);
    cout << "Три самых младших мальчика:" << endl;
    for (int i = 0; i < 3; ++i) {
        print_kid(youngest_boys[i]);
    }
    cout << endl;

    sort_by_birth(kids, SIZE);
    cout << "Список детей, отсортированный по дате рождения (от старших к младшим):" << endl;
    for (int i = 0; i < SIZE; ++i) {
        print_kid(kids[i]);
    }
    cout << endl;

    cout << "Поиск ученика по фамилии Smirnova:" << endl;
    by_fam(kids, SIZE, "Smirnova");
    cout << endl;

    int month = 4;
    kid filtered[SIZE];
    int filtered_count = 0;
    filter_by_month(kids, SIZE, month, filtered, filtered_count);
    cout << "Фильтрация по месяцу рождения (апрель):" << endl;
    print_filtered(filtered, filtered_count);

    cout << endl;
    cout << "Измененные данные:" << endl;

    ifstream fin;
    string str, pl[SIZE];
    int i = 0;
    fin.open("pol_test.txt");
    if (fin.is_open()) {
        while (!fin.eof()) {
            fin >> str;
            pl[i] = str;
            i++;
        }
    }
    fin.close();

    for (int i = 0; i < SIZE; i++) {
        kids[i].pol = (pl[i] == "0") ? m : f;
    }

    for (int i = 0; i < SIZE; ++i) {
        print_kid(kids[i]);
    }

    cout << endl;

    ofstream fout;
    fout.open("pol_test_changed.txt");
    for (int i = 0; i < SIZE; i++) {
        fout << kids[i].fam << ' ' << kids[i].name << ' ' << gender_name[kids[i].pol] << ' ' << kids[i].high << " см " << kids[i].date.time_day << "." << kids[i].date.time_month << "." << kids[i].date.time_year << endl;
    }
    fout.close();

    const string binary_file = "pol_test_changed.bin";
    if (write_kids_to_binary(kids, SIZE, binary_file)) {
        int read_size = 0;
        kid* kids_from_file = read_kids_from_binary(read_size, binary_file);
        if (kids_from_file != nullptr) {
            cout << "Чтение из бинарного файла:" << endl;
            for (int i = 0; i < SIZE; ++i) {
                print_kid(kids_from_file[i]);
            }
            delete[] kids_from_file;
        }
    }

    delete[] kids;

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Максимальна довжина рядків
#define MAX_STR 100
// Початкова місткість масиву
#define INITIAL_CAPACITY 100

// Структура для зберігання даних студента
typedef struct {
    int id;
    char fullName[MAX_STR];
    char birthDate[MAX_STR];
    char enrollmentDate[MAX_STR];
    char major[MAX_STR];
    float gpa;
} Student;

// Структура для бази даних студентів
typedef struct {
    Student* students;
    int count;
    int capacity;
} StudentDatabase;

// Прототипи функцій
void initDatabase(StudentDatabase* db);
bool isIdUnique(StudentDatabase* db, int id);
bool validateDate(const char* date);
bool validateGPA(float gpa);
void trimNewline(char* str);
void addStudent(StudentDatabase* db, Student student);
void updateStudent(StudentDatabase* db, int id, Student newData);
void deleteStudent(StudentDatabase* db, int id);
Student* findStudentById(StudentDatabase* db, int id);
void listAllStudents(StudentDatabase* db);
void searchBySurname(StudentDatabase* db, const char* surname);
void searchByMajor(StudentDatabase* db, const char* major);
void generateReport(StudentDatabase* db, float threshold);
void saveDatabase(StudentDatabase* db);
void loadDatabase(StudentDatabase* db);
bool displayAuthScreen(void);
int displayMainMenu(void);
Student inputStudent(void);

// Ініціалізація бази даних
void initDatabase(StudentDatabase* db) {
    db->capacity = INITIAL_CAPACITY;
    db->count = 0;
    db->students = (Student*)malloc(db->capacity * sizeof(Student));
    if (!db->students) {
        printf("Помилка виділення пам'яті!\n");
        exit(1);
    }
}

// Перевірка унікальності ID
bool isIdUnique(StudentDatabase* db, int id) {
    if (id <= 0) return false; // ID має бути додатним
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].id == id) return false;
    }
    return true;
}

// Валідація формату дати (ДД.ММ.РРРР)
bool validateDate(const char* date) {
    if (strlen(date) != 10) return false;
    if (date[2] != '.' || date[5] != '.') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    int day = atoi(date);
    int month = atoi(date + 3);
    int year = atoi(date + 6);
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 1900 || year > 2025) return false;
    return true;
}

// Валідація середнього балу (0.0–5.0)
bool validateGPA(float gpa) {
    return gpa >= 0.0 && gpa <= 5.0;
}

// Видалення символу нового рядка
void trimNewline(char* str) {
    str[strcspn(str, "\n")] = 0;
}

// Додавання студента
void addStudent(StudentDatabase* db, Student student) {
    if (!isIdUnique(db, student.id)) {
        printf("Помилка: ID %d уже існує або недійсний!\n", student.id);
        return;
    }
    if (db->count >= db->capacity) {
        db->capacity *= 2;
        Student* newStudents = (Student*)realloc(db->students, db->capacity * sizeof(Student));
        if (!newStudents) {
            printf("Помилка виділення пам’яті!\n");
            exit(1);
        }
        db->students = newStudents;
    }
    db->students[db->count++] = student;
    printf("Студента додано успішно!\n");
}

// Оновлення даних студента
void updateStudent(StudentDatabase* db, int id, Student newData) {
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].id == id) {
            if (newData.id != id && !isIdUnique(db, newData.id)) {
                printf("Помилка: Новий ID %d уже існує!\n", newData.id);
                return;
            }
            db->students[i] = newData;
            printf("Дані оновлено успішно!\n");
            return;
        }
    }
    printf("Студента з ID %d не знайдено!\n", id);
}

// Видалення студента
void deleteStudent(StudentDatabase* db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].id == id) {
            for (int j = i; j < db->count - 1; j++) {
                db->students[j] = db->students[j + 1];
            }
            db->count--;
            printf("Студента видалено успішно!\n");
            return;
        }
    }
    printf("Студента з ID %d не знайдено!\n", id);
}

// Пошук студента за ID
Student* findStudentById(StudentDatabase* db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].id == id) return &db->students[i];
    }
    return NULL;
}

// Виведення всіх студентів
void listAllStudents(StudentDatabase* db) {
    printf("+----+-------------------+---------------+-------+\n");
    printf("| ID | Прізвище та ім’я  | Спеціальність | Бал   |\n");
    printf("+----+-------------------+---------------+-------+\n");
    for (int i = 0; i < db->count; i++) {
        printf("| %-2d | %-17s | %-13s | %-5.1f |\n",
               db->students[i].id, db->students[i].fullName,
               db->students[i].major, db->students[i].gpa);
    }
    printf("+----+-------------------+---------------+-------+\n");
    if (db->count == 0) printf("База даних порожня.\n");
}

// Пошук за прізвищем
void searchBySurname(StudentDatabase* db, const char* surname) {
    bool found = false;
    printf("+----+-------------------+---------------+-------+\n");
    printf("| ID | Прізвище та ім’я  | Спеціальність | Бал   |\n");
    printf("+----+-------------------+---------------+-------+\n");
    for (int i = 0; i < db->count; i++) {
        if (strstr(db->students[i].fullName, surname)) {
            printf("| %-2d | %-17s | %-13s | %-5.1f |\n",
                   db->students[i].id, db->students[i].fullName,
                   db->students[i].major, db->students[i].gpa);
            found = true;
        }
    }
    printf("+----+-------------------+---------------+-------+\n");
    if (!found) printf("Студентів із прізвищем %s не знайдено!\n", surname);
}

// Пошук за спеціальністю
void searchByMajor(StudentDatabase* db, const char* major) {
    bool found = false;
    printf("+----+-------------------+---------------+-------+\n");
    printf("| ID | Прізвище та ім’я  | Спеціальність | Бал   |\n");
    printf("+----+-------------------+---------------+-------+\n");
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->students[i].major, major) == 0) {
            printf("| %-2d | %-17s | %-13s | %-5.1f |\n",
                   db->students[i].id, db->students[i].fullName,
                   db->students[i].major, db->students[i].gpa);
            found = true;
        }
    }
    printf("+----+-------------------+---------------+-------+\n");
    if (!found) printf("Студентів зі спеціальністю %s не знайдено!\n", major);
}

// Генерація звіту
void generateReport(StudentDatabase* db, float threshold) {
    if (!validateGPA(threshold)) {
        printf("Помилка: Недійсний пороговий бал (0.0–5.0)!\n");
        return;
    }
    FILE* reportFile = fopen("report.txt", "w");
    if (!reportFile) {
        printf("Помилка створення файлу звіту!\n");
        return;
    }
    fprintf(reportFile, "+----+-------------------+---------------+-------+\n");
    fprintf(reportFile, "| ID | Прізвище та ім’я  | Спеціальність | Бал   |\n");
    fprintf(reportFile, "+----+-------------------+---------------+-------+\n");
    bool found = false;
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].gpa >= threshold) {
            fprintf(reportFile, "| %-2d | %-17s | %-13s | %-5.1f |\n",
                    db->students[i].id, db->students[i].fullName,
                    db->students[i].major, db->students[i].gpa);
            found = true;
        }
    }
    fprintf(reportFile, "+----+-------------------+---------------+-------+\n");
    fclose(reportFile);
    if (found) {
        printf("Звіт створено у файлі report.txt\n");
    } else {
        printf("Студентів із балом >= %.1f не знайдено.\n", threshold);
    }
}

// Збереження бази у файл
void saveDatabase(StudentDatabase* db) {
    FILE* file = fopen("students.txt", "w");
    if (!file) {
        printf("Помилка відкриття файлу для запису!\n");
        return;
    }
    for (int i = 0; i < db->count; i++) {
        fprintf(file, "%d,%s,%s,%s,%s,%.1f\n",
                db->students[i].id, db->students[i].fullName,
                db->students[i].birthDate, db->students[i].enrollmentDate,
                db->students[i].major, db->students[i].gpa);
    }
    fclose(file);
    printf("Дані збережено у файлі students.txt\n");
}

// Завантаження бази з файлу
void loadDatabase(StudentDatabase* db) {
    FILE* file = fopen("students.txt", "r");
    if (!file) {
        printf("Файл students.txt не знайдено. Створюється нова база.\n");
        return;
    }
    Student student;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%f",
                   &student.id, student.fullName, student.birthDate,
                   student.enrollmentDate, student.major, &student.gpa) == 6) {
            trimNewline(student.fullName);
            trimNewline(student.birthDate);
            trimNewline(student.enrollmentDate);
            trimNewline(student.major);
            if (isIdUnique(db, student.id) && validateDate(student.birthDate) &&
                validateDate(student.enrollmentDate) && validateGPA(student.gpa)) {
                addStudent(db, student);
            }
        }
    }
    fclose(file);
}

// Екран аутентифікації
bool displayAuthScreen(void) {
    char username[MAX_STR], password[MAX_STR];
    int attempts = 3;
    while (attempts > 0) {
        printf("======================================\n");
        printf("            ВХІД У СИСТЕМУ\n");
        printf("======================================\n");
        printf("Введіть ім’я користувача: ");
        if (scanf("%99s", username) != 1) {
            printf("Помилка введення!\n");
            while (getchar() != '\n'); // Очистка буфера
            continue;
        }
        printf("Введіть пароль: ");
        if (scanf("%99s", password) != 1) {
            printf("Помилка введення!\n");
            while (getchar() != '\n');
            continue;
        }
        printf("Спроб залишилось: %d\n", --attempts);
        printf("======================================\n");
        if (strcmp(username, "admin") == 0 && strcmp(password, "password123") == 0) {
            while (getchar() != '\n'); // Очистка буфера
            return true;
        }
        printf("Неправильний логін або пароль!\n");
    }
    return false;
}

// Головне меню
int displayMainMenu(void) {
    int choice;
    do {
        printf("======================================\n");
        printf(" СИСТЕМА УПРАВЛІННЯ СТУДЕНТАМИ\n");
        printf("======================================\n");
        printf("1. Додати студента\n2. Оновити дані\n3. Видалити студента\n");
        printf("4. Переглянути студента\n5. Показати всіх студентів\n");
        printf("6. Пошук студентів\n7. Звітність\n8. Вихід\n");
        printf("======================================\n");
        printf("Виберіть опцію (1-8): ");
        if (scanf("%d", &choice) != 1) {
            printf("Невірний формат. Введіть число від 1 до 8.\n");
            while (getchar() != '\n'); // Очистка буфера
            choice = 0;
        } else {
            while (getchar() != '\n'); // Очистка буфера
        }
    } while (choice < 1 || choice > 8);
    return choice;
}

// Введення даних студента
Student inputStudent(void) {
    Student student;
    printf("======================================\n");
    printf("          ДОДАВАННЯ СТУДЕНТА\n");
    printf("======================================\n");
    do {
        printf("Введіть ID (ціле додатне число): ");
        if (scanf("%d", &student.id) != 1 || student.id <= 0) {
            printf("Недійсний ID. Введіть додатне число.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (true);
    while (getchar() != '\n'); // Очистка буфера
    do {
        printf("Введіть прізвище та ім’я: ");
        if (fgets(student.fullName, MAX_STR, stdin) == NULL) {
            printf("Помилка введення!\n");
            continue;
        }
        trimNewline(student.fullName);
        if (strlen(student.fullName) == 0) {
            printf("Поле не може бути порожнім!\n");
        } else {
            break;
        }
    } while (true);
    do {
        printf("Введіть дату народження (ДД.ММ.РРРР): ");
        if (fgets(student.birthDate, MAX_STR, stdin) == NULL) {
            printf("Помилка введення!\n");
            continue;
        }
        trimNewline(student.birthDate);
        if (!validateDate(student.birthDate)) {
            printf("Недійсний формат дати (ДД.ММ.РРРР)!\n");
        } else {
            break;
        }
    } while (true);
    do {
        printf("Введіть дату зарахування (ДД.ММ.РРРР): ");
        if (fgets(student.enrollmentDate, MAX_STR, stdin) == NULL) {
            printf("Помилка введення!\n");
            continue;
        }
        trimNewline(student.enrollmentDate);
        if (!validateDate(student.enrollmentDate)) {
            printf("Недійсний формат дати (ДД.ММ.РРРР)!\n");
        } else {
            break;
        }
    } while (true);
    do {
        printf("Введіть спеціальність: ");
        if (fgets(student.major, MAX_STR, stdin) == NULL) {
            printf("Помилка введення!\n");
            continue;
        }
        trimNewline(student.major);
        if (strlen(student.major) == 0) {
            printf("Поле не може бути порожнім!\n");
        } else {
            break;
        }
    } while (true);
    do {
        printf("Введіть середній бал (0.0–5.0): ");
        if (scanf("%f", &student.gpa) != 1) {
            printf("Недійсний формат балу!\n");
            while (getchar() != '\n');
        } else if (!validateGPA(student.gpa)) {
            printf("Бал має бути від 0.0 до 5.0!\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    } while (true);
    while (getchar() != '\n'); // Очистка буфера
    return student;
}

// Основна функція
int main(void) {
    StudentDatabase db;
    initDatabase(&db);
    loadDatabase(&db);

    if (!displayAuthScreen()) {
        printf("Вичерпано спроби входу. Програма завершена.\n");
        free(db.students);
        return 1;
    }

    while (true) {
        int choice = displayMainMenu();
        if (choice == 8) break;

        switch (choice) {
            case 1: {
                Student student = inputStudent();
                addStudent(&db, student);
                saveDatabase(&db);
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            }
            case 2: {
                int id;
                printf("Введіть ID студента: ");
                if (scanf("%d", &id) != 1 || id <= 0) {
                    printf("Недійсний ID!\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    Student newData = inputStudent();
                    updateStudent(&db, id, newData);
                    saveDatabase(&db);
                }
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            }
            case 3: {
                int id;
                printf("Введіть ID студента: ");
                if (scanf("%d", &id) != 1 || id <= 0) {
                    printf("Недійсний ID!\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    char confirm;
                    printf("Підтвердити видалення? (y/n): "); // Змінено з (т/н) на (y/n)
                    confirm = getchar();
                    while (getchar() != '\n');
                    if (confirm == 'y' || confirm == 'Y') { // Змінено з 'т'/'Т' на 'y'/'Y'
                        deleteStudent(&db, id);
                        saveDatabase(&db);
                    }
                }
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            }
            case 4: {
                int id;
                printf("Введіть ID студента: ");
                if (scanf("%d", &id) != 1 || id <= 0) {
                    printf("Недійсний ID!\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    Student* student = findStudentById(&db, id);
                    if (student) {
                        printf("ID: %d\nПрізвище та ім’я: %s\nДата народження: %s\n",
                               student->id, student->fullName, student->birthDate);
                        printf("Дата зарахування: %s\nСпеціальність: %s\nБал: %.1f\n",
                               student->enrollmentDate, student->major, student->gpa);
                    } else {
                        printf("Студента з ID %d не знайдено!\n", id);
                    }
                }
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            }
            case 5:
                listAllStudents(&db);
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            case 6: {
                int searchType;
                printf("Виберіть тип пошуку (1-ID, 2-Прізвище, 3-Спеціальність): ");
                if (scanf("%d", &searchType) != 1 || searchType < 1 || searchType > 3) {
                    printf("Невірний тип пошуку!\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    if (searchType == 1) {
                        int id;
                        printf("Введіть ID: ");
                        if (scanf("%d", &id) != 1 || id <= 0) {
                            printf("Недійсний ID!\n");
                            while (getchar() != '\n');
                        } else {
                            while (getchar() != '\n');
                            Student* student = findStudentById(&db, id);
                            if (student) {
                                printf("+----+-------------------+---------------+-------+\n");
                                printf("| ID | Прізвище та ім’я  | Спеціальність | Бал   |\n");
                                printf("+----+-------------------+---------------+-------+\n");
                                printf("| %-2d | %-17s | %-13s | %-5.1f |\n",
                                       student->id, student->fullName, student->major, student->gpa);
                                printf("+----+-------------------+---------------+-------+\n");
                            } else {
                                printf("Студента з ID %d не знайдено!\n", id);
                            }
                        }
                    } else if (searchType == 2) {
                        char surname[MAX_STR];
                        printf("Введіть прізвище: ");
                        if (fgets(surname, MAX_STR, stdin) == NULL) {
                            printf("Помилка введення!\n");
                        } else {
                            trimNewline(surname);
                            searchBySurname(&db, surname);
                        }
                    } else {
                        char major[MAX_STR];
                        printf("Введіть спеціальність: ");
                        if (fgets(major, MAX_STR, stdin) == NULL) {
                            printf("Помилка введення!\n");
                        } else {
                            trimNewline(major);
                            searchByMajor(&db, major);
                        }
                    }
                }
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            }
            case 7: {
                float threshold;
                printf("Введіть пороговий бал (0.0–5.0): ");
                if (scanf("%f", &threshold) != 1) {
                    printf("Недійсний формат балу!\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    generateReport(&db, threshold);
                }
                printf("Натисніть Enter для продовження...");
                getchar();
                break;
            }
        }
    }

    saveDatabase(&db);
    free(db.students);
    printf("Дані збережено. Програма завершена.\n");
    return 0;
}

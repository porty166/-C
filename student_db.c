#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Максимальна довжина рядків
#define MAX_STR 255
// Початкова місткість масиву
#define INITIAL_CAPACITY 100

// Структура для зберігання даних студента
typedef struct Student {
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

// Ініціалізація бази даних
void initDatabase(StudentDatabase* db) {
    db->capacity = INITIAL_CAPACITY;
    db->count = 0;
    db->students = (Student*)malloc(db->capacity * sizeof(Student));
    if (!db->students) {
        printf("Помилка виділення пам’яті!\n");
        exit(1);
    }
}

// Перевірка унікальності ID
bool isIdUnique(StudentDatabase* db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].id == id) return false;
    }
    return true;
}

// Додавання студента
void addStudent(StudentDatabase* db, Student student) {
    if (!isIdUnique(db, student.id)) {
        printf("Помилка: ID %d уже існує!\n", student.id);
        return;
    }
    if (db->count >= db->capacity) {
        db->capacity *= 2;
        db->students = (Student*)realloc(db->students, db->capacity * sizeof(Student));
        if (!db->students) {
            printf("Помилка виділення пам’яті!\n");
            exit(1);
        }
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
    FILE* reportFile = fopen("report.txt", "w");
    if (!reportFile) {
        printf("Помилка створення файлу звіту!\n");
        return;
    }
    fprintf(reportFile, "+----+-------------------+---------------+-------+\n");
    fprintf(reportFile, "| ID | Прізвище та ім’я  | Спеціальність | Бал   |\n");
    fprintf(reportFile, "+----+-------------------+---------------+-------+\n");
    for (int i = 0; i < db->count; i++) {
        if (db->students[i].gpa >= threshold) {
            fprintf(reportFile, "| %-2d | %-17s | %-13s | %-5.1f |\n",
                    db->students[i].id, db->students[i].fullName,
                    db->students[i].major, db->students[i].gpa);
        }
    }
    fprintf(reportFile, "+----+-------------------+---------------+-------+\n");
    fclose(reportFile);
    printf("Звіт створено у файлі report.txt\n");
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
}

// Завантаження бази з файлу
void loadDatabase(StudentDatabase* db) {
    FILE* file = fopen("students.txt", "r");
    if (!file) return; // Файл може бути відсутнім при першому запуску
    Student student;
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%f\n",
                  &student.id, student.fullName, student.birthDate,
                  student.enrollmentDate, student.major, &student.gpa) == 6) {
        addStudent(db, student);
    }
    fclose(file);
}

// Екран аутентифікації
bool displayAuthScreen() {
    char username[MAX_STR], password[MAX_STR];
    int attempts = 3;
    while (attempts > 0) {
        printf("======================================\n");
        printf("            ВХІД У СИСТЕМУ\n");
        printf("======================================\n");
        printf("Введіть ім’я користувача: ");
        scanf("%s", username);
        printf("Введіть пароль: ");
        scanf("%s", password);
        printf("Спроб залишилось: %d\n", --attempts);
        printf("======================================\n");
        if (strcmp(username, "admin") == 0 && strcmp(password, "password123") == 0) {
            return true;
        }
        printf("Неправильний логін або пароль!\n");
    }
    return false;
}

// Головне меню
int displayMainMenu() {
    int choice;
    printf("======================================\n");
    printf(" СИСТЕМА УПРАВЛІННЯ СТУДЕНТАМИ\n");
    printf("======================================\n");
    printf("1. Додати студента\n2. Оновити дані\n3. Видалити студента\n");
    printf("4. Переглянути студента\n5. Показати всіх студентів\n");
    printf("6. Пошук студентів\n7. Звітність\n8. Вихід\n");
    printf("======================================\n");
    printf("Виберіть опцію (1-8): ");
    scanf("%d", &choice);
    return choice;
}

// Введення даних студента
Student inputStudent() {
    Student student;
    printf("======================================\n");
    printf("          ДОДАВАННЯ СТУДЕНТА\n");
    printf("======================================\n");
    printf("Введіть ID (ціле додатне число): ");
    scanf("%d", &student.id);
    getchar(); // Очистка буфера
    printf("Введіть прізвище та ім’я: ");
    fgets(student.fullName, MAX_STR, stdin);
    student.fullName[strcspn(student.fullName, "\n")] = 0;
    printf("Введіть дату народження (ДД.ММ.РРРР): ");
    fgets(student.birthDate, MAX_STR, stdin);
    student.birthDate[strcspn(student.birthDate, "\n")] = 0;
    printf("Введіть дату зарахування (ДД.ММ.РРРР): ");
    fgets(student.enrollmentDate, MAX_STR, stdin);
    student.enrollmentDate[strcspn(student.enrollmentDate, "\n")] = 0;
    printf("Введіть спеціальність: ");
    fgets(student.major, MAX_STR, stdin);
    student.major[strcspn(student.major, "\n")] = 0;
    printf("Введіть середній бал (0.0–5.0): ");
    scanf("%f", &student.gpa);
    return student;
}

// Основна функція
int main() {
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
                break;
            }
            case 2: {
                int id;
                printf("Введіть ID студента: ");
                scanf("%d", &id);
                Student newData = inputStudent();
                updateStudent(&db, id, newData);
                saveDatabase(&db);
                break;
            }
            case 3: {
                int id;
                printf("Введіть ID студента: ");
                scanf("%d", &id);
                char confirm;
                printf("Підтвердити видалення? (т/н): ");
                scanf(" %c", &confirm);
                if (confirm == 'т' || confirm == 'Т') {
                    deleteStudent(&db, id);
                    saveDatabase(&db);
                }
                break;
            }
            case 4: {
                int id;
                printf("Введіть ID студента: ");
                scanf("%d", &id);
                Student* student = findStudentById(&db, id);
                if (student) {
                    printf("ID: %d\nПрізвище та ім’я: %s\nДата народження: %s\n",
                           student->id, student->fullName, student->birthDate);
                    printf("Дата зарахування: %s\nСпеціальність: %s\nБал: %.1f\n",
                           student->enrollmentDate, student->major, student->gpa);
                } else {
                    printf("Студента з ID %d не знайдено!\n", id);
                }
                getchar();
                printf("Натисніть Enter для повернення...");
                getchar();
                break;
            }
            case 5:
                listAllStudents(&db);
                getchar();
                printf("Натисніть Enter для повернення...");
                getchar();
                break;
            case 6: {
                int searchType;
                printf("Виберіть тип пошуку (1-ID, 2-Прізвище, 3-Спеціальність): ");
                scanf("%d", &searchType);
                getchar();
                if (searchType == 1) {
                    int id;
                    printf("Введіть ID: ");
                    scanf("%d", &id);
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
                } else if (searchType == 2) {
                    char surname[MAX_STR];
                    printf("Введіть прізвище: ");
                    fgets(surname, MAX_STR, stdin);
                    surname[strcspn(surname, "\n")] = 0;
                    searchBySurname(&db, surname);
                } else if (searchType == 3) {
                    char major[MAX_STR];
                    printf("Введіть спеціальність: ");
                    fgets(major, MAX_STR, stdin);
                    major[strcspn(major, "\n")] = 0;
                    searchByMajor(&db, major);
                }
                printf("Натисніть Enter для повернення...");
                getchar();
                break;
            }
            case 7: {
                float threshold;
                printf("Введіть пороговий бал (0.0–5.0): ");
                scanf("%f", &threshold);
                generateReport(&db, threshold);
                getchar();
                printf("Натисніть Enter для повернення...");
                getchar();
                break;
            }
            default:
                printf("Невірна опція!\n");
                getchar();
                printf("Натисніть Enter для повернення...");
                getchar();
        }
    }

    saveDatabase(&db);
    free(db.students);
    printf("Дані збережено. Програма завершена.\n");
    return 0;
}

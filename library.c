#include <stdio.h>
#include <string.h>

#define MAX_CAMPS 100
#define MAX_STUDENTS 26
#define MAX_THEME_LEN 20

typedef struct {
    int startMonth;
    int startDay;
    int endMonth;
    int endDay;
    char students[MAX_STUDENTS];
    char theme[MAX_THEME_LEN];
} Camp;

Camp camps[MAX_CAMPS];
int campCount = 0;

void readCampsFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File could not be opened.\n");
        return;
    }

    while (fscanf(file, "%d %d %d %d %25s %s",  // Limit students to 25 chars
                  &camps[campCount].startMonth, &camps[campCount].startDay,
                  &camps[campCount].endMonth, &camps[campCount].endDay,
                  camps[campCount].students, camps[campCount].theme) != EOF) {
        camps[campCount].students[MAX_STUDENTS - 1] = '\0';  // Ensure null termination
        campCount++;
    }

    fclose(file);
}

void displayCampInfo() {
    printf("Total number of camps: %d\n", campCount);
    if (campCount > 0) {
        printf("First camp theme: %s\n", camps[0].theme);
        printf("Last camp theme: %s\n", camps[campCount - 1].theme);
    }
}

void findMusicCamp() {
    int found = 0;
    for (int i = 0; i < campCount; i++) {
        if (strcmp(camps[i].theme, "music") == 0) {
            printf("Music camp start date: %d/%d\n", camps[i].startMonth, camps[i].startDay);
            found = 1;
        }
    }
    if (!found) {
        printf("No music camp found.\n");
    }
}

void findCampWithMostStudents() {
    int maxStudents = 0;

    // Find the maximum number of students
    for (int i = 0; i < campCount; i++) {
        int studentCount = strlen(camps[i].students);
        if (studentCount > maxStudents) {
            maxStudents = studentCount;
        }
    }

    printf("The camp with the most students has  :%d participants \n", maxStudents);
    for (int i = 0; i < campCount; i++) {
        if (strlen(camps[i].students) == maxStudents) {
            printf("Most participants camp is : %s, start date: %d/%d\n", camps[i].theme, camps[i].startMonth, camps[i].startDay);
        }
    }
}

int calculateDayNumber(int month, int day) {
    if (month == 6) {
        return day - 16 + 1;
    } else if (month == 7) {
        return 15 + day;
    } else if (month == 8) {
        return 46 + day;
    }
    return -1; // Invalid date
}

void findCampsOnDate(int month, int day) {
    int dayNumber = calculateDayNumber(month, day);
    printf("Camps ongoing on %d/%d:\n", month, day);
    for (int i = 0; i < campCount; i++) {
        int startDayNumber = calculateDayNumber(camps[i].startMonth, camps[i].startDay);
        int endDayNumber = calculateDayNumber(camps[i].endMonth, camps[i].endDay);
        if (dayNumber >= startDayNumber && dayNumber <= endDayNumber) {
            printf("Camp: %s\n", camps[i].theme);
        }
    }
}

void findStudentCamps(char student) {
    FILE *outputFile = fopen("/Users/silacetinkaya/CLionProjects/untitled8/student_camps.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int conflicts = 0;
    int lastEndDay = -1; // To track the last camp's end day

    for (int i = 0; i < campCount; i++) {
        if (strchr(camps[i].students, student)) {
            int startDay = calculateDayNumber(camps[i].startMonth, camps[i].startDay);
            int endDay = calculateDayNumber(camps[i].endMonth, camps[i].endDay);

            fprintf(outputFile, "Camp: %s, Start: %d/%d, End: %d/%d\n",
                    camps[i].theme, camps[i].startMonth, camps[i].startDay,
                    camps[i].endMonth, camps[i].endDay);

            if (startDay <= lastEndDay) {
                conflicts = 1;  // Conflict found
            }
            lastEndDay = endDay;
        }
    }

    fclose(outputFile);

    if (conflicts) {
        printf("Student %c cannot attend more than one camp on the same day.\n", student);
    } else {
        printf("No camp conflicts for student %c.\n", student);
    }
}

int isValidDate(int month, int day) {
    // Geçerli tarih aralığı: 16 Haziran (6/16) ile 31 Ağustos (8/31) arasında
    if ((month == 6 && day >= 16 && day <= 30) ||
        (month == 7 && day >= 1 && day <= 31) ||
        (month == 8 && day >= 1 && day <= 31)) {
        return 1; // Geçerli tarih
    }
    return 0; // Geçersiz tarih
}

void promptDateAndFindCamps() {
    int month, day;
    while (1) {
        printf("Enter month and day (MM DD): ");
        scanf("%d %d", &month, &day);
        if (isValidDate(month, day)) {
            break;  // Geçerli tarih
        } else {
            printf("Invalid date. Please enter a valid date between June 16 and August 31.\n");
        }
    }
    findCampsOnDate(month, day);
}

void sortCampsByStartDate() {
    for (int i = 0; i < campCount - 1; i++) {
        for (int j = 0; j < campCount - i - 1; j++) {
            int startDayJ = calculateDayNumber(camps[j].startMonth, camps[j].startDay);
            int startDayNext = calculateDayNumber(camps[j + 1].startMonth, camps[j + 1].startDay);

            if (startDayJ > startDayNext) {
                // Swap camps[j] and camps[j+1]
                Camp temp = camps[j];
                camps[j] = camps[j + 1];
                camps[j + 1] = temp;
            }
        }
    }
}

void listCampsByStartDate() {
    sortCampsByStartDate();

    printf("Camps sorted by start date:\n");
    for (int i = 0; i < campCount; i++) {
        printf("Camp: %s, start date: %d/%d\n", camps[i].theme, camps[i].startMonth, camps[i].startDay);
    }
}

void sortCampsByDuration() {
    for (int i = 0; i < campCount - 1; i++) {
        for (int j = 0; j < campCount - i - 1; j++) {
            int durationJ = calculateDayNumber(camps[j].endMonth, camps[j].endDay) -
                            calculateDayNumber(camps[j].startMonth, camps[j].startDay) + 1;
            int durationNext = calculateDayNumber(camps[j + 1].endMonth, camps[j + 1].endDay) -
                               calculateDayNumber(camps[j + 1].startMonth, camps[j + 1].startDay) + 1;

            if (durationJ < durationNext) {
                // Swap camps[j] and camps[j+1]
                Camp temp = camps[j];
                camps[j] = camps[j + 1];
                camps[j + 1] = temp;
            }
        }
    }

    printf("Camps sorted by duration:\n");
    for (int i = 0; i < campCount; i++) {
        int duration = calculateDayNumber(camps[i].endMonth, camps[i].endDay) -
                       calculateDayNumber(camps[i].startMonth, camps[i].startDay) + 1;
        printf("Camp: %s, Duration: %d days\n", camps[i].theme, duration);
    }
}

int main() {
    readCampsFile("/Users/silacetinkaya/Desktop/camps.txt");

    displayCampInfo();
    findMusicCamp();
    findCampWithMostStudents();
    promptDateAndFindCamps();

    char student;
    printf("Enter the student's letter code: ");
    scanf(" %c", &student);  // Space before %c to ignore any leftover
    sortCampsByDuration();
    findStudentCamps(student);

    listCampsByStartDate();

    return 0;
}

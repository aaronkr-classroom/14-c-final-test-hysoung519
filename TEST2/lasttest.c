#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터 파일명 정의
#define DATA_FILE "students.dat"
// 최대 학생 수 정의 (배열 크기)
#define MAX_STUDENTS 100

// 1. Student 구조체 정의
typedef struct {
    char name[20];   // 이름
    int kor;         // 국어 점수
    int eng;         // 영어 점수
    int math;        // 수학 점수
    int total;       // 총점
    int avg;         // 평균 (정수로 처리)
    int rank;        // 등수
} Student;

// 학생 데이터를 저장할 전역 배열과 현재 학생 수 변수
Student students[MAX_STUDENTS];
int student_count = 0;

// --- 함수 선언 ---
void print_menu();
void calculate_rank();
void read_data_from_file(const char* filename);
void add_new_student_data();
void save_data_to_file(const char* filename);
void calculate_and_display_students();


// 2. 메뉴 출력 함수 (요구사항)
void print_menu() {
    printf("\n=====================================\n");
    printf("[Menu]\n");
    printf("1. .dat 파일에서 데이터 읽기\n");
    printf("2. 추가 학생 정보 입력\n");
    printf("3. .dat 파일 저장\n");
    printf("4. 성적 확인 (평균 계산 등)\n");
    printf("5. 종료\n");
    printf("-------------------------------------\n");
    printf("현재 저장된 학생 수: %d / %d\n", student_count, MAX_STUDENTS);
    printf("선택 (1 ~ 5): ");
}

// 3. 등수 계산 함수
void calculate_rank() {
    for (int i = 0; i < student_count; i++) {
        students[i].rank = 1;
        for (int j = 0; j < student_count; j++) {
            if (students[j].total > students[i].total) {
                students[i].rank++;
            }
        }
    }
}

// 4. .dat 파일에서 데이터 읽기 함수 (메뉴 1)
void read_data_from_file(const char* filename) {
    FILE* fp;

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("-> 파일 '%s'을(를) 열 수 없습니다. (새로 데이터를 입력하세요.)\n", filename);
        student_count = 0;
        return;
    }

    student_count = fread(students, sizeof(Student), MAX_STUDENTS, fp);

    fclose(fp);
    printf("-> 파일 '%s'에서 총 %d명의 학생 데이터를 읽어왔습니다.\n", filename, student_count);
}

// 5. 추가 학생 정보 입력 함수 (메뉴 2)
void add_new_student_data() {
    if (student_count >= MAX_STUDENTS) {
        printf("-> 배열이 가득 찼습니다. (최대 %d명)\n", MAX_STUDENTS);
        return;
    }

    printf("\n--- 추가 학생 정보 입력 ---\n");

    Student* current = &students[student_count];

    printf("이름: ");
    // scanf_s 사용 시 문자열 입력은 반드시 버퍼 크기 명시
    scanf_s("%s", current->name, (unsigned int)sizeof(current->name));

    printf("국어 점수: ");
    // scanf_s를 사용하려면 &current->kor 뒤에 콤마와 함께 4(int의 크기)를 적어야 할 수도 있습니다. 
    // 하지만 VS에서는 %d에는 생략 가능하여 원래 코드대로 유지합니다.
    scanf_s("%d", &current->kor);

    printf("영어 점수: ");
    scanf_s("%d", &current->eng);

    printf("수학 점수: ");
    scanf_s("%d", &current->math);

    current->total = current->kor + current->eng + current->math;
    current->avg = current->total / 3;

    student_count++;
    printf("-> %s 학생 정보가 배열에 성공적으로 저장되었습니다. (현재 %d명)\n", current->name, student_count);
}

// 6. .dat 파일 저장 함수 (메뉴 3)
void save_data_to_file(const char* filename) {
    if (student_count == 0) {
        printf("-> 저장할 학생 데이터가 없습니다.\n");
        return;
    }

    FILE* fp;
    if ((fp = fopen(filename, "wb")) == NULL) {
        printf("-> 파일 '%s'을(를) 저장하기 위해 열 수 없습니다.\n", filename);
        return;
    }

    fwrite(students, sizeof(Student), student_count, fp);

    fclose(fp);
    printf("-> 총 %d명의 학생 정보가 파일 '%s'에 저장되었습니다.\n", student_count, filename);
}

// 7. 성적 확인 함수 (메뉴 4)
void calculate_and_display_students() {
    if (student_count == 0) {
        printf("-> 입력된 학생 정보가 없습니다.\n");
        return;
    }

    calculate_rank();

    printf("\n--- 성적 확인 (총점, 평균, 등수) ---\n");
    printf("---------------------------------------------------\n");
    printf("| 이름   | 국어 | 영어 | 수학 | 총점 | 평균 | 등수 |\n");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < student_count; i++) {
        printf("| %-6s | %-4d | %-4d | %-4d | %-4d | %-4d | %-4d |\n",
            students[i].name, students[i].kor, students[i].eng, students[i].math,
            students[i].total, students[i].avg, students[i].rank);
    }
    printf("---------------------------------------------------\n");
    printf("-> 성적 확인이 완료되었습니다. 메뉴 화면으로 돌아갑니다.\n");
}

// 8. 메인 함수 (프로그램 흐름 제어)
int main() {
    int choice;

    do {
        print_menu();

        if (scanf_s("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }

        switch (choice) {
        case 1:
            read_data_from_file(DATA_FILE);
            break;
        case 2:
            add_new_student_data();
            break;
        case 3:
            save_data_to_file(DATA_FILE);
            break;
        case 4:
            calculate_and_display_students();
            break;
        case 5:
            printf("\n-> '5' 선택: 프로그램을 종료합니다.\n");
            break;
        default:
            printf("-> 잘못된 선택입니다. 1에서 5 사이의 숫자를 입력하세요.\n");
            break;
        }

    } while (choice != 5);

    return 0;
}
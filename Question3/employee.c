#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define FILENAME "employees.dat"
#define NAME_LEN 30

struct Employee {
    int id;
    char name[NAME_LEN];
    float salary;
};
void write_employee(int fd, int id, const char *name, float salary) {
    struct Employee emp;
    emp.id = id;
    strncpy(emp.name, name, NAME_LEN);
    emp.salary = salary;

    // write() sends raw bytes of the struct directly to the file
    write(fd, &emp, sizeof(struct Employee));
}

// Updates a specific record IN PLACE, without touching any other record
void update_employee(int record_number, float new_salary) {
    int fd = open(FILENAME, O_RDWR);
    if (fd < 0) {
        perror("open failed for update");
        return;
	}
    off_t offset = (off_t) record_number * sizeof(struct Employee);
    lseek(fd, offset, SEEK_SET);
    struct Employee emp;
    read(fd, &emp, sizeof(struct Employee));

    emp.salary = new_salary;

    // Seek back to the SAME position and overwrite only this record
    lseek(fd, offset, SEEK_SET);
    write(fd, &emp, sizeof(struct Employee));

    close(fd);
    printf("Record %d updated: new salary = %.2f\n", record_number, new_salary);
}

// Retrieves and prints a specific record directly, without reading
// through all the records before it
void read_employee(int record_number) {
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0) {
        perror("open failed for read");
        return;
    }

    off_t offset = (off_t) record_number * sizeof(struct Employee);
    lseek(fd, offset, SEEK_SET);

    struct Employee emp;
    ssize_t bytes_read = read(fd, &emp, sizeof(struct Employee));

    if (bytes_read == sizeof(struct Employee)) {
        printf("Record %d -> ID: %d, Name: %s, Salary: %.2f\n",
               record_number, emp.id, emp.name, emp.salary);
    } else {
        printf("Record %d not found.\n", record_number);
    }

    close(fd);
}

int main() {
    // Step 1: Create the file (O_CREAT) and open for writing (O_WRONLY)
    // O_TRUNC clears any old content so we start fresh each run.
    // 0644 sets file permissions: owner can read/write, others can only read.
    int fd = open(FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed for creation");
        exit(1);
    }
    printf("File '%s' created successfully.\n", FILENAME);

    // Step 2: Write 4 employee records
    write_employee(fd, 101, "Rahul Sharma", 45000.00);
    write_employee(fd, 102, "Priya Nair", 52000.00);
    write_employee(fd, 103, "Amit Verma", 38000.00);
    write_employee(fd, 104, "John Mathew", 61000.00);
    close(fd);
    printf("4 employee records written.\n\n");

    // Step 3: Read back all records (sequential, to show initial state)
    printf("---- All records after initial write ----\n");
    for (int i = 0; i < 4; i++) {
        read_employee(i);
    }

    // Step 4: Update record #2 (Amit Verma) WITHOUT rewriting the whole file
    printf("\n---- Updating record 2 (Amit Verma) ----\n");
    update_employee(2, 42000.00);

    // Step 5: Retrieve record #2 directly to prove the update worked,
    // and also retrieve record #0 to prove other records were untouched
    printf("\n---- Verifying after update ----\n");
    read_employee(2);
    read_employee(0);

    return 0;
}

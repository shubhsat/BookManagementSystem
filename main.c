#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BookManagement {
    int ID;
    char Name[100];
    int ISBN;
};

int isExist(struct BookManagement books[], int countBooks, const char *Name, int ISBN) {
    for (int i = 0; i < countBooks; i++) {
        if (strcmp(books[i].Name, Name) == 0 || books[i].ISBN == ISBN) {
            return 1;
        }
    }
    return 0;
}

void getBooks(struct BookManagement books[], int countBooks) {
    printf("Book Database:\n");
    printf("ID\tName\tISBN\n");
    for (int i = 0; i < countBooks; i++) {
        printf("%d\t%s\t%d\n",
               books[i].ID, books[i].Name, books[i].ISBN);
    }
}

void saveToFile(struct BookManagement books[], int countBooks, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "ID\tName\tISBN\n");

    for (int i = 0; i < countBooks; i++) {
        fprintf(file, "%d\t%s\t%d\n", books[i].ID, books[i].Name, books[i].ISBN);
    }

    fclose(file);
}

void loadFromFile(struct BookManagement books[], int *countBooks, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[200];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        struct BookManagement newBook;
        if (sscanf(line, "%d %99s %d", &newBook.ID, newBook.Name, &newBook.ISBN) == 3) {
            books[*countBooks] = newBook;
            (*countBooks)++;
        }
    }

    fclose(file);
}

void updateBook(struct BookManagement books[], int countBooks, int ID) {
    for (int i = 0; i < countBooks; i++) {
        if (books[i].ID == ID) {
            printf("Enter new Book Name: ");
            scanf(" %[^\n]s", books[i].Name);

            int newISBN;
            printf("Enter new ISBN: ");
            if (scanf("%d", &newISBN) != 1) {
                printf("Invalid ISBN format!\n");
                return;
            }

            if (newISBN <= 0 || isExist(books, countBooks, "", newISBN)) {
                printf("Invalid or duplicate ISBN!\n");
                return;
            }
            books[i].ISBN = newISBN;

            printf("Details updated successfully!\n");
            return;
        }
    }
    printf("Not found!\n");
}

void deleteBook(struct BookManagement books[], int *countBooks, int ID) {
    for (int i = 0; i < *countBooks; i++) {
        if (books[i].ID == ID) {
            for (int j = i; j < (*countBooks - 1); j++) {
                books[j] = books[j + 1];
            }
            (*countBooks)--;
            printf("Book deleted successfully\n");
            return;
        }
    }
    printf("Not found!\n");
}

int main() {
    struct BookManagement books[100];
    int countBooks = 0;
    int nextBookID = 1;
    int choice;

    loadFromFile(books, &countBooks, "booksDB.txt");

    do {
        printf("\nBook Management System\n");
        printf("1. Add Book (POST)\n");
        printf("2. List Books (GET)\n");
        printf("3. Update Book (PATCH)\n");
        printf("4. Delete Book (DELETE)\n");
        printf("5. Save Books\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    if (countBooks >= 100) {
                        printf("Database full\n");
                        break;
                    }

                    struct BookManagement newBook;
                    newBook.ID = nextBookID++;

                    printf("Enter Book Name: ");
                    scanf(" %[^\n]s", newBook.Name);

                    if (isExist(books, countBooks, newBook.Name, 0)) {
                        printf("Name already exists\n");
                        break;
                    }

                    int newISBN;
                    printf("Enter ISBN: ");
                    if (scanf("%d", &newISBN) != 1) {
                        printf("Invalid ISBN format\n");
                        break;
                    }

                    if (newISBN <= 0 || isExist(books, countBooks, "", newISBN)) {
                        printf("Invalid or duplicate ISBN\n");
                        break;
                    }
                    newBook.ISBN = newISBN;

                    books[countBooks] = newBook;
                    countBooks++;
                    printf("Book added successfully\n");
                }
                break;

            case 2:
                getBooks(books, countBooks);
                break;

            case 3:
                {
                    int ID;
                    printf("Enter ID to update: ");
                    if (scanf("%d", &ID) != 1) {
                        printf("Invalid input format!\n");
                        break;
                    }
                    updateBook(books, countBooks, ID);
                }
                break;

            case 4:
                {
                    int ID;
                    printf("Enter ID to delete: ");
                    if (scanf("%d", &ID) != 1) {
                        printf("Invalid input format!\n");
                        break;
                    }
                    deleteBook(books, &countBooks, ID);
                }
                break;

            case 5:
                saveToFile(books, countBooks, "booksDB.txt");
                printf("Books saved to file.\n");
                break;

            case 6:
                printf("Exit from Code\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

        while (getchar() != '\n');

    } while (choice != 6);

    return 0;
}

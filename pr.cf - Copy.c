#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct MenuItem {
    int id;
    char name[50];
    float price;
};

struct Order {
    int itemId;
    int quantity;
};

int d = 1;

void password();
void heading();
void mainmenu();
void additem();
void viewmenu();
void searchmenu();
void edititem();
void deleteitem();
void takeorder();
float getPriceById(int id);
void clearScreen();

void clearScreen() {
    system("cls");
}

void heading() {
    clearScreen();
    printf("\n\t\t\t\t\t******* WELCOME TO THE CAFE *******\n\n");
}

void password() {
    char p1[10] = "admin";
    char p2[10];

    printf("\t\tEnter password: ");
    gets(p2);

    if(strcmp(p1, p2) == 0) {
        printf("\n\t\tPassword matched.\n");
        printf("\t\tPress enter to continue...\n");
        getchar();
        mainmenu();
    } else {
        if(d >= 3) {
            printf("\n\t\tMaximum attempts reached. Exiting...\n");
            exit(0);
        }
        printf("\n\t\tWrong password!\n");
        printf("\t\tAttempt %d of 3\n", d);
        d++;
        printf("\t\tPress enter to try again...\n");
        getchar();
        password();
    }
}

void additem() {
    clearScreen();
    struct MenuItem item;

    printf("\n--- Add New Item ---\n");
    printf("Enter item ID: ");
    scanf("%d", &item.id);

    FILE *check = fopen("menu.txt", "r");
    if (check != NULL) {
        struct MenuItem temp;
        int exists = 0;
        while (fscanf(check, "%d %49s %f", &temp.id, temp.name, &temp.price) != EOF) {
            if (temp.id == item.id) {
                exists = 1;
                break;
            }
        }
        fclose(check);
        if (exists) {
            printf("\nItem ID already exists! Please try a different one.\n");
            printf("Press enter to continue...");
            getchar(); getchar();
            return;
        }
    }

    printf("Enter item name: ");
    scanf(" %49[^\n]", item.name);
    printf("Enter price: ");
    scanf("%f", &item.price);

    FILE *fp = fopen("menu.txt", "a");
    if (fp == NULL) {
        printf("\nError: Could not open menu file.\n");
        return;
    }

    fprintf(fp, "%d %s %.2f\n", item.id, item.name, item.price);
    fclose(fp);
    printf("\nItem added successfully!\n");
    printf("Press enter to continue...");
    getchar(); getchar();
}

void viewmenu() {
    clearScreen();
    FILE *fp = fopen("menu.txt","r");
    if (fp == NULL) {
        printf("\nNo menu items found. The menu is empty.\n");
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    struct MenuItem item;
    printf("\n--- Menu ---\n");
    printf("%-5s %-20s %-10s\n", "ID", "Name", "Price");
    printf("---------------------------------\n");

    while (fscanf(fp,"%d %49s %f", &item.id, item.name, &item.price) != EOF) {
        printf("%-5d %-20s $%-10.2f\n", item.id, item.name, item.price);
    }

    fclose(fp);
    printf("\nPress enter to continue...");
    getchar(); getchar();
}

void searchmenu() {
    clearScreen();
    FILE *fp = fopen("menu.txt", "r");
    if (fp == NULL) {
        printf("\nNo menu items found. The menu is empty.\n");
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    struct MenuItem item;
    char searchName[50];
    int found = 0;

    printf("\n--- Search Item ---\n");
    printf("Enter name to search: ");
    scanf(" %49[^\n]", searchName);

    while (fscanf(fp, "%d %49s %f", &item.id, item.name, &item.price) != EOF) {
        if (strstr(item.name, searchName) != NULL) {
            printf("\nFound: ID=%d, Name=%s, Price=%.2f\n", item.id, item.name, item.price);
            found = 1;
        }
    }

    fclose(fp);
    if (!found) {
        printf("\nItem not found.\n");
    }
    printf("Press enter to continue...");
    getchar(); getchar();
}

void edititem() {
    clearScreen();
    FILE *fp = fopen("menu.txt", "r");
    if (fp == NULL) {
        printf("\nNo menu items found. The menu is empty.\n");
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("\nError creating temporary file.\n");
        fclose(fp);
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    struct MenuItem item;
    int editId, found = 0;

    printf("\n--- Edit Item ---\n");
    printf("Enter ID of item to edit: ");
    scanf("%d", &editId);

    while (fscanf(fp, "%d %49s %f", &item.id, item.name, &item.price) != EOF) {
        if (item.id == editId) {
            printf("\nCurrent details: ID=%d, Name=%s, Price=%.2f\n", item.id, item.name, item.price);
            printf("Enter new name: ");
            scanf(" %49[^\n]", item.name);
            printf("Enter new price: ");
            scanf("%f", &item.price);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.price);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("menu.txt");
        rename("temp.txt", "menu.txt");
        printf("\nItem updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("\nItem not found.\n");
    }
    printf("Press enter to continue...");
    getchar(); getchar();
}

void deleteitem() {
    clearScreen();
    FILE *fp = fopen("menu.txt", "r");
    if (fp == NULL) {
        printf("\nNo menu items found. The menu is empty.\n");
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("\nError creating temporary file.\n");
        fclose(fp);
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    struct MenuItem item;
    int delId, found = 0;

    printf("\n--- Delete Item ---\n");
    printf("Enter ID of item to delete: ");
    scanf("%d", &delId);

    while (fscanf(fp, "%d %49s %f", &item.id, item.name, &item.price) != EOF) {
        if (item.id == delId) {
            printf("\nDeleted item: ID=%d, Name=%s, Price=%.2f\n", item.id, item.name, item.price);
            found = 1;
        } else {
            fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.price);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("menu.txt");
        rename("temp.txt", "menu.txt");
        printf("\nItem deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("\nItem not found.\n");
    }
    printf("Press enter to continue...");
    getchar(); getchar();
}

float getPriceById(int id) {
    FILE *fp = fopen("menu.txt", "r");
    if (fp == NULL) {
        return -1;
    }

    struct MenuItem item;
    float price = -1;

    while (fscanf(fp, "%d %49s %f", &item.id, item.name, &item.price) != EOF) {
        if (item.id == id) {
            price = item.price;
            break;
        }
    }

    fclose(fp);
    return price;
}

void takeorder() {
    clearScreen();
    struct Order orders[10];
    int count = 0, id, qty;
    float total = 0, price;

    printf("\n--- Place Order ---\n");
    viewmenu(); // Show menu first

    while (1) {
        printf("\nEnter item ID (0 to finish): ");
        scanf("%d", &id);
        if (id == 0) break;

        price = getPriceById(id);
        if (price == -1) {
            printf("Invalid ID! Please try again.\n");
            continue;
        }

        printf("Enter quantity: ");
        scanf("%d", &qty);

        if (qty <= 0) {
            printf("Quantity must be positive! Please try again.\n");
            continue;
        }

        orders[count].itemId = id;
        orders[count].quantity = qty;
        total += price * qty;
        count++;

        printf("Added to order: Item ID %d x %d\n", id, qty);
    }

    if (count == 0) {
        printf("\nNo items were ordered.\n");
        printf("Press enter to continue...");
        getchar(); getchar();
        return;
    }

    printf("\n--- Bill ---\n");
    for (int i = 0; i < count; i++) {
        price = getPriceById(orders[i].itemId);
        printf("Item ID: %-5d | Qty: %-3d | Subtotal: $%-8.2f\n",
            orders[i].itemId, orders[i].quantity, price * orders[i].quantity);
    }

    printf("\nTotal: $%.2f\n", total);

    FILE *fp = fopen("orders.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "Order total: %.2f\n", total);
        fclose(fp);
        printf("\nOrder saved to history.\n");
    } else {
        printf("\nWarning: Could not save order to history.\n");
    }

    printf("Press enter to continue...");
    getchar(); getchar();
}

void mainmenu() {
    int choice;

    while (1) {
        clearScreen();
        printf("\n--- Cafe Management System ---\n");
        printf("1. Add Menu Item\n");
        printf("2. View Menu\n");
        printf("3. Search Menu Item\n");
        printf("4. Edit Menu Item\n");
        printf("5. Delete Menu Item\n");
        printf("6. Take Order\n");
        printf("7. Exit\n");
        printf("-----------------------------\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: additem(); break;
            case 2: viewmenu(); break;
            case 3: searchmenu(); break;
            case 4: edititem(); break;
            case 5: deleteitem(); break;
            case 6: takeorder(); break;
            case 7:
                printf("\nExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please try again.\n");
                printf("Press enter to continue...");
                getchar();
        }
    }
}

int main() {
    heading();
    password();
    mainmenu();
    return 0;
}


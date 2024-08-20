#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_SIZE 1000
#define MAX_LINE_SIZE 256

void menu();
void add();
void quit();
void advanced_search();
void transfer();
void withdraw();
void deposit();
void modify();
void report();
void search();
void quit_login();
void load();
void save();
void delete();
void lower_size();
int login();
int is_valid_name(char *name);
int is_valid_address(char *address);
int is_valid_mobile(char *mobile);
int index_of_account(unsigned long long int accnum);
unsigned long long int find_last_account();
void clearInputBuffer();
void print();
int yes_no();

typedef struct
{
    char user[50];
    char pass[50];
} user;
// Global variables to hold the number of accounts, the current account number, and the last account number
int num_accounts = 0;

unsigned long long int last_account = 9700000000;
unsigned long long int first_account = 9700000000;
int buffer_flag = 0;
unsigned long long int find_last_account() // Finds the last account number in the file "accounts.txt"
{
    unsigned long long int last = 9700000000;
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }
    char line[256];
    while (fgets(line, sizeof(line), f) != NULL) // Reads the file line by line
    {
        char *token = strtok(line, ",");
        if (token)              // If the token is not null
            last = atoll(token); // Converts the first token in the line to an integer and stores it in the variable "last"
    }
    fclose(f);
    return last; // Returns the last account number
}
unsigned long long int find_first_account()
{
    unsigned long long int first = 9700000000;
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }
    if (fscanf(f, "%llu", &first) != 1) // Reads the first account number directly into "first"
    {
        printf("Error reading account number\n");
        fclose(f);
        return 0;
    }
    fclose(f);
    return first;
}
// Define a structure to hold a date
typedef struct
{
    int month;
    char year[50];
} Date;

typedef struct
{
    unsigned long long int accnum;
    char name[50];
    char address[50];
    float balance;
    char mobile[20];
    Date date_opened;
} account;

account *accounts;
account *accounts_copy;
int lastSize;

int yes_no()
{
    char c[50];
    do
    {
        fgets(c, 50, stdin);
        if (strchr(c, '\n') == NULL) // If c doesn't contain a newline, it means that fgets() read more characters than it could store in c
            clearInputBuffer();      // So we clear the input buffer
        c[strcspn(c, "\n")] = 0;     // Remove the newline character
        for (int j = 0; j < strlen(c); j++)
        {
            c[j] = tolower(c[j]);
        }

        if (strcmp(c, "y") == 0)
            return 1;
        else if (strcmp(c, "n") == 0)
            return 0;
        else
            printf("Invalid input, Try again.\n");
    } while (1);
    return 0;
}

const char *monthNames[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void clearInputBuffer() // Clears the input buffer
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int index_of_account(unsigned long long int accnum)
{
    for (int i = 0; i < num_accounts; i++)
    {
        if (accounts[i].accnum == accnum)
        {
            return i;
        }
    }
    return -1;
}

void load()
{
    last_account = find_last_account(); // Finds the last account number in the file "accounts.txt"
    first_account = find_first_account();
    lastSize = 2 * (last_account - first_account + 1);
    // Allocate memory for the accounts array
    accounts = (account *)malloc(lastSize * sizeof(account));
    accounts_copy = (account *)malloc(lastSize * sizeof(account));
    // Open the file in read mode
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL)
    {
        printf("Could not open file accounts.txt\n");
        return;
    }

    // Initialize the number of accounts to 0
    num_accounts = 0;

    char line[256];
    // Read each line from the file until the end of the file is reached or the maximum number of accounts is reached
    while (fgets(line, sizeof(line), file) && num_accounts < 100)
    {
        // Tokenize the line using comma as the delimiter and read each field into the account structure
        char *token = strtok(line, ",");
        if (token)
            accounts[num_accounts].accnum = atoll(token);
        accounts_copy[num_accounts].accnum = atoll(token);
        char y[50];
        sprintf(y, "%lld.txt", accounts[num_accounts].accnum);
        FILE *f2 = fopen(y, "a");
        if (f2 == NULL)
        {
            printf("Could not open or create file %llu.txt\n", accounts[num_accounts].accnum);
            return;
        }
        fclose(f2);
        token = strtok(NULL, ",");
        if (token)
            strncpy(accounts[num_accounts].name, token, 49);
        strncpy(accounts_copy[num_accounts].name, token, 49);

        token = strtok(NULL, ",");
        if (token)
            strncpy(accounts[num_accounts].address, token, 49);
        strncpy(accounts_copy[num_accounts].address, token, 49);

        token = strtok(NULL, ",");
        if (token)
            accounts[num_accounts].balance = atof(token);
        accounts_copy[num_accounts].balance = atof(token);

        token = strtok(NULL, ",");
        if (token)
            strncpy(accounts[num_accounts].mobile, token, 19);
        strncpy(accounts_copy[num_accounts].mobile, token, 19);

        token = strtok(NULL, "-");
        if (token)
            accounts[num_accounts].date_opened.month = atoi(token);
        accounts_copy[num_accounts].date_opened.month = atoi(token);

        token = strtok(NULL, "\n");
        if (token)
            strncpy(accounts[num_accounts].date_opened.year, token, 49);
        strncpy(accounts_copy[num_accounts].date_opened.year, token, 49);

        num_accounts++;
    }

    fclose(file);

    char x;
    int b;
    printf("Would you like to go to the menu?(y/n)\n");
    if(buffer_flag == 0)
    {
        clearInputBuffer();
    }
    buffer_flag = 0;
    b = yes_no();
    if (b == 1)
    {
        menu();
        return;
    }
    else if (b == 0)
    {
        quit();
        return;
    }
    else
        printf("Invalid input, Try again.\n");
}

void lower_size()
{
    if (num_accounts < lastSize / 2)
    {
        lastSize /= 2;
        accounts = (account *)realloc(accounts, lastSize * sizeof(account));
        accounts_copy = (account *)realloc(accounts_copy, lastSize * sizeof(account));
    }
}

void delete()
{
    unsigned long long int accnum;
    int validate = 0;
    int i = -1;

    while (validate == 0 || i == -1) {
        printf("Enter account number: ");
        fflush(stdin);
        validate = scanf(" %llu", &accnum);
        fflush(stdin);

        if (validate == 0) {
            //clearInputBuffer();
            printf("Invalid input\n");
            continue;
        }

        i = index_of_account(accnum);
        if (i == -1) {
            //clearInputBuffer();
            printf("Account number does not exist\n");
            continue;
        }
    }

    if (accounts[i].balance != 0)
    {
        printf("Account balance is not 0, cannot delete account\n");
        //clearInputBuffer();
        return;
    }

    printf("Are you sure you'd like to delete this account?(y/n)\n");
    int c = yes_no();

    if (c == 1)
    {
        for (int j = i; j < num_accounts - 1; j++)
        {
            accounts[j] = accounts[j + 1];
        }
        num_accounts--;
        save();
        printf("Account deleted!\n");

        char* filename = malloc(20);
        sprintf(filename, "%llu.txt", accnum);
        if (remove(filename) == 0)
            printf("Account history deleted successfully!\n");
        else
            printf("Unable to delete the account history!\n");
        free(filename);

        last_account = find_last_account();

        lower_size();
        // clearInputBuffer();
        buffer_flag = 1;
        load();
        return;
    }
    else if (c == 0)
    {
        //clearInputBuffer();
        return;
    }
}

int is_valid_balance(char *Amount)
{
    int i, n=strlen(Amount);
    if(Amount[n-1]=='\n')
      Amount[n-1]='\0';
    int new = strlen(Amount);
    for(i=0; i<new; i++)
    {
        if((Amount[i]<'0' || Amount[i]>'9') && Amount[i]!='.')
        {
            return 0;
        }
    }
    return 1;
}

void add()
{
    account hold;
    last_account += 1;
    num_accounts += 1;

    hold.accnum = last_account;
    printf("Please input Name: ");
    while (1)
    {
        fgets(hold.name, sizeof(hold.name), stdin);
        hold.name[strcspn(hold.name, "\n")] = 0; // remove newline character
        if (is_valid_name(hold.name))
        {
            hold.name[0] = toupper(hold.name[0]);
            for (int i = 1; i < strlen(hold.name); i++)
            {
                if (hold.name[i - 1] == ' ')
                    hold.name[i] = toupper(hold.name[i]);
                else
                    hold.name[i] = tolower(hold.name[i]);
            }
            break;
        }
        else
            printf("Invalid name.\nPlease input Name: ");
    }
    printf("Please input Address: ");
    while (1)
    {
        fgets(hold.address, sizeof(hold.address), stdin);
        hold.address[strcspn(hold.address, "\n")] = 0; // remove newline character
        if (is_valid_address(hold.address))
            break;
        else
            printf("Invalid email.\nPlease input Address: ");
    }
    while (1)
    {
        char balance_str[50];
        printf("Please input Balance: ");
        fgets(balance_str, sizeof(balance_str), stdin);
        //clearInputBuffer();
        if (is_valid_balance(balance_str))
        {
            hold.balance = strtof(balance_str, NULL); // Convert the valid balance string to float
            break;
        }
        else
        {
            printf("Invalid balance.\n");
        }
    }

    printf("Please input mobile number: ");
    while (1)
    {
        fgets(hold.mobile, sizeof(hold.mobile), stdin);
        hold.mobile[strcspn(hold.mobile, "\n")] = 0; // remove newline character
        int x = is_valid_mobile(hold.mobile);
        if (x)
            break;
        else
            printf("Invalid mobile number.\nPlease input mobile number: ");
    }
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    hold.date_opened.month = tm->tm_mon + 1;
    sprintf(hold.date_opened.year, "%d", tm->tm_year + 1900);

    printf("do you want to save?(y/n):");
    int c = yes_no();

    if (c == 1)
    {

        if (num_accounts == lastSize)
        {
            lastSize *= 2;
            accounts = (account *)realloc(accounts, lastSize * sizeof(account));
            accounts_copy = (account *)realloc(accounts_copy, lastSize * sizeof(account));
        }
        accounts[num_accounts - 1] = hold;
        accounts_copy[num_accounts - 1] = hold;
        save();
        printf("Account added!\n");
        char filename[50];
        sprintf(filename, "%llu.txt", hold.accnum);
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("Error: Could not open file %s\n", filename);
            return;
        }
        fclose(file);
        buffer_flag = 1;
        load();
    }
    else if (c == 0)
    {
        last_account -= 1;
        num_accounts -= 1;
        return;
    }
}

void save()
{
    FILE *f = fopen("accounts.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    for (int i = 0; i < num_accounts; i++)
    {
        fprintf(f, "%llu,%s,%s,%.2f,%s,%d-%s\n", accounts[i].accnum, accounts[i].name, accounts[i].address, accounts[i].balance, accounts[i].mobile, accounts[i].date_opened.month, accounts[i].date_opened.year);
    }
    fclose(f);
}

void report()
{
    unsigned long long int accnum;
    printf("Enter account number: ");
    int validate = 0;
    int i = -1;

    while (validate == 0 || i == -1) {
        validate = scanf("%llu", &accnum);
        if (validate == 0) {
            clearInputBuffer();
            printf("Invalid input\n");
            continue;
        }
        i = index_of_account(accnum);
        if (i == -1) {
            clearInputBuffer();
            printf("Account number does not exist\n");
            continue;
        }
    }
    char filename[50];
    sprintf(filename, "%llu.txt", accnum);
    FILE *f3 = fopen(filename, "r");
    if (f3 == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    char transactions[MAX_SIZE][MAX_LINE_SIZE];
    int count = 0;
    while (fgets(transactions[count], sizeof(transactions[count]), f3) != NULL && count < MAX_SIZE)
    {
        count++;
    }
    fclose(f3);

    // Print the last 5 transactions in reverse order
    for (int j = count - 1; j >= 0 && j >= count - 5; j--)
    {
        printf("%s", transactions[j]);
    }
    clearInputBuffer();
}

void search()
{
    int pointer, found_flag = 1;
    unsigned long long int search_key, i;
    while (1)
    {

        printf("Input the search key(account number):");
        int error = scanf(" %llu", &search_key);
        clearInputBuffer();
        if (error == 0)
        {
            printf("Invalid input, please try again!\n");
            continue;
        }
        if (search_key > last_account || search_key < first_account)
        {
            printf("Invalid account number, please try again!\n");
            continue;
        }


        for (i = first_account; i <= last_account; i++)
        {
            if (search_key == i)
            {
                if(accounts[index_of_account(search_key)].accnum == 0)
                {
                    printf("This account was deleted!\n");
                    return;
                }

                found_flag = 0;
                pointer = index_of_account(search_key);
                printf("Account number: %llu\n", accounts[pointer].accnum);
                printf("Name: %s\n", accounts[pointer].name);
                printf("Address: %s\n", accounts[pointer].address);
                printf("Balance: %.2f\n", accounts[pointer].balance);
                printf("Mobile: %s\n", accounts[pointer].mobile);
                printf("Date opened: %s %s\n", (char *)monthNames[accounts[pointer].date_opened.month], (char *)accounts[pointer].date_opened.year);
                // clearInputBuffer();
                return;
            }
        }
        if (found_flag)
        {
            printf("There is no account found with this number, please try again!\n");
        }
    }
}

void quit()
{
    printf("Are you sure you'd like to quit?(y/n)\n");
    int c = yes_no();
    if (c == 1)
        exit(0);
    else if (c == 0)
        menu();
        return;
}

void quit_login()
{
    printf("Goodbye!\n");
    exit(0);
}

void menu()
{
    fflush(stdin);

    char hold[50];
    int flag = 1;
    printf("1)ADD\n");
    printf("2)DELETE\n");
    printf("3)MODIFY\n");
    printf("4)SEARCH\n");
    printf("5)ADVANCED SEARCH\n");
    printf("6)WITHDRAW\n");
    printf("7)DEPOSIT\n");
    printf("8)TRANSFER\n");
    printf("9)REPORT\n");
    printf("10)PRINT\n");
    printf("11)QUIT\n");

    while (flag)
    {
        printf("enter value of needed function(1-11): ");
        fgets(hold, sizeof(hold), stdin);
        if (strlen(hold) > 3)
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if ((strlen(hold) == 3 && !(isdigit(hold[0]) && isdigit(hold[1]))) || strlen(hold) == 2 && !isdigit(hold[0]))
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        int choice = atoi(hold);
        switch (choice)
        {
        case 1:
            add();
            flag = 0;
            break;
        case 2:
            delete ();
            flag = 0;
            break;
        case 3:
            modify();
            flag = 0;
            break;
        case 4:
            search();
            flag = 0;
            break;
        case 5:
            advanced_search();
            flag = 0;
            break;
        case 6:
            withdraw();
            flag = 0;
            break;
        case 7:
            deposit();
            flag = 0;
            break;
        case 8:
            transfer();
            flag = 0;
            break;
        case 9:
            report();
            flag = 0;
            break;
        case 10:
            print();
            flag = 0;
            break;
        case 11:
            quit();
            flag = 0;
            break;
        default:
            printf("wrong input, please try again!\n");
        }
    }

    printf("Would you like to return to the menu?(y/n)\n");
    fflush(stdin);
    int c = yes_no();
    if (c == 1)
        menu();
    else if (c == 0)
        quit();
    return;
}

int is_valid_name(char *name)
{
    int i = 0;
    while (name[i] != '\0')
    {
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int is_valid_address(char *address)
{
    int i = 0, flag_at = 0, flag_dot = 0;
    while (address[i] != '\0')
    {
        if (!isalnum(address[i]) && address[i] != '@' && address[i] != '_' && address[i] != '.' || address[i] == ' ')
        {
            return 0;
        }

        if (address[i] == '@')
        {
            flag_at++;
        }

        if (address[i] == '.' && address[i + 1] == '.')
        {
            return 0;
        }

        if (address[i] == '.' && address[i + 1] == '@')
        {
            return 0;
        }

        if (address[strlen(address) - 4] == '.' || address[strlen(address) - 3] == '.')
        {
            flag_dot = 1;
        }
        i++;
    }

    if (address[0] == '.' || address[0] == '@' || address[0] == '_' || address[strlen(address) - 1] == '.' || address[strlen(address) - 1] == '@' || address[strlen(address) - 1] == '_')
    {
        return 0;
    }
    if (flag_at != 1 || flag_dot != 1)
    {
        return 0;
    }
    return 1;
}

int is_valid_mobile(char *mobile)
{
    if (strlen(mobile) != 11 || mobile[0] != '0' || mobile[1] != '1')
    {
        return 0;
    }

    int n = strlen(mobile);
    for (int i = 0; i < n; i++)
    {
        if (mobile[i] < '0' || mobile[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

void modify()
{
    buffer_flag = 1;
    unsigned long long int accnum;
    int n;
    char m[50];
    account temp;
    int validate = 0;
    int index = -1;
    //clearInputBuffer();
    while (validate == 0 || index == -1)
    {
        printf("Enter account number: ");
        validate = scanf(" %llu", &accnum);
        if (validate == 0)
        {
            printf("Invalid input\n");
        }
        else
        {
            index = index_of_account(accnum);
            if (index == -1)
            {
                printf("Account number does not exist\n");
            }
        }
    }
    printf("1)Name\n2)email\n3)mobile number\n");
    clearInputBuffer();
    printf("Choose what you want to change:\n");
    do
    {
        fgets(m, sizeof(m), stdin);
        if (strlen(m) > 2 || !(isdigit(m[0])))
        {
            printf("Wrong input, try again:");
            continue;
        }
        n = atoi(m);
        switch (n)
        {
        case 1:

            do
            {
                printf("Enter new name: ");
                fgets(temp.name, 50, stdin);
                if (temp.name[strlen(temp.name) - 1] == '\n')
                    temp.name[strlen(temp.name) - 1] = '\0';
            } while (!is_valid_name(temp.name));

            break;
        case 2:
            do
            {
                printf("Enter new address: ");
                fgets(temp.address, 50, stdin);
                if (temp.address[strlen(temp.address) - 1] == '\n')
                    temp.address[strlen(temp.address) - 1] = '\0';
            } while (!is_valid_address(temp.address));
            break;
        case 3:
            char mobile[20];
            do
            {
                printf("Enter new mobile: ");
                fgets(temp.mobile, 20, stdin);
                if (temp.mobile[strlen(temp.mobile) - 1] == '\n')
                    temp.mobile[strlen(temp.mobile) - 1] = '\0';
            } while (!is_valid_mobile(temp.mobile));
            break;
        default:
            printf("Wrong input, try again:");
        }
    } while (n != 1 && n != 2 && n != 3);

    int c;
    printf("Would you like to save changes?(y/n)\n");
    c = yes_no();
    if (c == 1)
    {
        switch (n)
        {
        case 1:
            strcpy(accounts[index].name, temp.name);
            break;
        case 2:
            strcpy(accounts[index].address, temp.address);
            break;
        case 3:
            strcpy(accounts[index].mobile, temp.mobile);
            break;
        }
        save();
        load();
        printf("Changes saved!\n");

        return;
    }
    else if (c == 0)
    {
        return;
    }
}

void transfer()
{
    unsigned long long int reciver_acc_num;
    unsigned long long int sender_acc_num;
    float amount;
    int validate = 0, validate2 = 0;
    int x = -1, y = -1;

    while (validate == 0 || x == -1 || validate2 == 0 || y == -1) {
        if (validate == 0 || x == -1) {
            printf("Enter account number to transfer from: ");
            validate = scanf("%llu", &sender_acc_num);
            
            if (validate == 0) {
                clearInputBuffer();
                printf("Invalid input\n");
                continue;
            }
            x = index_of_account(sender_acc_num);
            if (x == -1) {
                clearInputBuffer();
                printf("Account number does not exist\n");
                continue;
            }
        }
        if (validate2 == 0 || y == -1) {
            printf("Enter account number to transfer to: ");
            validate2 = scanf("%llu", &reciver_acc_num);
            
            if (validate2 == 0) {
               clearInputBuffer(); 
               printf("Invalid input\n");
                continue;
            }
            y = index_of_account(reciver_acc_num);
            if (y == -1) {
                clearInputBuffer();
                printf("Account number does not exist\n");
                continue;
            }
        }
    }
    if (sender_acc_num == reciver_acc_num)
    {
        printf("Cannot transfer to the same account\n");
        return;
    }
    char amount_str[50];
    clearInputBuffer();
    do
    {
        printf("Enter the amount to transfer: ");
        fgets(amount_str, sizeof(amount_str), stdin); 
        if (is_valid_balance(amount_str)) 
        {
            amount = strtof(amount_str, NULL); // Convert the valid amount string to float
            if (amount > accounts[x].balance)
            {
                printf("Insufficient funds. Try again.\n");
                continue;
            }
            break;
        }
        else
        {
            printf("Invalid input. Please enter a number.\n");
        }
    } while (1);
    printf("Would you like to save changes?(y/n)\n");
    //clearInputBuffer();
    int c = yes_no();
    if (c == 1)
    {
        accounts[x].balance -= amount;
        accounts[y].balance += amount;
        printf("Transfer successful\n");
        save();
        // Open the sender's file in append mode
        char filename_sender[50];
        sprintf(filename_sender, "%llu.txt", accounts[x].accnum);
        FILE *file_sender = fopen(filename_sender, "a");

        if (file_sender == NULL)
        {
            printf("Error opening sender's file!\n");
            return;
        }

        // Write the transaction details to the sender's file
        fprintf(file_sender, "Transfer to %llu: $%.2f\n", accounts[y].accnum, amount);

        // Close the sender's file
        fclose(file_sender);

        // Open the receiver's file in append mode
        char filename_receiver[50];
        sprintf(filename_receiver, "%llu.txt", accounts[y].accnum);
        FILE *file_receiver = fopen(filename_receiver, "a");

        if (file_receiver == NULL)
        {
            printf("Error opening receiver's file!\n");
            return;
        }

        // Write the transaction details to the receiver's file
        fprintf(file_receiver, "Transfer from %llu: $%.2f\n", accounts[x].accnum, amount);

        // Close the receiver's file
        fclose(file_receiver);
        buffer_flag = 1;
        load();
    }
    else if (c == 0)
        return;
}

void withdraw()
{
    unsigned long long int accnum;
    float amount;
    int validate = 0;
    int i = -1;

    while (validate == 0 || i == -1) {
        printf("Enter account number: ");
        validate = scanf(" %llu", &accnum);
        
        if (validate == 0) {
            clearInputBuffer();
            printf("Invalid input\n");
            continue;
        }
        i = index_of_account(accnum);
        if (i == -1) {
            clearInputBuffer();
            printf("Account number does not exist\n");
            continue;
        }
    }
        clearInputBuffer();
    do
    {
        char amount_str[50];
        printf("Enter amount to withdraw: ");
        fgets(amount_str, sizeof(amount_str), stdin); 
        if (is_valid_balance(amount_str)) 
        {
            amount = strtof(amount_str, NULL); 
            if (amount < 0)
            {
                printf("Invalid amount\n");
                continue;
            }
            if (amount > accounts[i].balance)
            {
                printf("Insufficient funds\n");
                continue;
            }
            break;
        }
        else
        {
            printf("Invalid input, try again!\n");
        }
    } while (1);

    printf("Would you like to save changes?(y/n)\n");
    //clearInputBuffer();
    int c = yes_no();

    if (c == 1)
    {
        accounts[i].balance -= amount;
        char filename[50];
        sprintf(filename, "%llu.txt", accnum);
        FILE *file = fopen(filename, "a");

        if (file == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        // Write the transaction details to the file
        fprintf(file, "Withdrawal: $%.2f\n", amount);

        // Close the file
        fclose(file);
        printf("Withdraw successful\n");
        save();
        buffer_flag = 1;
        load();
    }
    else if (c == 0)
    {
        return;
    }
}

void deposit()
{
    unsigned long long int accnum;
    float amount;
    int validate = 0;
    int i = -1;

    //clearInputBuffer();
    while (validate == 0 || i == -1) {
        printf("Enter account number: ");
        validate = scanf(" %llu", &accnum);
        fflush(stdin);
        if (validate == 0) {
            printf("Invalid input\n");
            continue;
        }
        i = index_of_account(accnum);
        if (i == -1) {
            printf("Account number does not exist\n");
            continue;
        }
    }
    //clearInputBuffer();
    do
    {
        char amount_str[50];
        printf("Enter amount to deposit: ");
        fgets(amount_str, sizeof(amount_str), stdin);

        if (is_valid_balance(amount_str))
        {
            amount = strtof(amount_str, NULL);
            if (amount < 0)
            {
                printf("Invalid amount, try again!\n");
                continue;
            }
            break;
        }
        else
        {
            printf("Invalid input, try again!\n");
        }
    } while (1);


    printf("Would you like to save changes?(y/n)\n");
    //clearInputBuffer();
    int c = yes_no();
    if (c == 1)
    {
        accounts[i].balance += amount;
        char filename[50];
        sprintf(filename, "%llu.txt", accnum);
        FILE *file = fopen(filename, "a");

        if (file == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        // Write the transaction details to the file
        fprintf(file, "Deposit: $%.2f\n", amount);

        // Close the file
        fclose(file);
        printf("Deposit successful\n");
        save();
        buffer_flag = 1;
        load();
        return;
    }
    else if (c == 0)
    {
        return;
    }
}

void swap(account *arr, int index)
{
    account temp = arr[index];
    arr[index] = arr[index + 1];
    arr[index + 1] = temp;
}

int date_int(Date a)
{
    int hold;
    hold = 100 * atoi(a.year) + a.month;
    return hold;
}

void SortByBalance()
{
    int i, j;
    for (i = 0; i < num_accounts - 1; i++)
    {
        for (j = 0; j < num_accounts - i - 1; j++)
        {
            if (accounts_copy[j].balance > accounts_copy[j + 1].balance)
            {
                swap(accounts_copy, j);
            }
        }
    }
}

void SortByDate()
{
    int i, j;
    for (i = 0; i < num_accounts - 1; i++)
    {
        for (j = 0; j < num_accounts - i - 1; j++)
        {
            if (date_int(accounts_copy[j].date_opened) < date_int(accounts_copy[j + 1].date_opened))
            {
                swap(accounts_copy, j);
            }
        }
    }
}

void SortByName()
{
    int i, j;
    for (i = 0; i < num_accounts - 1; i++)
    {
        for (j = 0; j < num_accounts - i - 1; j++)
        {
            if (strcmp(accounts_copy[j].name, accounts_copy[j + 1].name) > 0)
            {
                swap(accounts_copy, j);
            }
        }
    }
}

void print()
{
    char hold[50];
    int flag = 1;
    printf("CHOOSE SORTING ALGORITHM: \n");
    printf("1)Name\n");
    printf("2)Date\n");
    printf("3)Balance\n");
    printf("INPUT SORTING ALGORITHM(1-3): ");
    fgets(hold, sizeof(hold), stdin);

    while (flag)
    {
        if (strlen(hold) > 2)
        {
            printf("Wrong Input please try again!\n");
            fgets(hold, sizeof(hold), stdin);
            continue;
        }
        switch (hold[0])
        {
        case '1':
            SortByName();
            flag = 0;
            break;
        case '2':
            SortByDate();
            flag = 0;
            break;
        case '3':
            SortByBalance();
            flag = 0;
            break;
        default:
            printf("Wrong Input please try again!\n");
            fgets(hold, sizeof(hold), stdin);
            break;
        }
    }
    for (int i = 0; i < num_accounts; i++)
    {
        printf("Account number: %llu\n", accounts_copy[i].accnum);
        printf("Name: %s\n", accounts_copy[i].name);
        printf("Address: %s\n", accounts_copy[i].address);
        printf("Balance: %.2f\n", accounts_copy[i].balance);
        printf("Mobile: %s\n", accounts_copy[i].mobile);
        printf("Date opened: %s %s\n", (char *)monthNames[accounts_copy[i].date_opened.month], (char *)accounts_copy[i].date_opened.year);
        printf("\n");
    }
}

int login()
{

    // opens the file
    FILE *f = fopen("users.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }
    user u[10];
    int i = 0;
    // reads the usernames and passwords from the file
    // EOF is end of file character which is used to check if the file has ended
    while (fscanf(f, "%s %s", u[i].user, u[i].pass) != EOF && i < 10)
    {
        i++;
    }
    fclose(f);

    char c;
    // Declare character arrays to store username and password
    char username[50];
    char password[50];
    // Prompt the user to enter a username and password
    printf("Enter username: ");
    scanf("%s", username);
    printf("\n");
    printf("Enter password: ");
    scanf("%s", password);
    // Declare a flag variable to check if the username and password are correct
    int flag = 0;
    // compares username and password with the ones in the file
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(username, u[i].user) == 0 && strcmp(password, u[i].pass) == 0)
        {
            flag = 1;
            printf("Login successful!\n");
            load();
            break;
        }
    }
    // if username and password are incorrect
    if (flag == 0)
    {
        printf("Incorrect username or password\n");
        printf("Try again?(y/n)\n");
        clearInputBuffer();
        int c = yes_no();
        if (c == 1)
            login();
        else if (c == 0)
        {
            quit_login();
        }
    }
    return 1;
}

// Function to search for a keyword in the file "accounts.txt" and print the lines that contain the keyword
void advanced_search()
{
    int found = 0; // Flag variable to check if the keyword was found in the file

    // Asks the user for a keyword to search for
    char keyword[50];
    printf("Enter keyword to search for: ");
    scanf("%49[^\n]", keyword);
    fflush(stdin);

    // Converts the "keyword" string to lowercase
    for (int i = 0; i < strlen(keyword); i++)
    {
        keyword[i] = tolower(keyword[i]);
    }

    // Sets the last character of the string to null if it is a newline character
    if (keyword[strlen(keyword) - 1] == '\n')
    {
        keyword[strlen(keyword) - 1] = '\0';
    }

    // Opens the file "accounts.txt" for reading
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Reads the file line by line and prints the lines that contain the keyword
    char *c;
    char *temp;
    int i = 0;
    // int num = find_last_account() - FIRST_ACCOUNT_NUMBER;
    while (!feof(f) && !ferror(f))
    {
        c = (char *)malloc(256);
        fgets(c, 256, f);                     // Reads a line from the file and stores it in the string "c"
        temp = (char *)malloc(strlen(c) + 1); // Creates a temporary string to store the original line
        strcpy(temp, c);

        // Converts the "c" string to lowercase
        for (int i = 0; i < strlen(c); i++)
        {
            c[i] = tolower(c[i]);
        }

        // Checks if the line contains the keyword
        if (strstr(c, keyword) != NULL)
        { // strstr function returns a pointer to the first occurrence of the keyword in the line
            char *accountNumber = strtok(temp, ",");
            printf("Account Number: %s\n", accountNumber);
            char *name = strtok(NULL, ",");
            printf("Name: %s\n", name);
            char *email = strtok(NULL, ",");
            printf("Email: %s\n", email);
            char *balance = strtok(NULL, ",");
            printf("Balance: %s\n", balance);
            char *phone = strtok(NULL, ",");
            printf("Phone: %s\n", phone);
            char *month = strtok(NULL, "-");
            char *year = strtok(NULL, "\n");
            printf("Opened: %s %s\n\n", monthNames[atoi(month)], year);

            found = 1; // Sets the flag variable to 1 to indicate that the keyword was found
        }
        i++;
        free(temp); // Frees the memory allocated for the temporary string
        free(c);    // Frees the memory allocated for the string "c"
    }

    if (!found)
    { // If the keyword was not found
        printf("No accounts found\n");
    }
    fclose(f); // Closes the file
    //clearInputBuffer();
}

int main()
{
    printf("Welcome to the bank!\n");
    printf("1)Login\n");
    printf("2)Quit\n");
    printf("Enter your choice: ");
    while(1){
        char choice[10];
        fgets(choice, sizeof(choice), stdin);
        if (strlen(choice) > 2 || !(isdigit(choice[0])) || choice[1] != '\n')
        {
            printf("Invalid input, please try again!\n");
            continue;
        }
        int c = atoi(choice);
        if (c == 1)
        {
            login();
            break;
        }
            else if (c == 2)
            quit_login();
    }
    return 0;
}

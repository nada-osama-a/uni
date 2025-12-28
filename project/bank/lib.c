#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/*
.EDITED WITHDRAW
.added validate_amount
.bubble sort to sort by NAME
. made deletemultiple function
.added all new to lib.h
*/

// ANSI COLOR CODES
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

//GLOBALS
int loggedin = 0;
int numberOfAccounts = 0;
char* months[13] = {"","January","February","March","April","May","June","July","August","September","October","November","December"};
char* statusOfAccount[2]= {"inactive","active"};

// COLORED PRINT FUNCTIONS
void printSuccess(char* message)
{
    printf(GREEN "%s" RESET, message);
}

void printError(char* message)
{
    printf(RED "%s" RESET, message);
}

void printWarning(char* message)
{
    printf(YELLOW "%s" RESET, message);
}

void printInfo(char* message)
{
    printf(CYAN "%s" RESET, message);
}

//FUNCTIONS USED ALOT
int equal_string(char* s1,char* s2)
{
    int i = 0;
    int size1 = strlen(s1),size2 = strlen(s2);
    if(size1 == size2)
    {
        while(s1[i])
        {
            if(s1[i]==s2[i])i++;
            else return 0;
        }
        return 1;
    }

    else return 0;
}

int MatchS(char* s1, char* s2)  //STRSTR FUNCTION WHICH HANDELS CASE SENSITIVITY FOR THE ADVANCED SEARCH
{
    int i = 0;
    while (s2[i] != '\0')
    {
        if (s1[i] == '\0') return 0;
        if (tolower((unsigned char)s1[i]) == tolower((unsigned char)s2[i]))
            i++;
        else
            return 0;
    }
    return 1;
}

char* Strstr_C(char* s1, char* s2)
{
    if (!*s2) return s1;
    while (*s1)
    {
        if (MatchS(s1, s2))
            return (char*)s1;
        s1++;
    }
    return NULL;
}
int validate_email(const char *email) //validate e-mail
{

    int at_pos = -1;
    int dot_pos = -1;
    int len = strlen(email);

    for(int i = 0; i < len; i++)
    {
        if(email[i] == ' ')
            return 0;
        if(email[i] == '@')
            at_pos = i;
        if(email[i] == '.')
            dot_pos = i;
    }
    if(at_pos <= 0) return 0;                 // no @ or starts with @
    if(dot_pos <= at_pos + 1) return 0;       // no . after @
    if(dot_pos == len - 1) return 0;           // ends with .

    return 1;
}
int validate_phone(char* phone)  //validate phone num
{
    // Check length
    if(strlen(phone) != 11)
    {
        printf(RED "Phone number must be exactly 11 digits.\n" RESET);
        return 0;
    }

    // Check if all digits
    for(int i = 0; i < 11; i++)
    {
        if(!isdigit(phone[i]))
        {
            printf(RED "Phone number must contain only digits.\n" RESET);
            return 0;
        }
    }

    return 1;
}
double validate_amount(void)
{
    char input[100];
    double amount;
    int dotCount;
    int valid;
    while(getchar() != '\n');
    while (1)
    {
        printf(CYAN "Enter amount: " RESET);

        if (!fgets(input, sizeof(input), stdin))
        {
            printf(RED "Input error.\n" RESET);
            continue;
        }

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
            continue;

        dotCount = 0;
        valid = 1;

        for (int i = 0; input[i] != '\0'; i++)
        {
            if (input[i] == '.')
            {
                dotCount++;
                if (dotCount > 1)
                {
                    valid = 0;
                    break;
                }
            }
            else if (!isdigit((unsigned char)input[i]))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf(RED "Invalid number format.\n" RESET);
            continue;
        }
        amount = atof(input);
        if (amount <= 0)
        {
            printf(RED "Amount must be greater than zero.\n" RESET);
            continue;
        }
        return amount;
    }
}

//DATE STRUCT
typedef struct
{
    int day, mon, year;
} date; //NEW(added day --> needed in transactions)
date init_date(char *Date)  //INITIALIZING A DATE GIVEN BY THE FORM(MM-YYYY)
{
    date d;
    char* token = strtok(Date, "-");
    d.mon=atoi(token);
    token = strtok(NULL,"-");
    d.year = atoi(token);
    return d;
}
date localTime() //NEW(returns the date on the local computer)
{
    date d;
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    d.day = local->tm_mday;
    d.mon = local->tm_mon+1;
    d.year = local->tm_year+1900;

    return d;
}
date init_date_trans(char *Date)  //NEW (INITIALIZING A DATE GIVEN BY THE FORM(DD-MM-YYYY)for transactions)
{
    date d;
    char* token = strtok(Date, "-");
    d.day = atoi(token);
    token = strtok(NULL,"-");
    d.mon=atoi(token);
    token = strtok(NULL,"-");
    d.year = atoi(token);
    return d;
}

//USER STRUCT
typedef struct
{
    char username[64];
    char password[64];
} user;
user init_user(char* username, char* password)  //INITIALINZING A USER FOR LOGIN FUNCTION
{
    user u ;
    strcpy(u.username,username);
    strcpy(u.password,password);
    return u;
}


//ACCOUNT STRUCT
typedef struct
{

    char acc_number[20];
    char name[50];
    char address[50];
    char phone[15];
    double balance;
    double wlimit;//NEW (withdrawal limits added as a property for each account)
    date dateOpened;
    int status;
    char filetrans[20];//NEW (we found out we need a transaction for each account)
} account;
double wlim(account* acc) // (returning the withdrawal limits of an account ---> idea: adding all withdrawals in the transaction file )
{

    double wl = 0,w=0;

    char buffer[200];
    date d;
    FILE* f = fopen(acc->filetrans,"r");
    if(!f)return 50000;
    while(fgets(buffer,190,f))
    {
        char* token = strtok(buffer,",");
        w = atof(token+11);
        token = strtok(NULL,",");
        token = strtok(NULL,",");
        *(token+11) = '\0';
        d = init_date_trans(token);
        if(d.day==localTime().day && d.mon==localTime().mon && d.year==localTime().year)wl+=w;

    }
    return 50000-wl;


}
void record_transaction(account* acc, const char* transaction_type, double amount)
{

    FILE* f = fopen(acc->filetrans, "a");
    if(f)
    {
        date today = localTime();
        fprintf(f, "%s %.2f, balance = %.2f, %d-%d-%d\n",
                transaction_type, amount, acc->balance,
                today.day, today.mon, today.year);
        fclose(f);
    }
}
account* init_acc(char* num, char* name, char* address, char* balance, char* phone,char* d, char* status)       //INIT AN ACCOUNT GIVEN ALL INPUTS AS STRINGS FROM LOAD
{
    account* acc = malloc(sizeof(account));
    strcpy(acc->acc_number,num);
    acc->balance = atof(balance);
    strcpy(acc->name,name);
    strcpy(acc->address,address);
    strcpy(acc->phone,phone);
    acc->dateOpened = init_date(d);
    if(equal_string(status,"active"))acc->status = 1;
    else acc->status=0;
    strcpy(acc->filetrans,num);
    strcat(acc->filetrans,".txt");
    acc->wlimit = wlim(acc);
    return acc;
}

void print_acc(account* acc) //PRINTING INFO ACCORDING TO THE PDF FORMAT
{
    printf("\n" BOLD CYAN "=========================================\n" RESET);
    printf(CYAN "Account Number : " GREEN "%s\n" RESET, acc->acc_number);
    printf(CYAN "Name : " GREEN "%s\n" RESET, acc->name);
    printf(CYAN "E-mail : " GREEN "%s\n" RESET, acc->address);
    printf(CYAN "Balance : " GREEN "$%.2f\n" RESET, acc->balance);
    printf(CYAN "Mobile : " GREEN "%s\n" RESET, acc->phone);
    printf(CYAN "Date Opened : " GREEN "%s %d\n" RESET, months[acc->dateOpened.mon], acc->dateOpened.year);
    printf(CYAN "Status : " RESET);
    if(acc->status)
        printSuccess("active\n");
    else
        printError("inactive\n");
    printf(BOLD CYAN "=========================================\n" RESET);
}

void destruct_acc(account *acc) //DESTROYING THE ACC IF NEEDED TO BE DELETED(DELETED IN account.txt TEXT FILE TOO)
{
    FILE* f = fopen("accounts.txt","r");
    FILE* temp = fopen("temp.txt","w");
    char buffer[200];

    while(!feof(f))
    {
        fgets(buffer,190,f);
        if(!strstr(buffer,acc->acc_number))
            fputs(buffer,temp);
    }
    fclose(f);
    fclose(temp);
    remove("accounts.txt");
    rename("temp.txt","accounts.txt");
    free(acc);
}
int searchNum(account** accs)
{
    char num[20];
    int i;
    printf(YELLOW "ENTER THE NUMBER OF AN ACTIVE ACCOUNT: " RESET);
    scanf("%s",num);
    for(i = 0; i<=numberOfAccounts; i++)
    {
        if(i<numberOfAccounts)
            if(equal_string(num,accs[i]->acc_number))break;
    }
    return i;
}

//LOADING INFO OF ALL ACCOUNTS FROM THE accounts.txt FILE
account** LOAD()
{
    account** accs = malloc(100*sizeof(account*));
    char buffer[200];
    char acc_number[20];
    char name[50];
    char address[50];
    char phone[15];
    char balance[30];
    char Date[10];
    char status[10];


    if(loggedin)
    {
        FILE* f = fopen("accounts.txt","r");
        if(!f)exit(0);

        while(!feof(f))
        {
            fgets(buffer, 190, f);
            char* token = strtok(buffer,",");
            strcpy(acc_number,token);
            token = strtok(NULL,",");
            strcpy(name,token);
            token = strtok(NULL,",");
            strcpy(address,token);
            token = strtok(NULL,",");
            strcpy(balance,token);
            token = strtok(NULL,",");
            strcpy(phone,token);
            token = strtok(NULL,",");
            strcpy(Date,token);
            token = strtok(NULL,",");
            strcpy(status,token);
            if(status[strlen(status)-1]=='\n')status[strlen(status)-1] = '\0';
            accs[numberOfAccounts]=init_acc(acc_number, name,address, balance, phone, Date,status);
            numberOfAccounts++;
        }
        fclose(f);
        printSuccess("Accounts loaded successfully!\n");
        return accs;
    }
}


//LOGIN USING users.txt
void LOGIN()
{
    loggedin = 0;
    user u = init_user("NONE","NONE");

    char username[64] = "";
    char password[64] = "";

    printf("\n" BOLD BLUE "===========================================\n" RESET);
    printf(BOLD BLUE "             LOGIN PORTAL                    \n" RESET);
    printf(BOLD BLUE "===========================================\n" RESET);

    while(!loggedin)
    {
        printf(CYAN "enter the username: " RESET);
        scanf("%s",username);
        printf(CYAN "enter the password: " RESET);
        scanf("%s",password);

        FILE* f = fopen("users.txt","r");
        if(!f)exit(0);

        while(!feof(f))
        {
            fscanf(f,"%s",u.username);
            fscanf(f,"%s",u.password);
            if(equal_string(u.username,username)&&equal_string(u.password,password))
            {
                loggedin = 1;
                break;
            }
            else loggedin = 0;
        }
        fclose(f);
        if(!loggedin)
        {
            printError("INCORRECT USERNAME OR PASSWORD ! RETRY TO LOGIN\n");
            printf("\n");
        }
    }
    printSuccess("\nLogin successful! Welcome to Banking System.\n");

}

void SAVE(account** accs)
{
    char choice;
    printf(YELLOW "\nSave all changes to file? (y/n): " RESET);
    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y')
    {
        FILE* file = fopen("accounts.txt", "w");
        if(!file)
        {
            printError("Error: Cannot open accounts.txt for writing!\n");
            return;
        }

        for(int i = 0; i < numberOfAccounts; i++)
        {
            fprintf(file, "%s,%s,%s,%.2f,%s,%d-%d,%s",
                    accs[i]->acc_number,
                    accs[i]->name,
                    accs[i]->address,
                    accs[i]->balance,
                    accs[i]->phone,
                    accs[i]->dateOpened.mon,
                    accs[i]->dateOpened.year,
                    statusOfAccount[accs[i]->status]);

            if(i < numberOfAccounts - 1)
                fprintf(file, "\n");
        }

        fclose(file);
        printSuccess("All accounts saved to accounts.txt\n");
    }
    else
    {
        printWarning("Changes not saved.\n");
    }

}


void SEARCH(account** accs)
{
    char num[20];
    int i;
    printf(YELLOW "ENTER THE NUMBER OF THE ACCOUNT: " RESET);
    scanf("%s",num);
    for(i = 0; i<=numberOfAccounts; i++)
    {
        if(i<numberOfAccounts)
            if(equal_string(num,accs[i]->acc_number))break;
    }
    if(i<numberOfAccounts)
    {
        print_acc(accs[i]);
    }
    else printError("THE ACCOUNT NUMBER IS NOT FOUND!\n");

}

void ADVANCED_SEARCH(account** accs)
{
    char keyword[60];
    int found = 0;

    printf(CYAN "Enter keyword: " RESET);
    scanf("%s", keyword);
    printf("\nSearch results:\n", keyword);

    for(int i = 0; i < numberOfAccounts; i++)
    {
        if(Strstr_C(accs[i]->acc_number, keyword) != NULL ||
                Strstr_C(accs[i]->name, keyword) != NULL ||
                Strstr_C(accs[i]->address, keyword) != NULL ||
                Strstr_C(accs[i]->phone, keyword) != NULL)
        {

            print_acc(accs[i]);
            found = 1;
        }
    }

    if(!found)
    {
        printError("No matches found for keyword.\n");
    }
}

void ADD(account** accs)
{
    int i, checkNum;
    char num[20];
    char name[50];
    char address[50];
    char phone[15];
    char balance_str[20];
    double balance;
    char date_str[20];

    printf(BOLD BLUE "\nADD NEW ACCOUNT\n" RESET);
    printf(BLUE "================\n" RESET);

    do
    {
        checkNum = 0;
        printf(CYAN "Enter the account number (must be exactly 10 digits): " RESET);
        scanf("%s", num);

        // Check length
        if (strlen(num) != 10)
        {
            printError("Invalid account number! Must be exactly 10 digits.\n");
            checkNum = 1;
            continue;
        }

        // Check if all digits
        for(i = 0; i < 10; i++)
        {
            if(!isdigit(num[i]))
            {
                printError("Invalid account number! Must contain only digits.\n");
                checkNum = 1;
                break;
            }
        }
        if(checkNum) continue;

        // Check for existing account
        for(i = 0; i < numberOfAccounts; i++)
        {
            if(strcmp(num, accs[i]->acc_number) == 0)
            {
                printError("This account number already exists.\n");
                checkNum = 1;
                break;
            }
        }
    }
    while(checkNum);
    while(getchar() != '\n');

    printf(CYAN "Enter name: " RESET);
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;

    do
    {
        printf(CYAN "Enter email: " RESET);
        fgets(address, 50, stdin);
        address[strcspn(address, "\n")] = 0;

        if(!validate_email(address))
        {
            printError("Invalid email format!\n");
        }
    }
    while(!validate_email(address));

    do
    {
        printf(CYAN "Enter phone number (11 digits): " RESET);
        fgets(phone, 15, stdin);
        phone[strcspn(phone, "\n")] = 0;
    }
    while(!validate_phone(phone));

    printf(CYAN "THE BALANCE:\n" RESET);
    balance = validate_amount();
    // Convert to string
    balance_str[20];
    sprintf(balance_str, "%.2f", balance);

    date current_date = localTime();
    sprintf(date_str, "%d-%d", current_date.mon, current_date.year);

    account* new_acc = init_acc(num, name, address, balance_str, phone, date_str, "active");

    // Add to array
    accs[numberOfAccounts] = new_acc;
    numberOfAccounts++;

    printSuccess("Account added successfully!\n");
    SAVE(accs);


}

void DELETE(account** accs)
{
    printf(BOLD RED "\nDELETE ACCOUNT\n" RESET);
    printf(RED "===============\n" RESET);

    int i = searchNum(accs);
    int j;

    if(i<numberOfAccounts)
    {
        if(accs[i]->balance == 0)
        {
            destruct_acc(accs[i]);
            numberOfAccounts--;
            for(j=0; j<numberOfAccounts-i; j++)
            {
                accs[i+j]=accs[i+j+1];
            }
            printSuccess("Account deleted successfully!\n");
        }
        else printError("THE ACCOUNT CANNOT BE DELETED, BALANCE NOT ZERO\n");
    }
    else printError("THE ACCOUNT NUMBER IS NOT FOUND!\n");
}


void MODIFY(account** accs)
{
    printf(BOLD YELLOW "\nMODIFY ACCOUNT\n" RESET);
    printf(YELLOW "===============\n" RESET);

    int i = searchNum(accs);

    if(i < numberOfAccounts)
    {
        printf("\nCurrent details:\n");
        print_acc(accs[i]);

        printf("\nModify Account\n");

        // Clear input buffer
        while(getchar() != '\n');

        // Modify name
        printf(CYAN "Enter new name (or press Enter to keep current): " RESET);
        char new_name[50];
        fgets(new_name, 50, stdin);
        new_name[strcspn(new_name, "\n")] = 0;

        if(strlen(new_name) > 0)
        {
            strcpy(accs[i]->name, new_name);
        }

        // Modify phone
        char new_phone[15];
        int phone_valid = 0;
        do
        {
            printf(CYAN "Enter new phone (or press Enter to keep current): " RESET);
            fgets(new_phone, 15, stdin);
            new_phone[strcspn(new_phone, "\n")] = 0;

            if(strlen(new_phone) == 0)
            {
                phone_valid = 1;
                break;
            }

            if(validate_phone(new_phone))
            {
                strcpy(accs[i]->phone, new_phone);
                phone_valid = 1;
            }
            else
                printError("Invalid phone number!\n");
        }
        while(!phone_valid);

        // Modify email
        char new_email[50];
        int email_valid = 0;
        do
        {
            printf(CYAN "Enter new email (or press Enter to keep current): " RESET);
            fgets(new_email, 50, stdin);
            new_email[strcspn(new_email, "\n")] = 0;

            if(strlen(new_email) == 0)
            {
                email_valid = 1;
                break;
            }

            if(validate_email(new_email))
            {
                strcpy(accs[i]->address, new_email);
                email_valid = 1;
            }
            else
                printError("Invalid email format!\n");

        }
        while(!email_valid);

        printSuccess("Account modified successfully!\n");
        SAVE(accs);
    }
}

void CHANGE_STATUS(account** accs)
{
    printf(BOLD MAGENTA "\nCHANGE ACCOUNT STATUS\n" RESET);
    printf(MAGENTA "======================\n" RESET);

    int i = searchNum(accs);
    char x = 0;

    if(i<numberOfAccounts)
    {
        printf(CYAN "The account status is %s, Are you sure want to change the current status? [y/n]:" RESET, statusOfAccount[accs[i]->status]);
        getchar();
        scanf("%c",&x);
        if(x == 'y' || x == 'Y')
        {
            (accs[i]->status)=((accs[i]->status)+1)%2;
            printSuccess("Status changed successfully!\n");
            printf("New status: %s\n", statusOfAccount[accs[i]->status]);
            SAVE(accs);
        }
        else if(x == 'n' || x == 'N')
        {
            printWarning("Status change cancelled.\n");
        }
        else
        {
            printError("Invalid choice.\n");
        }

    }
    else printError("THE ACCOUNT NUMBER IS NOT FOUND!\n");

}


void DAILYLIMIT(account** accs)
{
    printf(BOLD CYAN "\nDAILY LIMIT INFORMATION\n" RESET);
    printf(CYAN "=========================\n" RESET);

    int i = searchNum(accs);

    if(i < numberOfAccounts)
    {
        printf("Account: %s\n", accs[i]->acc_number);
        printf("Name: %s\n", accs[i]->name);
        printf(GREEN "Daily withdrawal limit: $50,000.00\n" RESET);
        printf(YELLOW "Remaining daily limit: $%.2f\n" RESET, accs[i]->wlimit);
        printf(RED "Used today: $%.2f\n" RESET, 50000.0 - accs[i]->wlimit);
        printf(CYAN "Max per transaction: $10,000.00\n" RESET);
    }
    else
    {
        printError("Account not found!\n");
    }
}

void WITHDRAW(account** accs)
{
    printf(BOLD RED "\nWITHDRAW MONEY\n" RESET);
    printf(RED "===============\n" RESET);

    double withdraw;
    int i = searchNum(accs);

    if(i<numberOfAccounts)
    {
        if(accs[i]->status)
        {
            if(accs[i]->wlimit > 0)
            {
                printf(CYAN "Account balance: $%.2lf\n" RESET, accs[i]->balance);
                printf(CYAN "Amount to withdraw (max $10,000 per transaction): " RESET);
                withdraw=validate_amount();
                if(withdraw>accs[i]->balance)printError("UNABLE TO WITHDRAW! INSUFFICIENT BALANCE\n");
                else if(withdraw>accs[i]->wlimit)printError("UNABLE TO WITHDRAW! DAILY LIMIT EXCEEDED\n");
                else if(withdraw>10000.0)printError("UNABLE TO WITHDRAW! MAX PER TRANSACTION IS $10,000\n");
                else
                {
                    accs[i]->balance-=withdraw;
                    accs[i]->wlimit-=withdraw;

                    record_transaction(accs[i],"withdrawed",withdraw);
                    printSuccess("Withdrawal successful!\n");
                    printf("New balance: $%.2f\n", accs[i]->balance);
                    SAVE(accs);
                }
            }

        }
        else printError("THE ACCOUNT IS INACTIVE!\n");
    }
    else printError("THE ACCOUNT NUMBER IS NOT FOUND!\n");

}

void DEPOSIT(account** accs)
{
    printf(BOLD GREEN "\nDEPOSIT MONEY\n" RESET);
    printf(GREEN "==============\n" RESET);

    double amount;
    int i = searchNum(accs);

    if(i < numberOfAccounts)
    {
        // Check if account is active, if not, ask for another account
        while(!accs[i]->status)
        {
            printError("Cannot deposit to inactive account!\n");
            printf("Enter another account number\n");
            i = searchNum(accs);

            if(i >= numberOfAccounts)
            {
                printError("Account not found!\n");
            }
        }
        printf(CYAN "Account balance: $%.2lf\n" RESET, accs[i]->balance);
        do
        {
            printf(CYAN "Amount to deposit (max $10,000 per transaction): " RESET);
            amount=validate_amount();

            if(amount > 10000.0)
                printError("Cannot deposit more than $10,000 per transaction!\n");
        }
        while(amount > 10000.0);

        accs[i]->balance += amount;

        record_transaction(accs[i], "deposited", amount);

        printSuccess("Deposit successful!\n");
        printf("New balance: $%.2f\n", accs[i]->balance);
        SAVE(accs);
    }

}

void TRANSFER(account** accs)
{
    printf(BOLD MAGENTA "\nTRANSFER MONEY\n" RESET);
    printf(MAGENTA "===============\n" RESET);

    double amount;
    int i, j;
    int validS = 0, validR = 0;


    // Get sender account
    do
    {
        validS = 1;

        printf(CYAN "\nSENDER ACCOUNT:\n" RESET);
        i = searchNum(accs);

        if(i >= numberOfAccounts)
        {
            printError("Sender account not found!\n");
            validS = 0;
            continue;
        }

        if(!accs[i]->status)
        {
            printError("Sender account is INACTIVE!\n");
            validS = 0;
        }

        if(accs[i]->wlimit <= 0)
        {
            printError("Daily withdrawal limit reached!\n");
            printf("Remaining: $%.2f\n", accs[i]->wlimit);
            validS = 0;
        }

    }
    while(!validS);

    // Get receiver account
    do
    {
        validR = 1;
        printf(CYAN "\nRECEIVER ACCOUNT:\n" RESET);
        j = searchNum(accs);
        if(j >= numberOfAccounts)
        {
            printError("Receiver account not found!\n");
            validR = 0;
            continue;
        }
        if(!accs[j]->status)
        {
            printError("Receiver account is INACTIVE!\n");
            validR = 0;
        }
        if(i == j)
        {
            printError("Cannot transfer to same account!\n");
            validR = 0;
        }

    }
    while(!validR);

    // Show balances
    printf("\nCurrent balances:\n");
    printf("Sender (%s): $%.2f\n", accs[i]->name, accs[i]->balance);
    printf("Receiver (%s): $%.2f\n", accs[j]->name, accs[j]->balance);
    printf("Daily limit remaining: $%.2f\n", accs[i]->wlimit);

    // Get transfer amount
    printf(CYAN "\nEnter transfer amount: $" RESET);
    amount = validate_amount();

    if(amount > 10000.0)
    {
        printError("Max $10,000 per transaction!\n");
        amount = validate_amount();

    }

    else if (amount > accs[i]->balance)
    {
        printError("Insufficient balance!\n");
        printf("Available: $%.2f\n", accs[i]->balance);
        amount = validate_amount();

    }

    else if(amount > accs[i]->wlimit)
    {
        printError("Exceeds daily limit!\n");
        printf("Remaining: $%.2f\n", accs[i]->wlimit);
        amount = validate_amount();
    }

    // Perform transfer
    accs[i]->balance -= amount;
    accs[i]->wlimit -= amount;
    accs[j]->balance += amount;

    // Record transactions
    record_transaction(accs[i], "transferred(sent)", amount);
    record_transaction(accs[j], "transferred(received)", amount);

    printSuccess("Transfer successful!\n");
    printf("New balances:\n");
    printf("Sender: $%.2f\n", accs[i]->balance);
    printf("Receiver: $%.2f\n", accs[j]->balance);

    // Save automatically
    SAVE(accs);
}
// SORTING HELPER FUNCTIONS

void sortByName(account** accounts, int size)
{

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            char* name1 = accounts[j]->name;
            char* name2 = accounts[j + 1]->name;
            int k = 0;

            // Compare character by character (case-insensitive)
            while (name1[k] && name2[k])
            {
                char c1 = tolower((unsigned char)name1[k]);
                char c2 = tolower((unsigned char)name2[k]);

                if (c1 > c2)
                {
                    // Swap needed - name1 comes after name2
                    account* temp = accounts[j];
                    accounts[j] = accounts[j + 1];
                    accounts[j + 1] = temp;
                    break;
                }
                else if (c1 < c2)
                {

                    break;
                }
                k++;
            }

            // If all compared characters were equal, check lengths
            if (name1[k] == 0 && name2[k] != 0 && name1[k] != name2[k])
            {
                // name1 ended but name2 continues - no swap needed
                continue;
            }
            else if (name1[k] != 0 && name2[k] == 0)
            {
                // name2 ended but name1 continues - swap needed
                account* temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}

void sortByBalance(account** accs, int n)
{
    for(int i = 0; i < n-1; i++)
    {
        for(int j = 0; j < n-i-1; j++)
        {
            if(accs[j]->balance < accs[j+1]->balance)
            {
                account* temp = accs[j];
                accs[j] = accs[j+1];
                accs[j+1] = temp;
            }
        }
    }
}

void sortByDate(account** accs, int n)
{
    for(int i = 0; i < n-1; i++)
    {
        for(int j = 0; j < n-i-1; j++)
        {
            // Compare years, then months
            if(accs[j]->dateOpened.year > accs[j+1]->dateOpened.year ||
                    (accs[j]->dateOpened.year == accs[j+1]->dateOpened.year &&
                     accs[j]->dateOpened.mon > accs[j+1]->dateOpened.mon))
            {
                account* temp = accs[j];
                accs[j] = accs[j+1];
                accs[j+1] = temp;
            }
        }
    }
}


void PRINT(account** accs)
{
    if(numberOfAccounts == 0)
    {
        printError("No accounts to display!\n");
        return;
    }

    printf(BOLD CYAN "\n========================================\n" RESET);
    printf(BOLD CYAN "|       Sort Accounts By                |\n" RESET);
    printf(BOLD CYAN "========================================\n" RESET);
    printf(CYAN "|     1. Name (A-Z)                     |\n" RESET);
    printf(CYAN "|     2. Balance (highest to lowest)    |\n" RESET);
    printf(CYAN "|     3. Date Opened (oldest to newest) |\n" RESET);
    printf(BOLD CYAN "========================================\n" RESET);
    printf(YELLOW "      Enter choice: " RESET);

    int choice;
    scanf("%d", &choice);

    // Create temporary array for sorting
    account** temp_accs = malloc(numberOfAccounts * sizeof(account*));
    for(int i = 0; i < numberOfAccounts; i++)
    {
        temp_accs[i] = accs[i];
    }

    switch(choice)
    {
    case 1:
        sortByName(temp_accs, numberOfAccounts);
        printf(BOLD GREEN "\nAccounts Sorted by Name\n" RESET);
        break;
    case 2:
        sortByBalance(temp_accs, numberOfAccounts);
        printf(BOLD GREEN "\nAccounts Sorted by Balance\n" RESET);
        break;
    case 3:
        sortByDate(temp_accs, numberOfAccounts);
        printf(BOLD GREEN "\nAccounts Sorted by Date Opened\n" RESET);
        break;
    default:
        printError("Invalid choice!\n");
        free(temp_accs);
        return;
    }

    // Print all accounts
    for(int i = 0; i < numberOfAccounts; i++)
    {
        print_acc(temp_accs[i]);
    }

    free(temp_accs);
}


void REPORT(account** accs)
{
    printf(BOLD MAGENTA "\nTRANSACTION REPORT\n" RESET);
    printf(MAGENTA "===================\n" RESET);

    int i = searchNum(accs), j;
    char* lines[1000];
    char buffer[200];
    int count = 0;

    if(i<numberOfAccounts)
    {
        FILE* f=fopen(accs[i]->filetrans,"r");
        if(!f)printf(YELLOW "THE ACCOUNT HAS NO TRANSACTIONS!\n" RESET);

        while(fgets(buffer,190,f))
        {
            lines[count] = malloc(strlen(buffer) + 1);
            strcpy(lines[count], buffer);
            count++;
        }
        fclose(f);


        if(count>5)
        {
            printf(BOLD CYAN "\n==============================================================\n" RESET);
            printf(CYAN "|                             REPORT                          |\n" RESET);
            printf(BOLD CYAN "==============================================================\n" RESET);
            for(j=count-1; j>=count-5; j--)
            {
                printf(GREEN "- %s \n" RESET,lines[j]);
            }
            printf(BOLD CYAN "==============================================================\n" RESET);
        }
        else
        {
            printf(BOLD CYAN "\n================================================================\n" RESET);
            printf(CYAN "|                              REPORT                          |\n" RESET);
            printf(BOLD CYAN "================================================================\n" RESET);
            for(j=count-1; j>=0; j--)
            {
                printf(GREEN "- %s \n" RESET,lines[j]);
            }
            printf(BOLD CYAN "================================================================\n" RESET);
        }
    }
    else printError("THE ACCOUNT NUMBER IS NOT FOUND!\n");

}
//BONUS
void DELETEMULTIPLE(account** accs)
{
    int option;
    printf(BOLD RED "\nDELETE MULTIPLE ACCOUNTS\n" RESET);
    printf(RED "========================\n" RESET);
    printf(CYAN "1. By Date \n" RESET);
    printf(CYAN "2. Inactive Accounts (>90 days)\n" RESET);
    printf(YELLOW "Enter option: " RESET);
    scanf("%d", &option);

    while(getchar() != '\n');

    if(option == 1) // Delete by date
    {
        char date_input[8];
        int year, month;

        printf(CYAN "Enter date (MM-YYYY): " RESET);
        scanf("%7s", date_input);

        date target_date = init_date(date_input);
        year = target_date.year;
        month = target_date.mon;

        // First pass: count and store accounts to delete
        int total_count = 0;
        int has_active = 0;

        printf(YELLOW "\nAccounts to delete from %d-%02d:\n" RESET, year, month);
        printf(YELLOW "================================\n" RESET);

        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->dateOpened.year == year && accs[i]->dateOpened.mon == month)
            {
                total_count++;
                if(accs[i]->status == 1) has_active = 1;

                printf("%d. %s - %s\n", total_count, accs[i]->acc_number, accs[i]->name);
                printf("   Status: %s, Balance: $%.2f\n",
                       statusOfAccount[accs[i]->status],
                       accs[i]->balance);
                printf("   Email: %s\n", accs[i]->address);
                printf("   Date opened: %s %d\n",
                       months[accs[i]->dateOpened.mon],
                       accs[i]->dateOpened.year);
                printf(YELLOW "--------------------------------\n" RESET);
            }
        }

        if(total_count == 0)
        {
            printf(RED "No accounts found from %d-%02d\n" RESET, year, month);
            return;
        }
        // Ask for confirmation
        char confirm;
        if(has_active)
        {
            printWarning("\n WARNING: Some accounts are ACTIVE!\n");
            printf(YELLOW "Delete these %d account(s)? (y/n): " RESET, total_count);
        }
        else
            printf(YELLOW "\nDelete these %d account(s)? (y/n): " RESET, total_count);
        scanf(" %c", &confirm);

        if(confirm != 'y' && confirm != 'Y')
        {
            printWarning("Deletion cancelled\n");
            return;
        }

        // Delete accounts
        int deleted_count = 0;

        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->dateOpened.year == year && accs[i]->dateOpened.mon == month)
            {
                printf(RED "Deleting: %s - %s\n" RESET, accs[i]->acc_number, accs[i]->name);
                destruct_acc(accs[i]);
                // Shift remaining accounts
                for(int j = i; j < numberOfAccounts - 1; j++)
                    accs[j] = accs[j + 1];

                numberOfAccounts--;
                deleted_count++;
                i--;
            }
        }

        if(deleted_count > 0)
        {
            printSuccess("Deleted %d account(s)\n");
            SAVE(accs);
        }
    }
    else if(option == 2) // Delete inactive accounts
    {
        date today = localTime();
        int total_count = 0;

        printf(YELLOW "\nInactive accounts (>90 days, zero balance):\n" RESET);
        printf(YELLOW "============================================\n" RESET);

        // First pass: show accounts to delete
        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->status == 0 && accs[i]->balance == 0.0)
            {
                int days_since_opening = 0;
                int year_diff = today.year - accs[i]->dateOpened.year;
                int month_diff = today.mon - accs[i]->dateOpened.mon;
                int total_months = year_diff * 12 + month_diff;
                int approx_days = total_months * 30;

                if(approx_days > 90)
                {
                    total_count++;
                    printf("%d. %s - %s\n", total_count, accs[i]->acc_number, accs[i]->name);
                    printf("   Inactive for: ~%d days (%d months)\n", approx_days, total_months);
                    printf("   Email: %s\n", accs[i]->address);
                    printf("   Phone: %s\n", accs[i]->phone);
                    printf("   Date opened: %s %d\n",
                           months[accs[i]->dateOpened.mon],
                           accs[i]->dateOpened.year);
                    printf(YELLOW "--------------------------------\n" RESET);
                }
            }
        }

        if(total_count == 0)
        {
            printf(GREEN " No inactive accounts found (>90 days with zero balance)\n" RESET);
            return;
        }
        char confirm;
        printf(YELLOW "\nDelete these %d inactive account(s)? (y/n): " RESET, total_count);
        scanf(" %c", &confirm);

        if(confirm != 'y' && confirm != 'Y')
        {
            printWarning("Deletion cancelled\n");
            return;
        }

        // Delete accounts
        int deleted_count = 0;

        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->status == 0 && accs[i]->balance == 0.0)
            {
                // Calculate days difference again
                int year_diff = today.year - accs[i]->dateOpened.year;
                int month_diff = today.mon - accs[i]->dateOpened.mon;
                int total_months = year_diff * 12 + month_diff;
                int approx_days = total_months * 30;

                if(approx_days > 90)
                {
                    printf(RED "Deleting: %s - %s\n" RESET, accs[i]->acc_number, accs[i]->name);
                    destruct_acc(accs[i]);
                    for(int j = i; j < numberOfAccounts - 1; j++)
                        accs[j] = accs[j + 1];
                    numberOfAccounts--;
                    deleted_count++;
                    i--;
                }
            }
        }

        if(deleted_count > 0)
        {
            printSuccess("Deleted %d inactive account(s)\n");
            SAVE(accs);
        }
    }
    else
    {
        printError("Invalid option\n");
        printf(YELLOW "\nPress Enter to return to main menu..." RESET);
        getchar();
    }
}

void QUIT(account** accs)
{
    printf(BOLD BLUE "\n===========================================\n" RESET);
    printf(BLUE "            EXITING BANKING SYSTEM           \n" RESET);
    printf(BOLD BLUE "===========================================\n" RESET);
    SAVE(accs);
    printSuccess("SEE YOU LATER ...\n");
    exit(0);
}

void displayMenu()
{
    printf("\n");
    printf(BOLD BLUE " ============================================\n" RESET);
    printf(BOLD BLUE "|        BANK MANAGEMENT SYSTEM             |\n" RESET);
    printf(BOLD BLUE " ============================================\n" RESET);
    printf(CYAN "|    1.  ADD Account                        |\n" RESET);
    printf(CYAN "|    2.  DELETE Account                     |\n" RESET);
    printf(CYAN "|    3.  MODIFY Account                     |\n" RESET);
    printf(CYAN "|    4.  SEARCH Account                     |\n" RESET);
    printf(CYAN "|    5.  ADVANCED SEARCH                    |\n" RESET);
    printf(CYAN "|    6.  CHANGE STATUS                      |\n" RESET);
    printf(CYAN "|    7.  DAILY LIMIT Check                  |\n" RESET);
    printf(CYAN "|    8.  WITHDRAW                           |\n" RESET);
    printf(CYAN "|    9.  DEPOSIT                            |\n" RESET);
    printf(CYAN "|    10. TRANSFER                           |\n" RESET);
    printf(CYAN "|    11. REPORT (Last 5 transactions)       |\n" RESET);
    printf(CYAN "|    12. PRINT (Sorted accounts)            |\n" RESET);
    printf(CYAN "|    13. SAVE to file                       |\n" RESET);
    printf(CYAN "|    14. DELETE multiple                    |\n" RESET);
    printf(CYAN "|    15. QUIT                               |\n" RESET);
    printf(BOLD BLUE " ============================================\n" RESET);
    printf(YELLOW "\n Enter your choice (1-15): " RESET);
}

void MENU(account** accs)
{
    int choice;
    do
    {
        displayMenu();
        scanf("%d", &choice);

        // Clear input buffer
        while(getchar() != '\n');

        switch(choice)
        {
        case 1:
            ADD(accs);
            getchar();
            break;
        case 2:
            DELETE(accs);
            getchar();
            break;
        case 3:
            MODIFY(accs);
            getchar();
            break;
        case 4:
            SEARCH(accs);
            getchar();
            break;
        case 5:
            ADVANCED_SEARCH(accs);
            getchar();
            break;
        case 6:
            CHANGE_STATUS(accs);
            getchar();
            break;
        case 7:
            DAILYLIMIT(accs);
            getchar();
            break;
        case 8:
            WITHDRAW(accs);
            getchar();
            break;
        case 9:
            DEPOSIT(accs);
            getchar();
            break;
        case 10:
            TRANSFER(accs);
            getchar();
            break;
        case 11:
            REPORT(accs);
            getchar();
            break;
        case 12:
            PRINT(accs);
            getchar();
            break;
        case 13:
            SAVE(accs);
            getchar();
            break;
        case 14:
            DELETEMULTIPLE(accs);
            getchar();
            break;
        case 15:
            QUIT(accs);
            break;
        default:
            printError("Invalid choice! Press Enter to continue...\n");
            getchar(); // Wait for Enter
        }

        // Pause after each operation (except QUIT)
        if(choice != 15)
        {
            printf(YELLOW "\nPress Enter to return to main menu..." RESET);
            while(getchar() != '\n'); // Wait for Enter
        }

    }
    while(choice != 15);

    // Cleanup
    for(int i = 0; i < numberOfAccounts; i++)
    {
        free(accs[i]);
    }
    free(accs);
}

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


//GLOBALS
int loggedin = 0;
int numberOfAccounts = 0;
char* months[13] = {"","January","February","March","April","May","June","July","August","September","October","November","December"};
char* statusOfAccount[2]= {"inactive","active"};

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
        printf("Phone number must be exactly 11 digits.\n");
        return 0;
    }

    // Check if all digits
    for(int i = 0; i < 11; i++)
    {
        if(!isdigit(phone[i]))
        {
            printf("Phone number must contain only digits.\n");
            return 0;
        }
    }

    return 1;
}
double validate_amount()
{
    char input[100];
    char *endptr;
    double amount;

    while (1)
    {
        printf("Enter amount: $");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            continue;
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        // Handle empty input (user pressed Enter)
        if (input[0] == '\0') {
            printf("No amount entered. Please enter an amount.\n");
            continue;
        }

        // Convert to double
        amount = strtod(input, &endptr);

        // Check for conversion errors
        if (endptr == input) {
            printf("Invalid number: '%s'\n", input);
            continue;
        }

        // Check for trailing non-numeric characters
        if (*endptr != '\0') {
            printf("Invalid characters: '%s'\n", endptr);
            continue;
        }

        // Check amount range
        if (amount <= 0) {
            printf("Amount must be greater than zero.\n");
            continue;
        }
        break;
    }

    return amount;
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
        //printf("%f      %d-%d-%d\n",w,d.day,d.mon,d.year);
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
    printf("\nAccount Number : %s\nName : %s\nE-mail : %s\nBalance : %.2f\nMobile : %s\nDate Opened : %s %d\nStatus : %s\n",acc->acc_number,acc->name,acc->address,acc->balance,acc->phone,months[acc->dateOpened.mon],acc->dateOpened.year,statusOfAccount[acc->status]);
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
    printf("ENTER THE NUMBER OF AN ACTIVE ACCOUNT: ");
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
    account** accs = malloc(100*sizeof(account*));//.!!!.EDITED.!!! WAS account** accs = malloc(100*sizeof(account));
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

    while(!loggedin)
    {
        printf("enter the username: ");
        scanf("%s",username);
        printf("enter the password: ");
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
        if(!loggedin)printf("INCORRECT USERNAME OR PASSWORD ! RETRY TO LOGIN\n");
    }

}

void SAVE(account** accs)
{
    char choice;
    printf("\nSave all changes to file? (y/n): ");
    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y')
    {
        FILE* file = fopen("accounts.txt", "w");
        if(!file)
        {
            printf("Error: Cannot open accounts.txt for writing!\n");
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

            // Add newline after each account except the last one
            if(i < numberOfAccounts - 1)
                fprintf(file, "\n");
        }

        fclose(file);
        printf("\n All %d accounts saved to accounts.txt\n", numberOfAccounts);
    }
    else
        printf("Changes not saved.\n");

}


void SEARCH(account** accs)
{
    char num[20];
    int i;
    printf("ENTER THE NUMBER OF THE ACCOUNT: ");
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
    else printf("\nTHE ACCOUNT NUMBER %s IS NOT FOUND!\n",num);

}

void ADVANCED_SEARCH(account** accs)
{
    char keyword[60];
    int found = 0;

    printf("Enter keyword: ");
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
        printf("\nNo matches found for keyword: %s\n", keyword);
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

    do
    {
        checkNum = 0;
        printf("Enter the account number (must be exactly 10 digits): ");
        scanf("%s", num);

        // Check length
        if (strlen(num) != 10)
        {
            printf("Invalid account number! Must be exactly 10 digits.\n");
            checkNum = 1;
            continue;
        }

        // Check if all digits
        for(i = 0; i < 10; i++)
        {
            if(!isdigit(num[i]))
            {
                printf("Invalid account number! Must contain only digits.\n");
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
                printf("This account number already exists.\n");
                checkNum = 1;
                break;
            }
        }
    }
    while(checkNum);
    while(getchar() != '\n');

    printf("Enter name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;

    do
    {
        printf("Enter email: ");
        fgets(address, 50, stdin);
        address[strcspn(address, "\n")] = 0;

        if(!validate_email(address))
        {
            printf("Invalid email format!\n");
        }
    }
    while(!validate_email(address));

    do
    {
        printf("Enter phone number (11 digits): ");
        fgets(phone, 15, stdin);
        phone[strcspn(phone, "\n")] = 0;
    }
    while(!validate_phone(phone));

    printf("THE BALANCE:\n");
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

    SAVE(accs);


}

void DELETE(account** accs)
{
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

        }
        else printf("\nTHE ACCOUNT CANNOT BE DELETED , BALANCE = %.2f \n",accs[i]->balance);
    }
    else printf("\nTHE ACCOUNT NUMBER IS NOT FOUND!\n");
}


void MODIFY(account** accs)
{
    int i = searchNum(accs);

    if(i < numberOfAccounts)
    {
        printf("\nCurrent details:\n");
        print_acc(accs[i]);

        printf("\nModify Account\n");

        // Clear input buffer
        while(getchar() != '\n');

        // Modify name
        printf("Enter new name (or press Enter to keep current): ");
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
            printf("Enter new phone (or press Enter to keep current): ");
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
                printf("Invalid phone number!\n");
        }
        while(!phone_valid);

        // Modify email
        char new_email[50];
        int email_valid = 0;
        do
        {
            printf("Enter new email (or press Enter to keep current): ");
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
                printf("Invalid email format!\n");

        }
        while(!email_valid);

        printf("\nAccount modified in memory!\n");

        SAVE(accs);
    }
}

void CHANGE_STATUS(account** accs)
{
    int i = searchNum(accs);
    char x = 0;

    if(i<numberOfAccounts)
    {
        printf("\nThe account status is %s , Are you sure want to change the current status? [y/n]:",statusOfAccount[accs[i]->status]);
        getchar();
        scanf("%c",&x);
        if(x == 'y' || x == 'Y')
        {
            (accs[i]->status)=((accs[i]->status)+1)%2;
            printf("your status now is %s\n\n",statusOfAccount[accs[i]->status]);
            SAVE(accs);
        }
        if(x != 'y' && x != 'n' && x != 'Y' && x != 'N' )
        {
            printf("UNDEFINED CHOICE. ");
            printf("your status now is %s\n\n",statusOfAccount[accs[i]->status]);
        }

    }
    else printf("\nTHE ACCOUNT NUMBER IS NOT FOUND!\n");

}


void DAILYLIMIT(account** accs)
{
    int i = searchNum(accs);

    if(i < numberOfAccounts)
    {
        printf("\nDaily Limit Information \n");
        printf("Account: %s\n", accs[i]->acc_number);
        printf("Name: %s\n", accs[i]->name);
        printf("Daily withdrawal limit: $50,000.00\n");
        printf("Remaining daily limit: $%.2f\n", accs[i]->wlimit);
        printf("Used today: $%.2f\n", 50000.0 - accs[i]->wlimit);
        printf("Max per transaction: $10,000.00\n");
    }
    else
    {
        printf("\nAccount not found!\n");
    }
}

void WITHDRAW(account** accs)
{
    double withdraw;
    int i = searchNum(accs);

    if(i<numberOfAccounts)
    {
        if(accs[i]->status)
        {
            if(accs[i]->wlimit > 0)
            {
                printf("account balance is: %.2lf\n",accs[i]->balance);
                printf("Amount to withdraw (max $10,000 per transaction): ");
                withdraw=validate_amount();          //.!!!!EDITEDDD!!
                if(withdraw>accs[i]->balance)printf("\nUNABLE TO WITHDRAW! YOUR BALANCE IS %.2f\n",accs[i]->balance);
                else if(withdraw>accs[i]->wlimit)printf("\nUNABLE TO WITHDRAW! YOUR LIMIT FOR TODAY IS %.2f\n",accs[i]->wlimit);
                else if(withdraw>10000.0)printf("\nUNABLE TO WITHDRAW! YOU ARE LIMITED WITH 10000.0$ IN A TRANSACTION\n");
                else
                {
                    accs[i]->balance-=withdraw;
                    accs[i]->wlimit-=withdraw;

                    record_transaction(accs[i],"withdrawed",withdraw);
                    printf("withdrawal successful! New balance: $%.2f\n", accs[i]->balance);
                    SAVE(accs);
                }
            }

        }
        else printf("\nTHE ACCOUNT IS INACTIVE!\n");
    }
    else printf("\nTHE ACCOUNT NUMBER IS NOT FOUND!\n");

}

void DEPOSIT(account** accs)
{
    double amount;
    int i = searchNum(accs);

    if(i < numberOfAccounts)
    {
        // Check if account is active, if not, ask for another account
        while(!accs[i]->status)
        {
            printf("Cannot deposit to inactive account!\n");
            printf("Enter another account number\n");
            i = searchNum(accs);

            if(i >= numberOfAccounts)
            {
                printf("Account not found!\n");
            }
        }
        printf("account balance is: %.2lf\n",accs[i]->balance);
        do
        {
            printf("Amount to deposit (max $10,000 per transaction): ");
            amount=validate_amount();

            if(amount > 10000.0)
                printf("Cannot deposit more than $10,000 per transaction!\n");
        }
        while(amount > 10000.0);

        accs[i]->balance += amount;

        record_transaction(accs[i], "deposited", amount);

        printf("Deposit successful! New balance: $%.2f\n", accs[i]->balance);
        SAVE(accs);
    }

}

void TRANSFER(account** accs)
{
    double amount;
    int i, j;
    int validS = 0, validR = 0;


    // Get sender account
    do
    {
        validS = 1;

        printf("\nSENDER ACCOUNT:\n");
        i = searchNum(accs);

        if(i >= numberOfAccounts) {
            printf("Sender account not found!\n");
            validS = 0;
            continue;
        }

        if(!accs[i]->status) {
            printf("Sender account is INACTIVE!\n");
            validS = 0;
        }

        if(accs[i]->wlimit <= 0) {
            printf("Daily withdrawal limit reached!\n");
            printf("Remaining: $%.2f\n", accs[i]->wlimit);
            validS = 0;
        }

    } while(!validS);

    // Get receiver account
    do
    {
        validR = 1;
        printf("\nRECEIVER ACCOUNT:\n");
        j = searchNum(accs);
        if(j >= numberOfAccounts) {
            printf("Receiver account not found!\n");
            validR = 0;
            continue;
        }
        if(!accs[j]->status) {
            printf("Receiver account is INACTIVE!\n");
            validR = 0;
        }
        if(i == j) {
            printf("Cannot transfer to same account!\n");
            validR = 0;
        }

    } while(!validR);

    // Show balances
    printf("\nCurrent balances:\n");
    printf("Sender (%s): $%.2f\n", accs[i]->name, accs[i]->balance);
    printf("Receiver (%s): $%.2f\n", accs[j]->name, accs[j]->balance);
    printf("Daily limit remaining: $%.2f\n", accs[i]->wlimit);

    // Get transfer amount
        printf("\nEnter transfer amount: $");
        amount = validate_amount();

        if(amount > 10000.0) {
            printf("Max $10,000 per transaction!\n");
             amount = validate_amount();

        }

        else if (amount > accs[i]->balance) {
            printf("Insufficient balance! Available: $%.2f\n", accs[i]->balance);
            amount = validate_amount();

        }

        else if(amount > accs[i]->wlimit) {
            printf("Exceeds daily limit! Remaining: $%.2f\n", accs[i]->wlimit);
            amount = validate_amount();}

    // Perform transfer
    accs[i]->balance -= amount;
    accs[i]->wlimit -= amount;
    accs[j]->balance += amount;

    // Record transactions
    record_transaction(accs[i], "transferred(sent)", amount);
    record_transaction(accs[j], "transferred(received)", amount);

    printf("\nTransfer successful!\n");
    printf("New balances:\n");
    printf("Sender: $%.2f\n", accs[i]->balance);
    printf("Receiver: $%.2f\n", accs[j]->balance);

    // Save automatically
    SAVE(accs);
}
// SORTING HELPER FUNCTIONS

void sortByName(account** accounts, int size) {

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            char* name1 = accounts[j]->name;
            char* name2 = accounts[j + 1]->name;
            int k = 0;

            // Compare character by character (case-insensitive)
            while (name1[k] && name2[k]) {
                char c1 = tolower((unsigned char)name1[k]);
                char c2 = tolower((unsigned char)name2[k]);

                if (c1 > c2) {
                    // Swap needed - name1 comes after name2
                    account* temp = accounts[j];
                    accounts[j] = accounts[j + 1];
                    accounts[j + 1] = temp;
                    break;
                }
                else if (c1 < c2) {

                    break;
                }
                k++;
            }

            // If all compared characters were equal, check lengths
            if (name1[k] == 0 && name2[k] != 0 && name1[k] != name2[k]) {
                // name1 ended but name2 continues - no swap needed
                continue;
            }
            else if (name1[k] != 0 && name2[k] == 0) {
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
        printf("No accounts to display!\n");
        return;
    }
    printf("========================================\n");
    printf("|       Sort Accounts By                |\n");
    printf("========================================\n");
    printf("|     1. Name (A-Z)                     |\n");
    printf("|     2. Balance (highest to lowest)    |\n");
    printf("|     3. Date Opened (oldest to newest) |\n");
    printf("========================================\n");
    printf("      Enter choice: \n");

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
        printf("\nAccounts Sorted by Name \n");
        break;
    case 2:
        sortByBalance(temp_accs, numberOfAccounts);
        printf("\nAccounts Sorted by Balance\n");
        break;
    case 3:
        sortByDate(temp_accs, numberOfAccounts);
        printf("\nAccounts Sorted by Date Opened \n");
        break;
    default:
        printf("Invalid choice!\n");
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
    int i = searchNum(accs), j;
    char* lines[1000];
    char buffer[200];
    int count = 0;

    if(i<numberOfAccounts)
    {
        FILE* f=fopen(accs[i]->filetrans,"r");
        if(!f)printf("THE ACCOUNT HAS NO TRANSACTIONS!\n");

        while(fgets(buffer,190,f))
        {
            lines[count] = malloc(strlen(buffer) + 1);
            strcpy(lines[count], buffer);
            count++;
        }
        fclose(f);


        if(count>5)
        {
            printf("\n==============================================================\n");
            printf("|                             REPORT                          |\n");
            printf("===============================================================\n");
            for(j=count-1; j>=count-5; j--)
            {
                printf("- %s \n",lines[j]);
            }
            printf("===============================================================\n");
        }
        else
        {
            printf("\n================================================================\n");
            printf("|                              REPORT                          |\n");
            printf("================================================================\n");
            for(j=count-1; j>=0; j--)
            {
                printf("- %s \n",lines[j]);
            }
            printf("================================================================\n");
        }
    }
    else printf("\nTHE ACCOUNT NUMBER IS NOT FOUND!\n");

}
//BONUS
void DELETEMULTIPLE(account** accs)
{
    int option;
    printf("\nDELETE MULTIPLE ACCOUNTS\n");
    printf("1. By Date \n");
    printf("2. Inactive Accounts (>90 days)\n");
    printf("Enter option: ");
    scanf("%d", &option);

    while(getchar() != '\n');

    if(option == 1) // Delete by date
    {
        char date_input[8];
        int year, month;

        printf("Enter date (YYYY-MM): ");
        scanf("%7s", date_input);

        // Parse date
        char* token = strtok(date_input, "-");
        year = atoi(token);
        token = strtok(NULL, "-");
        month = atoi(token);

        // First pass: count and store accounts to delete
        int total_count = 0;
        int has_active = 0;

        printf("\nAccounts to delete from %d-%02d:\n", year, month);
        printf("================================\n");

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
                printf("--------------------------------\n");
            }
        }

        if(total_count == 0)
        {
            printf("No accounts found from %d-%02d\n", year, month);
            return;
        }

        // Ask for confirmation
        char confirm;
        if(has_active)
        {
            printf("\n  WARNING: Some accounts are ACTIVE!\n");
            printf("Delete these %d account(s)? (y/n): ", total_count);
        }
        else
        {
            printf("\nDelete these %d account(s)? (y/n): ", total_count);
        }

        scanf(" %c", &confirm);

        if(confirm != 'y' && confirm != 'Y')
        {
            printf("Deletion cancelled\n");
            return;
        }

        // Delete accounts
        int deleted_count = 0;

        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->dateOpened.year == year && accs[i]->dateOpened.mon == month)
            {
                printf("Deleting: %s - %s\n", accs[i]->acc_number, accs[i]->name);
                destruct_acc(accs[i]);

                // Shift remaining accounts
                for(int j = i; j < numberOfAccounts - 1; j++)
                {
                    accs[j] = accs[j + 1];
                }

                numberOfAccounts--;
                deleted_count++;
                i--; // Adjust index
            }
        }

        printf(" Deleted %d account(s)\n", deleted_count);
        SAVE(accs);
    }
    else if(option == 2) // Delete inactive accounts
    {
        date today = localTime();
        int total_count = 0;

        printf("\nInactive accounts (>90 days, zero balance):\n");
        printf("============================================\n");

        // First pass: show accounts to delete
        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->status == 0 && accs[i]->balance == 0)
            {
                int months_since = (today.year - accs[i]->dateOpened.year) * 12
                                 + (today.mon - accs[i]->dateOpened.mon);

                if(months_since >= 3)
                {
                    total_count++;
                    printf("%d. %s - %s\n", total_count, accs[i]->acc_number, accs[i]->name);
                    printf("   Inactive for: ~%d months\n", months_since);
                    printf("   Email: %s\n", accs[i]->address);
                    printf("   Phone: %s\n", accs[i]->phone);
                    printf("   Date opened: %s %d\n",
                           months[accs[i]->dateOpened.mon],
                           accs[i]->dateOpened.year);
                    printf("--------------------------------\n");
                }
            }
        }

        if(total_count == 0)
        {
            printf("No inactive accounts found (>90 days with zero balance)\n");
            return;
        }

        // Ask for confirmation
        char confirm;
        printf("\nDelete these %d inactive account(s)? (y/n): ", total_count);
        scanf(" %c", &confirm);

        if(confirm != 'y' && confirm != 'Y')
        {
            printf("Deletion cancelled\n");
            return;
        }

        // Delete accounts
        int deleted_count = 0;

        for(int i = 0; i < numberOfAccounts; i++)
        {
            if(accs[i]->status == 0 && accs[i]->balance == 0)
            {
                int months_since = (today.year - accs[i]->dateOpened.year) * 12
                                 + (today.mon - accs[i]->dateOpened.mon);

                if(months_since >= 3)
                {
                    printf("Deleting: %s - %s\n", accs[i]->acc_number, accs[i]->name);
                    destruct_acc(accs[i]);

                    // Shift remaining accounts
                    for(int j = i; j < numberOfAccounts - 1; j++)

                        accs[j] = accs[j + 1];

                    numberOfAccounts--;
                    deleted_count++;
                    i--; // Adjust index
                }
            }
        }

        printf("Deleted %d inactive account(s)\n", deleted_count);
        SAVE(accs);
    }
    else
    {
        printf("Invalid option\n");
        printf("\nPress Enter to return to main menu...");
    }
}
void QUIT(account** accs)
{
    SAVE(accs);
    printf("SEE YOU LATER ...\n");
    exit(0);
}

void displayMenu()
{
    printf("\n");
    printf(" ============================================\n");
    printf("|        BANK MANAGEMENT SYSTEM             |\n");
    printf(" ============================================\n");
    printf("|    1.  ADD Account                        |\n");
    printf("|    2.  DELETE Account                     |\n");
    printf("|    3.  MODIFY Account                     |\n");
    printf("|    4.  SEARCH Account                     |\n");
    printf("|    5.  ADVANCED SEARCH                    |\n");
    printf("|    6.  CHANGE STATUS                      |\n");
    printf("|    7.  DAILY LIMIT Check                  |\n");
    printf("|    8.  WITHDRAW                           |\n");
    printf("|    9.  DEPOSIT                            |\n");
    printf("|    10. TRANSFER                           |\n");
    printf("|    11. REPORT (Last 5 transactions)       |\n");
    printf("|    12. PRINT (Sorted accounts)            |\n");
    printf("|    13. SAVE to file                       |\n");
    printf("|    14. DELETE multiple                     |\n");
    printf("|    15. QUIT                               |\n");
    printf(" ============================================\n");
    printf("\n Enter your choice (1-15): ");
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
            printf("Invalid choice! Press Enter to continue...\n");
            getchar(); // Wait for Enter
        }

        // Pause after each operation (except QUIT)
        if(choice != 15)
        {
            printf("\nPress Enter to return to main menu...");
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

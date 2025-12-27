#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

int equal_string(char* s1 ,char* s2);
int MatchS(char* s1, char* s2);
char* Strstr_C(char* s1, char* s2);
int validate_email(const char *email);
int validate_phone(char* phone);
double validate_amount();
typedef struct{int day, mon , year;}date;
date init_date(char *Date);
date localTime();
date init_date_trans(char *Date);
typedef struct{
    char username[64];
    char password[64];
}user;
user init_user(char* username , char* password);
typedef struct{

    char acc_number[20];
    char name[50];
    char address[50];
    char phone[15];
    double balance;
    double wlimit;//NEW (withdrawal limits added as a property for each account)
    date dateOpened;
    int status;
    char filetrans[20];//NEW (we found out we need a transaction for each account)
}account;
double wlim(account* acc);
void record_transaction(account* acc, const char* transaction_type, double amount);

account* init_acc(char* num , char* name , char* address , char* balance, char* phone ,char* d , char* status);
void print_acc(account* acc);
void destruct_acc(account *acc);
int searchNum(account** accs);
account** LOAD();
void LOGIN();
void SEARCH(account** accs);
void SAVE(account** accs);
void DELETE(account** accs);
void CHANGE_STATUS(account** accs);
void ADVANCED_SEARCH(account** accs);
void ADD(account** accs);
void MODIFY(account** accs);
void sortByName(account** accs, int n);
void sortByBalance(account** accs, int n);
void sortByDate(account** accs, int n);
void PRINT(account** accs);
void DAILYLIMIT(account** accs);
void WITHDRAW(account** accs);
void DEPOSIT(account** accs);
void TRANSFER(account** accs);
void REPORT(account** accs);
void QUIT(account** accs);
void displayMenu();
void MENU(account** accs);
void DELETEMULTIPLE(account** accs);



#endif // LIB_H_INCLUDED

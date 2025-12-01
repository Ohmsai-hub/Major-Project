#ifndef BANK_H
#define BANK_H

#include "utils.h"

#define FILE_NAME "bank.txt"
#define MAX_USERS 200
#define MAX_TX 200
#define MAX_NAME 64
#define MAX_EMAIL 64
#define MAX_PHONE 24
#define MAX_PASS 64
#define MAX_TYPE 16

typedef struct {
    time_t ts;
    char type[MAX_TYPE];
    double amount;
    double balance_after;
} Transaction;

typedef struct {
    int id;
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    char password[MAX_PASS];
    double balance;
    int tx_count;
    Transaction tx[MAX_TX];
} Account;

/* Global storage (declared in bank.c) */
extern Account users[MAX_USERS];
extern int user_count;

/* Bank operations */
int find_user_by_email(const char *email);
void add_transaction(Account *a, const char *type, double amount);

void register_user(void);
int login_user(void);

void deposit(Account *a);
void withdraw(Account *a);
void view_balance(const Account *a);
void view_profile(const Account *a);
void view_transactions(const Account *a);

void account_menu(Account *a);
void main_menu(void);

#endif /* BANK_H */


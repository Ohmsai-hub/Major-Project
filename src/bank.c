#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bank.h"
#include "file_ops.h"
#include "utils.h"

/* Globals */
Account users[MAX_USERS];
int user_count = 0;

/* Find user index by email, -1 if not found */
int find_user_by_email(const char *email) {
    for (int i = 0; i < user_count; ++i) {
        if (strcmp(users[i].email, email) == 0) return i;
    }
    return -1;
}

void add_transaction(Account *a, const char *type, double amount) {
    if (!a) return;
    if (a->tx_count >= MAX_TX) {
        /* simple rollover: drop oldest */
        memmove(&a->tx[0], &a->tx[1], sizeof(Transaction) * (MAX_TX - 1));
        a->tx_count = MAX_TX - 1;
    }
    Transaction *t = &a->tx[a->tx_count++];
    t->ts = time(NULL);
    strncpy(t->type, type, sizeof(t->type) - 1);
    t->type[sizeof(t->type)-1] = '\0';
    t->amount = amount;
    t->balance_after = a->balance;
}

/* Register a new user */
void register_user(void) {
    if (user_count >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    Account a;
    memset(&a, 0, sizeof(a));
    a.id = user_count + 1;
    a.balance = 0.0;
    a.tx_count = 0;

    read_line("Enter name: ", a.name, sizeof(a.name));
    read_line("Enter email: ", a.email, sizeof(a.email));
    if (find_user_by_email(a.email) != -1) {
        printf("Email already registered.\n");
        return;
    }
    read_line("Enter phone: ", a.phone, sizeof(a.phone));
    read_line("Set password: ", a.password, sizeof(a.password));

    users[user_count++] = a;
    save_db();
    printf("Registration successful! Your ID: %d\n", a.id);
}

/* Login returns index of user on success, -1 on failure */
int login_user(void) {
    char email[MAX_EMAIL];
    char password[MAX_PASS];
    read_line("Enter email: ", email, sizeof(email));
    int idx = find_user_by_email(email);
    if (idx == -1) {
        printf("No such user.\n");
        return -1;
    }
    read_line("Enter password: ", password, sizeof(password));
    if (strcmp(users[idx].password, password) == 0) {
        printf("Login successful! Welcome %s.\n", users[idx].name);
        return idx;
    } else {
        printf("Incorrect password.\n");
        return -1;
    }
}

/* Deposit */
void deposit(Account *a) {
    double amt = read_double("Enter amount to deposit: ");
    if (amt <= 0.0) {
        printf("Amount must be positive.\n");
        return;
    }
    a->balance += amt;
    add_transaction(a, "DEPOSIT", amt);
    save_db();
    printf("Deposited %.2f. New balance: %.2f\n", amt, a->balance);
}

/* Withdraw */
void withdraw(Account *a) {
    double amt = read_double("Enter amount to withdraw: ");
    if (amt <= 0.0) {
        printf("Amount must be positive.\n");
        return;
    }
    if (amt > a->balance) {
        printf("Insufficient balance.\n");
        return;
    }
    a->balance -= amt;
    add_transaction(a, "WITHDRAW", amt);
    save_db();
    printf("Withdrawn %.2f. New balance: %.2f\n", amt, a->balance);
}

void view_balance(const Account *a) {
    printf("Current balance: %.2f\n", a->balance);
}

void view_profile(const Account *a) {
    printf("\n--- Profile ---\n");
    printf("ID: %d\nName: %s\nEmail: %s\nPhone: %s\nBalance: %.2f\n",
           a->id, a->name, a->email, a->phone, a->balance);
}

void view_transactions(const Account *a) {
    printf("\n--- Transactions (%d) ---\n", a->tx_count);
    printf("%-20s  %-10s  %-12s  %-12s\n", "Timestamp", "Type", "Amount", "Balance");
    for (int i = 0; i < a->tx_count; ++i) {
        const Transaction *t = &a->tx[i];
        print_time(t->ts);
        printf("  %-10s  %-12.2f  %-12.2f\n", t->type, t->amount, t->balance_after);
    }
}

/* Account menu after login */
void account_menu(Account *a) {
    char choice[8];
    while (1) {
        printf("\n== Account Menu (%s) ==\n", a->email);
        printf("1) View Profile\n2) View Balance\n3) Deposit\n4) Withdraw\n5) Transactions\n6) Logout\n");
        read_line("Choose: ", choice, sizeof(choice));
        switch (choice[0]) {
            case '1': view_profile(a); break;
            case '2': view_balance(a); break;
            case '3': deposit(a); break;
            case '4': withdraw(a); break;
            case '5': view_transactions(a); break;
            case '6': return;
            default: printf("Invalid choice.\n"); break;
        }
    }
}

/* Main menu */
void main_menu(void) {
    char choice[8];
    while (1) {
        printf("\n== Main Menu ==\n");
        printf("1) Register\n2) Login\n3) Exit\n");
        read_line("Choose: ", choice, sizeof(choice));
        switch (choice[0]) {
            case '1': register_user(); break;
            case '2': {
                int idx = login_user();
                if (idx >= 0) account_menu(&users[idx]);
            } break;
            case '3': printf("Exiting...\n"); return;
            default: printf("Invalid choice.\n"); break;
        }
    }
}


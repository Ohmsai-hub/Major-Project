#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bank.h"

/* Save DB to text file in human readable format */
void save_db(void) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        perror("Error saving file");
        return;
    }

    fprintf(fp, "%d\n", user_count);
    for (int i = 0; i < user_count; ++i) {
        Account *a = &users[i];
        fprintf(fp, "ID:%d\n", a->id);
        fprintf(fp, "Name:%s\n", a->name);
        fprintf(fp, "Email:%s\n", a->email);
        fprintf(fp, "Phone:%s\n", a->phone);
        fprintf(fp, "Password:%s\n", a->password);
        fprintf(fp, "Balance:%.2f\n", a->balance);
        fprintf(fp, "Transactions:%d\n", a->tx_count);
        for (int j = 0; j < a->tx_count; ++j) {
            Transaction *t = &a->tx[j];
            fprintf(fp, "%ld,%s,%.2f,%.2f\n", (long)t->ts, t->type, t->amount, t->balance_after);
        }
        fprintf(fp, "END\n");
    }

    fclose(fp);
}

/* Load DB from text file */
void load_db(void) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return; /* file not present is ok */

    if (fscanf(fp, "%d\n", &user_count) != 1) {
        user_count = 0;
        fclose(fp);
        return;
    }

    for (int i = 0; i < user_count; ++i) {
        Account *a = &users[i];
        char line[512];
        memset(a, 0, sizeof(*a));
        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "ID:%d\n", &a->id);

        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "Name:%[^\n]\n", a->name);

        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "Email:%[^\n]\n", a->email);

        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "Phone:%[^\n]\n", a->phone);

        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "Password:%[^\n]\n", a->password);

        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "Balance:%lf\n", &a->balance);

        if (fgets(line, sizeof(line), fp) == NULL) { user_count = i; break; }
        sscanf(line, "Transactions:%d\n", &a->tx_count);

        for (int j = 0; j < a->tx_count; ++j) {
            if (fgets(line, sizeof(line), fp) == NULL) { a->tx_count = j; break; }
            long ts;
            char type[32];
            double amt, bal;
            if (sscanf(line, "%ld,%31[^,],%lf,%lf\n", &ts, type, &amt, &bal) == 4) {
                a->tx[j].ts = (time_t)ts;
                strncpy(a->tx[j].type, type, sizeof(a->tx[j].type)-1);
                a->tx[j].amount = amt;
                a->tx[j].balance_after = bal;
            } else {
                a->tx_count = j;
                break;
            }
        }

        /* read END line */
        fgets(line, sizeof(line), fp);
    }

    fclose(fp);
}


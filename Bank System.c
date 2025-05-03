#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#include <time.h>

//functions prototypes
int findAccountIndex(char acc_number[11]);

int Save();

char *Month_name(int month_number);   //function to turn month number to month name
//useful for query and advanced search
typedef struct
{
    int month;
    int year;
} Date;

typedef struct
{
    char account_number[11];
    char name[50];
    char mobile_number[12]; //change
    char email_address[50];
    double balance;
    Date date_opened;
} Account;

Account account_data[1000];//global
int numAccounts;//global
typedef struct
{
    char username[50];
    char password[50];
} User;

void login()
{
    char username[50];
    char password[50];
    char buffer[100];
    int num_users = 0, i, flag = 0;
    FILE *f;
    f = fopen("users.txt", "r");
    if (f == NULL)
    {
        printf("Error opening users file");
        exit(2);
    }
    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        num_users++;
    }
    User *users = (User *) malloc(num_users * sizeof(User));// more efficient in memory usage
    rewind(f);

    printf("\n======= Login =======\n\n");

    do
    {
        printf("Enter username: ");
        fflush(stdin);
        fgets(username, sizeof(username), stdin);
        username[strcspn(username,"\n")] = '\0';//have to use it with fgets to remove the \n as fgets avoids buffer overflow
        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';
        for (i = 0; i < num_users; i++)
        {
            fscanf(f, "%s%s\n", users[i].username, users[i].password);
            if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
            {
                printf("Login Successful\n");
                flag = 1;
                break;
            }
            else if (i == num_users - 1)
                printf("Invalid username or password\n\n");
        }
    }
    while (flag != 1);
    free(users);
    fclose(f);
}

void LoadAccountsFromFile()
{
    numAccounts = 0;
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL)
    {
        printf("File not found.\n");
        exit(1);
    }

    while (fscanf(file, "%[^,],%[^,],%[^,],%lf,%[^,],%d-%d\n",
                  account_data[numAccounts].account_number,
                  account_data[numAccounts].name,
                  account_data[numAccounts].email_address,
                  &account_data[numAccounts].balance,
                  account_data[numAccounts].mobile_number,
                  &account_data[numAccounts].date_opened.month,
                  &account_data[numAccounts].date_opened.year) != EOF)
    {
        numAccounts++;
    }

    fclose(file);
}

void displayAccounts()
{
    for (int i = 0; i < numAccounts; i++)
    {
        printf("Account Number: %s\n", account_data[i].account_number);
        printf("Name: %s\n", account_data[i].name);
        printf("Email: %s\n", account_data[i].email_address);
        printf("Balance: %.2lf\n", account_data[i].balance);
        printf("Mobile: %s\n", account_data[i].mobile_number);
        printf("Date Opened: %d-%d\n\n", account_data[i].date_opened.month, account_data[i].date_opened.year);
    }
}

void reverseddisplayAccounts()
{
    for (int i = numAccounts - 1; i >= 0; i--)
    {
        printf("Account Number: %s\n", account_data[i].account_number);
        printf("Name: %s\n", account_data[i].name);
        printf("Email: %s\n", account_data[i].email_address);
        printf("Balance: %.2lf\n", account_data[i].balance);
        printf("Mobile: %s\n", account_data[i].mobile_number);
        printf("Date Opened: %d-%d\n\n", account_data[i].date_opened.month, account_data[i].date_opened.year);
    }
}


int is_valid_acc_num(char *account_number)
{
    int i;
    if (strlen(account_number) != 10)
    {
        return 0;
    }
    for (i = 0; i < 10; i++)
    {
        if (!isdigit(account_number[i]))
        {
            return 0;
        }
    }
    return 1;
}

int is_valid_mob_num(char *mob_num)
{
    int i;
    if (strlen(mob_num) != 11)
    {
        return 0;
    }
    for (i = 0; i < 11; i++)
    {
        if (!isdigit(mob_num[i]))
        {
            return 0;
        }
    }
    return 1;
}

char *validate_name(char *name)
{
    char first_name[50];
    char second_name[25];
    char delim[] = " ";
    int i = 1;
    strcpy(first_name, strtok(name, delim));
    strcpy(second_name, strtok(NULL, delim));
    first_name[0] = toupper(first_name[0]);
    second_name[0] = toupper(second_name[0]);
    while (first_name[i])
    {
        first_name[i] = tolower(first_name[i]);
        i++;
    }
    i = 1;
    while (second_name[i])
    {
        second_name[i] = tolower(second_name[i]);
        i++;
    }
    strcat(first_name, " ");
    strcat(first_name, second_name);
    strcpy(name, first_name);
    return name;
}

int is_valid_email(char *email)
{
    int i, j, flag = 0;
    if(email[0] == '@')
        return 0;
    for (i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            flag++;
            j = i;
        }
        else if (email[i] == ',')
            return 0;
    }
    if (flag != 1 || email[j + 1] == '.' || email[j + 1] == '\0')
        return 0;
    j += 2;
    for (; email[j] != '\0'; j++)
    {
        if (email[j] == '.')
        {
            if (email[j + 1] != '\0')
                return 1;
            else
                return 0;
        }
    }
    return 0;
}

void getMonthAndYear(int *month, int *year)
{
    time_t t;
    struct tm *current_time;

    // Get current time
    time(&t);
    current_time = localtime(&t);

    // Extract month and year
    *month = current_time->tm_mon + 1;  // Months are 0-indexed
    *year = current_time->tm_year + 1900;  // Years since 1900
}

void query_search()
{
    int i;
    char acc_num[11]; //is the string that will read from the user account number
    printf("\n\n======= Search =======\n\n");

    while (1)
    {
        printf("Enter an account number: ");
        fflush(stdin);
        gets(acc_num);
        if (is_valid_acc_num(acc_num) == 0)
        {
            printf("Invalid account number\n\n");
            continue;
        }
        for (i = 0; i < numAccounts; i++)
        {
            if (strcmp(account_data[i].account_number, acc_num) == 0)
            {
                printf("Account Number: %s\n", account_data[i].account_number);
                printf("Name: %s\n", account_data[i].name);
                printf("E-mail: %s\n", account_data[i].email_address);
                printf("Balance: %.2lf\n", account_data[i].balance);
                printf("Mobile: %s\n", account_data[i].mobile_number);
                printf("Opened : %s %d\n\n", Month_name(account_data[i].date_opened.month), account_data[i].date_opened.year);

                break;
            }
        }
        if (i == numAccounts)
            printf("The account you have entered is not found\n");
        break;
    }
}

void add_account()
{
    int i, flag;
    printf("\n======= Add =======\n\n");
    while (1)
    {
        flag = 1;
        printf("Enter a new account number: ");
        fflush(stdin);
        gets(account_data[numAccounts].account_number);
        if (is_valid_acc_num(account_data[numAccounts].account_number) == 0)
        {
            printf("Account number is invalid\n\n");
            continue;
        }
        for (i = 0; i < numAccounts; i++)
        {
            if (strcmp(account_data[numAccounts].account_number, account_data[i].account_number) == 0)
            {
                printf("This account number already exists\n\n");
                flag = 0;
                break;
            }
        }
        if (flag)
            break;
    }
    do
    {
        flag = 1;
        printf("Please enter the name (first and last names): ");
        fflush(stdin);//to avoid multiple output prints if you spam letters
        fgets(account_data[numAccounts].name, 50, stdin);
        account_data[numAccounts].name[strcspn(account_data[numAccounts].name, "\n")] = '\0';
        if (account_data[numAccounts].name[0] == '\0')
        {
            printf("Name is invalid\n\n");
            flag = 0;
            continue;
        }
        i = 0;
        while (account_data[numAccounts].name[i])
        {
            if (!isalpha(account_data[numAccounts].name[i]) && !isspace(account_data[numAccounts].name[i]))
            {
                printf("Name is invalid\n\n");
                flag = 0;
                break;
            }
            i++;
        }
        if (!flag)
            continue;
        i = 0;
        int check_num_names = 0;
        while (account_data[numAccounts].name[i])
        {
            if (account_data[numAccounts].name[i] == ' ' && isalpha(account_data[numAccounts].name[i + 1]))
            {
                check_num_names++;
            }
            i++;
        }
        i = 0;
        int check_true = 0;
        while (account_data[numAccounts].name[i])
        {
            if (isalpha(account_data[numAccounts].name[0]))
            {
                check_true = 1;
            }
            i++;
        }
        if (check_num_names == 2)
            flag = 1;
        else if (check_num_names == 1 && check_true == 1)
            flag = 1;
        else
            flag = 0;
        if (flag)
            validate_name(account_data[numAccounts].name);
        else
            printf("Name is invalid\n\n");
    }
    while (!flag);
    do
    {
        flag = 1;
        printf("Enter an Email address: ");
        fflush(stdin);
        fgets(account_data[numAccounts].email_address, 50, stdin);
        account_data[numAccounts].email_address[strcspn(account_data[numAccounts].email_address, "\n")] = '\0';
        if (!(is_valid_email(account_data[numAccounts].email_address)))
        {
            printf("Invalid Email address\n\n");
            flag = 0;
        }
    }
    while (!flag);
    account_data[numAccounts].balance = 0;
    while (1)
    {
        printf("Enter a mobile number: ");
        fflush(stdin);
        gets(account_data[numAccounts].mobile_number);
        if (is_valid_mob_num(account_data[numAccounts].mobile_number) == 0)
        {
            printf("Invalid mobile number\n\n");
            continue;
        }
        break;
    }
    getMonthAndYear(&account_data[numAccounts].date_opened.month, &account_data[numAccounts].date_opened.year);
    numAccounts++;
    //1 means changes are saved
    if (Save() == 1)
    {
        printf("Account added.\n");
    }
    //0 means changes are not saved ,hence returns number of accounts back to before addition
    else
    {
        printf("Account is not added.\n");
    }
}
void delete_account()
{
    int i, flag, found_index=0;
    char acc_num[11];
    printf("\n\n======= Delete =======\n\n");

    while (1)
    {
        printf("Enter an account number to be deleted: ");
        fflush(stdin);
        gets(acc_num);
        if (is_valid_acc_num(acc_num) == 0)
        {
            printf("Account number is invalid\n\n");
            continue;
        }
        for (i = 0; i < numAccounts; i++)
        {
            flag = 0;
            if (strcmp(acc_num, account_data[i].account_number) == 0)
            {
                flag = 1;
                found_index = i;
                break;
            }
        }
        if (!flag)
        {
            printf("Account not found\n\n");
            return;
        }
        if (account_data[found_index].balance != 0)
        {
            printf("Cannot delete account.\n");
            printf("Balance of account is not empty.\n");
            return;
        }


        strcpy(account_data[found_index].account_number, account_data[numAccounts - 1].account_number);
        strcpy(account_data[found_index].name, account_data[numAccounts - 1].name);
        strcpy(account_data[found_index].email_address, account_data[numAccounts - 1].email_address);
        account_data[found_index].balance = account_data[numAccounts - 1].balance;
        strcpy(account_data[found_index].mobile_number, account_data[numAccounts - 1].mobile_number);
        account_data[found_index].date_opened.month = account_data[numAccounts - 1].date_opened.month;
        account_data[found_index].date_opened.year = account_data[numAccounts - 1].date_opened.year;
        numAccounts--;
        if (Save() == 1)
        {
            printf("Account has been deleted.\n");
        }
        else
        {
            printf("Account deletion canceled.\n");
        }
        break;

    }
}
void modify_account()
{
    int i, flag, found_index;
    char acc_num[11];

    printf("\n\n======= Modify =======\n\n");

    while (1)
    {
        printf("Enter an account number: ");
        fflush(stdin);
        gets(acc_num);
        if (is_valid_acc_num(acc_num) == 0)
        {
            printf("Account number is invalid\n\n");
            continue;
        }
        for (i = 0; i < numAccounts; i++)
        {
            flag = 0;
            if (strcmp(acc_num, account_data[i].account_number) == 0)
            {
                flag = 1;
                found_index = i;
                break;
            }
        }
        if (!flag)
        {
            printf("Account not found\n\n");
            continue;
        }
        else
        {
            do
            {
                flag = 1;
                printf("Please enter a new name (first and last names): ");
                fflush(stdin);//to avoid multiple output prints if you spam letters
                fgets(account_data[found_index].name, 50, stdin);
                account_data[found_index].name[strcspn(account_data[found_index].name, "\n")] = '\0';
                if (account_data[found_index].name[0] == '\0')
                {
                    printf("Name is invalid\n\n");
                    flag = 0;
                    continue;
                }
                i = 0;
                while (account_data[found_index].name[i])
                {
                    if (!isalpha(account_data[found_index].name[i]) && !isspace(account_data[found_index].name[i]))
                    {
                        printf("Name is invalid\n\n");
                        flag = 0;
                        break;
                    }
                    i++;
                }
                if (!flag)
                    continue;
                i = 0;
                int check_num_names = 0;
                while (account_data[found_index].name[i])
                {
                    if (account_data[found_index].name[i] == ' ' && isalpha(account_data[found_index].name[i + 1]))
                    {
                        check_num_names++;
                    }
                    i++;
                }
                i = 0;
                int check_true = 0;
                while (account_data[found_index].name[i])
                {
                    if (isalpha(account_data[found_index].name[0]))
                    {
                        check_true = 1;
                    }
                    i++;
                }
                if (check_num_names == 2)
                    flag = 1;
                else if (check_num_names == 1 && check_true == 1)
                    flag = 1;
                else
                    flag = 0;
                if (flag)
                    validate_name(account_data[found_index].name);
                else
                    printf("Name is invalid\n\n");
            }
            while (!flag);
            while (1)
            {
                printf("Enter a new mobile number: ");
                fflush(stdin);
                gets(account_data[found_index].mobile_number);
                if (is_valid_mob_num(account_data[found_index].mobile_number) == 0)
                {
                    printf("Invalid mobile number\n\n");
                    continue;
                }
                break;
            }
            do
            {
                flag = 1;
                printf("Enter a new Email address: ");
                fflush(stdin);
                fgets(account_data[found_index].email_address, 50, stdin);
                account_data[found_index].email_address[strcspn(account_data[found_index].email_address, "\n")] = '\0';
                if (!(is_valid_email(account_data[found_index].email_address)))
                {
                    printf("Invalid Email address\n\n");
                    flag = 0;
                }
            }
            while (!flag);
        }
        break;
    }
    if (Save() == 1)
    {
        printf("Account has been successfully modified.\n");
    }
    else
    {
        printf("Account modification has been canceled.\n");
    }
}
char *Month_name(int month_number)
{
    char *months[] = {"January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December"
                     };
    if (month_number >= 1 && month_number <= 12) return months[month_number - 1];
    else return "wrong month";
}

int is_invalid_name(char *name)     // invalid = 1 // valid = 0
{
    int flag = 0;
    int len = strlen(name);

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(name[i]) && !isspace(name[i]))
        {
            flag++;
        }
    }
    return flag;
}

void Advanced_Search()
{
    char keyword[50];
    int count=0;
    int z =0;
    printf("\n\n======= Advanced Search =======\n\n");

    do
    {
        fflush(stdin);
        printf("Enter a keyword: ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword,"\n")] = '\0';//have to use it with fgets to remove the \n as fgets avoids buffer overflow
    }
    while (is_invalid_name(keyword));

    printf("Search results:\n");
    printf("-----------------------------\n");
    int i = 0, j = 0,flag =0;

    while (z < numAccounts)
    {
        for(j=0; j<strlen(keyword); j++)
        {
            if(account_data[z].name[i+j]==toupper(keyword[j])||account_data[z].name[i+j]==tolower(keyword[j]))
            {
                count++;
            }
            else count=0;
        }
        if(count==strlen(keyword))
        {
            flag = 1;
            printf("Account Number: %s\n", account_data[z].account_number);
            printf("Name: %s\n", account_data[z].name);
            printf("E-mail: %s\n", account_data[z].email_address);
            printf("Balance: %.2lf\n", account_data[z].balance);
            printf("Mobile: %s\n", account_data[z].mobile_number);
            printf("Opened : %s %d\n\n", Month_name(account_data[z].date_opened.month), account_data[z].date_opened.year);
            i=0;
            z++;
        }
        if(account_data[z].name[i+strlen(keyword)]=='\0')
        {
            i=0;
            count=0;
            z++;
        }
        else i++;
    }
    if(!flag) printf("No matches are found.\n\n");
}

void account_withdraw()
{
    double amount;
    char account_number[11];
    int flag = 0;
    printf("\n\n======= Withdraw =======\n\n");

    while (flag == 0)
    {
        printf("Enter the account number :");
        scanf("%s", account_number);

        if (is_valid_acc_num(account_number) == 0)
        {
            printf("Invalid Account number! Please enter 10 digits!\n");
            flag = 0;
        }
        else
        {
            flag = 1; // Assign 1 to flag to exit the loop
        }
    }

    int index_of_account_number = findAccountIndex(account_number);
    if (index_of_account_number == -1)
    {
        printf("Account number not found\n");
        return;
    }

    int valid_amount_flag = 0;

    while (valid_amount_flag == 0)
    {
        printf("Enter amount to withdraw: ");
        if (scanf("%lf", &amount) != 1 || getchar() != '\n')
        {
            printf("Invalid input! Please enter a valid amount (numeric).\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else
        {
            valid_amount_flag = 1;
        }
    }

    double maximum_withdraw = 10000.0;

    if (amount > maximum_withdraw)
    {
        printf("the withdraw amount is above the maximum limit of%0.2lf\n",
               maximum_withdraw); //withdraw process failed here as it is above the (maximum_withdraw).
        return;
    }
    else if (amount <= 0)
    {
        printf("invalid withdraw amount\n"); //withdraw process failed.
        return;
    }
    else if (amount > account_data[index_of_account_number].balance)
    {
        printf("insufficient quantity\n");  //withdraw process failed.
        return;
    }
    if (valid_amount_flag)
    {
        (account_data[index_of_account_number].balance) -= amount;
        if (Save() == 1)   //change
        {
            printf("------------------------------------------------\n");
            printf("\nWithdraw process Successfully\n");
            // Create files for both sender and receiver
            char withdraw_file_name[15]; // Account file name with ".txt" extension
            sprintf(withdraw_file_name, "%s.txt", account_number); // Append ".txt" to the sender's account number
            FILE *withdraw_file = fopen(withdraw_file_name, "a");
            if (withdraw_file == NULL)
            {
                printf("Error opening account file for transaction record.\n");
                if (withdraw_file != NULL) fclose(withdraw_file);
                return;
            }
            fprintf(withdraw_file, "withdraw transaction -%.2lf$ \n", amount);
            fclose(withdraw_file);
            // printf("\nUpdated Account Details After Withdraw:\n");
        }
        //0 means changes are not saved ,hence returns number of accounts back to before addition
        else
        {
            printf("------------------------------------------------\n");
            printf("\nWithdraw process failed\n");

        }
    }

}

int findAccountIndex(char acc_number[11])
{
    for (int i = 0; i < numAccounts; i++)
    {
        if (strcmp(account_data[i].account_number, acc_number) == 0)
        {
            return i;//el acount num mawgood
        }
    }
    return -1;//el rakam me4mawgood
}

void account_deposit()
{
    double amount;
    char account_number[11];
    int flag = 0;
    printf("\n\n======= Deposit =======\n\n");

    while (flag == 0)
    {
        printf("Enter the account number :");
        scanf("%s", account_number);

        if (is_valid_acc_num(account_number) == 0)
        {
            printf("Invalid Account number! Please enter 10 digits!\n");
            flag = 0;
        }
        else
        {
            flag = 1; // Assign 1 to flag to exit the loop
        }
    }
    int index_of_account_number = findAccountIndex(account_number);
    if (index_of_account_number == -1)
    {
        printf("Account number not found\n");
        return;
    }

    int valid_amount_flag = 0;
    while (valid_amount_flag == 0)
    {
        printf("Enter amount to Deposit: ");
        if (scanf("%lf", &amount) != 1 || getchar() != '\n')
        {
            printf("Invalid input! Please enter a valid amount (numeric).\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else
        {
            valid_amount_flag = 1;
        }
    }

    double maximum_deposit = 10000.0;

    if (amount > maximum_deposit)
    {
        printf("The deposit amount is above the maximum limit of %0.2lf\n",
               maximum_deposit); //deposit process failed here as it is above the (maximum_withdraw).
        return;
    }
    if (amount <= 0)
    {
        printf("invalid deposit amount\n"); //deposit process failed.
        return;
    }
    if (valid_amount_flag)
    {
        (account_data[index_of_account_number].balance) += amount;

        if (Save() == 1)   //change
        {
            printf("------------------------------------------------\n");
            printf("\nDeposit process Successfully\n");
            // Create files for both sender and receiver
            char deposit_file_name[15]; // Account file name with ".txt" extension
            sprintf(deposit_file_name, "%s.txt", account_number); // Append ".txt" to the  account number
            FILE *deposit_file = fopen(deposit_file_name, "a");
            if (deposit_file == NULL)
            {
                printf("Error opening account file for transaction record.\n");
                if (deposit_file != NULL) fclose(deposit_file);
                return;
            }
            fprintf(deposit_file, "Deposit transaction +%.2lf$ \n", amount);
            fclose(deposit_file);
            // printf("\nUpdated Account Details After Deposit:\n");
        }
        //0 means changes are not saved ,hence returns number of accounts back to before addition
        else
        {
            printf("------------------------------------------------\n");
            printf("\nDeposit process failed\n");

        }
    }


}

void TransferMoney()
{
    char sender_acc[11], receiver_acc[11];
    double amount;

    printf("\n\n======= Transfer =======\n\n");

    do
    {
        printf("Enter sender's account number: ");
        scanf("%s", sender_acc);
        if (!is_valid_acc_num(sender_acc))
            printf("Invalid Account number! Please enter 10 digits!\n");
    }
    while (!is_valid_acc_num(sender_acc));

    do
    {
        printf("Enter receiver's account number: ");
        scanf("%s", receiver_acc);
        if (!is_valid_acc_num(receiver_acc))
        {
            printf("Invalid Account number! Please enter 10 digits!\n");

        }
    }
    while (!is_valid_acc_num(receiver_acc));

    int sender_index = findAccountIndex(sender_acc);
    int receiver_index = findAccountIndex(receiver_acc);

    if (sender_index == -1 || receiver_index == -1)
    {
        printf("Sender or receiver account not found.\n");
        return;
    }

    if (strcmp(account_data[sender_index].account_number, account_data[receiver_index].account_number) == 0)
    {
        printf("Error! Enter a different account number");
        return;
    }

    int valid_amount_flag = 0;

    while (valid_amount_flag == 0)
    {
        printf("Enter an amount to transfer: ");
        if (scanf("%lf", &amount) != 1 || getchar() != '\n')
        {
            printf("Invalid input! Please enter a valid amount (numeric).\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else
        {
            valid_amount_flag = 1;
        }
    }
    if (amount <= 0 || amount > account_data[sender_index].balance)
    {
        printf("Invalid amount or insufficient balance for transfer.\n");
        return;
    }
    account_data[sender_index].balance -= amount;
    account_data[receiver_index].balance += amount;

    if (Save() == 1)
    {

        printf("Transfer successful!\n");
        // Create files for both sender and receiver
        char sender_file_name[15]; // Account file name with ".txt" extension
        char receiver_file_name[15]; // Account file name with ".txt" extension
        sprintf(sender_file_name, "%s.txt", sender_acc); // Append ".txt" to the sender's account number
        sprintf(receiver_file_name, "%s.txt", receiver_acc); // Append ".txt" to the receiver's account number

        FILE *sender_file = fopen(sender_file_name, "a");
        FILE *receiver_file = fopen(receiver_file_name, "a");

        if (sender_file == NULL || receiver_file == NULL)
        {
            printf("Error opening account file for transaction record.\n");
            if (sender_file != NULL) fclose(sender_file);
            if (receiver_file != NULL) fclose(receiver_file);
            return;
        }

        fprintf(sender_file, "Transfer transaction sent %.2lf$ to %s Account Number\n", amount, receiver_acc);
        fprintf(receiver_file, "Transfer transaction received %.2lf$ from %s Account Number\n", amount, sender_acc);

        fclose(sender_file);
        fclose(receiver_file);
    }

    //0 means changes are not saved ,hence returns number of accounts back to before addition
    else
    {
        printf("Transfer Failed.\n");


    }
}

int Save()
{
    char ans;
    FILE *file;
    printf("\nDo you want to save your changes?\n(choose y or n)\n");    //changes
    ans = getchar();
    //handles uppercases provided by the user
    if (islower(ans));
    else ans = tolower(ans);
    switch (ans)
    {
    case 'y':   //if yes opens file in write mode to be used for all functions that modifies the array account_data
        file = fopen("accounts.txt", "w");
        if (file == NULL)
        {
            printf("Save failed due to file not found.\n");
            return 0;
        }
        //writes the whole file after the modification
        for (int i = 0; i < numAccounts; i++)
        {
            fprintf(file, "%s,", account_data[i].account_number);
            fprintf(file, "%s,", account_data[i].name);
            fprintf(file, "%s,", account_data[i].email_address);
            fprintf(file, "%.2lf,", account_data[i].balance);
            fprintf(file, "%s,", account_data[i].mobile_number);
            fprintf(file, "%d-%d\n", account_data[i].date_opened.month, account_data[i].date_opened.year);
        }
        printf("Changes saved successfully.\n");
        fclose(file);
        LoadAccountsFromFile();
        return 1;   //1 means change successful

    case 'n':   //if yes ,then file is not overwritten
        LoadAccountsFromFile();    //loads data to return the array (account_data) to its values before modification
        printf("Changes discarded.\n");
        return 0;   //0 means no changes happened

    default:    //false input by user
        printf("Invalid input.\n");
        Save();     //calls save again to
        break;
    }
    return 0;
}

void Report()
{
    char account_number[11];
    printf("\n\n======= Report =======\n\n");

    do
    {
        printf("Enter Account Number: ");
        scanf("%s", account_number);
        if (!is_valid_acc_num(account_number))
            printf("Invalid Account number! Please enter 10 digits!\n");

    }
    while (!is_valid_acc_num(account_number));
    if(findAccountIndex(account_number)==-1)
    {
        printf("The account number not found.\n");
        return;
    }



    char file_name[15]; // Account file name with ".txt" extension
    sprintf(file_name, "%s.txt", account_number); // Append ".txt" to the account number

    FILE *account_file = fopen(file_name, "r");
    if (account_file == NULL)
    {
        printf("No Transaction happened.\n");
        return;
    }

    char transactions[5][100]; // Array to store last 5 transactions
    int count = 0;

    // Read the last 5 lines of the file
    while (fgets(transactions[count % 5], sizeof(transactions[count % 5]), account_file) != NULL)
    {
        count++;
    }

    if (count < 5)
    {
        int start = 0; // Start index to print from
        int i;
        for (i = start; i < count; i++)
        {
            printf("%s", transactions[i]);
        }
    }
    else
    {
        // Print the last 5 transactions in reverse order
        int start = count - 5; // Start index to print from
        int i;
        for (i = start; i < count ; i++)
        {
            printf("%s", transactions[i % 5]);
        }
    }

    fclose(account_file);
}
void sortByDate()
{
    int i = 0;
    int j = 0;
    Account temp;
    for (i = 0; i < numAccounts - 1; i++)
    {
        for (j = 0; j < numAccounts - i - 1; j++)
        {
            if ((account_data[j + 1].date_opened.year < account_data[j].date_opened.year) ||
                    ((account_data[j + 1].date_opened.year == account_data[j].date_opened.year)
                     && (account_data[j + 1].date_opened.month <= account_data[j].date_opened.month)))
            {

                strcpy(temp.account_number, account_data[j + 1].account_number);
                strcpy(temp.name, account_data[j + 1].name);
                strcpy(temp.mobile_number, account_data[j + 1].mobile_number);
                strcpy(temp.email_address, account_data[j + 1].email_address);
                temp.balance = account_data[j + 1].balance;
                temp.date_opened.month = account_data[j + 1].date_opened.month;
                temp.date_opened.year = account_data[j + 1].date_opened.year;

                strcpy(account_data[j + 1].account_number, account_data[j].account_number);
                strcpy(account_data[j + 1].name, account_data[j].name);
                strcpy(account_data[j + 1].mobile_number, account_data[j].mobile_number);
                strcpy(account_data[j + 1].email_address, account_data[j].email_address);
                account_data[j + 1].balance = account_data[j].balance;
                account_data[j + 1].date_opened.month = account_data[j].date_opened.month;
                account_data[j + 1].date_opened.year = account_data[j].date_opened.year;


                strcpy(account_data[j].account_number, temp.account_number);
                strcpy(account_data[j].name, temp.name);
                strcpy(account_data[j].mobile_number, temp.mobile_number);
                strcpy(account_data[j].email_address, temp.email_address);
                account_data[j].balance = temp.balance;
                account_data[j].date_opened.month = temp.date_opened.month;
                account_data[j].date_opened.year = temp.date_opened.year;

            }

        }


    }
    while (1)
    {
        int x;

        int valid_amount_flag = 0;

        while (valid_amount_flag == 0)
        {
            printf("Please choose the order of sorting\n1)Oldest first\n2)Newest first\n");
            printf("-----------------------------\n");
            if (scanf("%d", &x) != 1 || getchar() != '\n')
            {
                printf("Invalid input! Please enter a valid one (numeric).\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            else
            {
                valid_amount_flag = 1;
            }
        }

        if (x == 1)
        {
            printf("-----------------------------\n");
            printf("Result: \n");
            printf("-----------------------------\n");
            displayAccounts();
            break;
        }
        else if (x == 2)
        {
            printf("-----------------------------\n");
            printf("Result: \n");
            printf("-----------------------------\n");
            reverseddisplayAccounts();
            break;
        }
        else
            printf("The option you have entered is invalid \n");
        printf("-----------------------------\n");

    }
    LoadAccountsFromFile();
}

void sortByBalance()
{
    int i = 0;
    int j = 0;
    Account temp;
    for (i = 0; i < numAccounts - 1; i++)
    {
        for (j = 0; j < numAccounts - i - 1; j++)
        {
            if (account_data[j + 1].balance < account_data[j].balance)
            {
                strcpy(temp.account_number, account_data[j + 1].account_number);
                strcpy(temp.name, account_data[j + 1].name);
                strcpy(temp.mobile_number, account_data[j + 1].mobile_number);
                strcpy(temp.email_address, account_data[j + 1].email_address);
                temp.balance = account_data[j + 1].balance;
                temp.date_opened.month = account_data[j + 1].date_opened.month;
                temp.date_opened.year = account_data[j + 1].date_opened.year;

                strcpy(account_data[j + 1].account_number, account_data[j].account_number);
                strcpy(account_data[j + 1].name, account_data[j].name);
                strcpy(account_data[j + 1].mobile_number, account_data[j].mobile_number);
                strcpy(account_data[j + 1].email_address, account_data[j].email_address);
                account_data[j + 1].balance = account_data[j].balance;
                account_data[j + 1].date_opened.month = account_data[j].date_opened.month;
                account_data[j + 1].date_opened.year = account_data[j].date_opened.year;


                strcpy(account_data[j].account_number, temp.account_number);
                strcpy(account_data[j].name, temp.name);
                strcpy(account_data[j].mobile_number, temp.mobile_number);
                strcpy(account_data[j].email_address, temp.email_address);
                account_data[j].balance = temp.balance;
                account_data[j].date_opened.month = temp.date_opened.month;
                account_data[j].date_opened.year = temp.date_opened.year;

            }

        }

    }
    while (1)
    {
        int x;

        int valid_amount_flag = 0;

        while (valid_amount_flag == 0)
        {
            printf("Please choose the order of sorting\n1)Accending\n2)Decending\n");
            printf("-----------------------------\n");
            if (scanf("%d", &x) != 1 || getchar() != '\n')
            {
                printf("Invalid input! Please enter a valid one (numeric).\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            else
            {
                valid_amount_flag = 1;
            }
        }

        if (x == 1)
        {
            printf("-----------------------------\n");
            printf("Result: \n");
            printf("-----------------------------\n");
            displayAccounts();
            break;
        }
        else if (x == 2)
        {
            printf("-----------------------------\n");
            printf("Result: \n");
            printf("-----------------------------\n");
            reverseddisplayAccounts();
            break;
        }
        else
            printf("The option you have entered is invalid \n");
        printf("-----------------------------\n");

    }
    LoadAccountsFromFile();
}

void sortByName()
{
    int i;
    int j;
    Account temp;
    for (i = 0; i < numAccounts - 1; i++)
    {
        for (j = 0; j < numAccounts - 1 - i; j++)
        {
            if (strcmp(account_data[j + 1].name, account_data[j].name) < 0)
            {
                strcpy(temp.account_number, account_data[j + 1].account_number);
                strcpy(temp.name, account_data[j + 1].name);
                strcpy(temp.mobile_number, account_data[j + 1].mobile_number);
                strcpy(temp.email_address, account_data[j + 1].email_address);
                temp.balance = account_data[j + 1].balance;
                temp.date_opened.month = account_data[j + 1].date_opened.month;
                temp.date_opened.year = account_data[j + 1].date_opened.year;

                strcpy(account_data[j + 1].account_number, account_data[j].account_number);
                strcpy(account_data[j + 1].name, account_data[j].name);
                strcpy(account_data[j + 1].mobile_number, account_data[j].mobile_number);
                strcpy(account_data[j + 1].email_address, account_data[j].email_address);
                account_data[j + 1].balance = account_data[j].balance;
                account_data[j + 1].date_opened.month = account_data[j].date_opened.month;
                account_data[j + 1].date_opened.year = account_data[j].date_opened.year;


                strcpy(account_data[j].account_number, temp.account_number);
                strcpy(account_data[j].name, temp.name);
                strcpy(account_data[j].mobile_number, temp.mobile_number);
                strcpy(account_data[j].email_address, temp.email_address);
                account_data[j].balance = temp.balance;
                account_data[j].date_opened.month = temp.date_opened.month;
                account_data[j].date_opened.year = temp.date_opened.year;
            }


        }

    }

    while (1)
    {
        int x;

        int valid_amount_flag = 0;

        while (valid_amount_flag == 0)
        {
            printf("Please choose the order of sorting\n1)A to Z\n2)Z to A\n");
            printf("-----------------------------\n");
            if (scanf("%d", &x) != 1 || getchar() != '\n')
            {
                printf("Invalid input! Please enter a valid one (numeric).\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            else
            {
                valid_amount_flag = 1;
            }
        }

        if (x == 1)
        {
            printf("-----------------------------\n");
            printf("Result: \n");
            printf("-----------------------------\n");
            displayAccounts();
            break;
        }
        else if (x == 2)
        {
            printf("-----------------------------\n");
            printf("Result: \n");
            printf("-----------------------------\n");
            reverseddisplayAccounts();
            break;
        }
        else
            printf("The option you have entered is invalid \n");
        printf("-----------------------------\n");

    }
    LoadAccountsFromFile();
}

void print()
{
    while (1)
    {
        int x;

        int valid_amount_flag = 0;
        printf("\n\n======= Print =======\n\n");

        while (valid_amount_flag == 0)
        {
            printf("Please enter the type of sort you want\n1)Sort by name\n2)Sort by balance\n3)Sort by date\n");
            printf("-----------------------------\n");
            if (scanf("%d", &x) != 1 || getchar() != '\n')
            {
                printf("Invalid input! Please enter a valid one (numeric).\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            else
            {
                valid_amount_flag = 1;
            }
        }


        if (x == 1)
        {
            sortByName();
            break;
        }
        else if (x == 2)
        {
            sortByBalance();
            break;
        }
        else if (x == 3)
        {
            sortByDate();
            break;
        }
        else
            printf("The option you have entered is invalid\n");
        printf("-----------------------------\n");

    }

}

void Menu()
{
    int option;

    do
    {
        printf("\n======= Menu =======\n\n");
        printf("1. ADD\n");
        printf("2. DELETE\n");
        printf("3. MODIFY\n");
        printf("4. SEARCH\n");
        printf("5. ADVANCED SEARCH\n");
        printf("6. WITHDRAW\n");
        printf("7. DEPOSIT\n");
        printf("8. TRANSFER\n");
        printf("9. REPORT\n");
        printf("10. PRINT\n");
        printf("11. QUIT\n");
        int valid_number_flag = 0;
        while (valid_number_flag == 0)
        {
            printf("-----------------------------\n");
            fflush(stdin);
            printf("Enter your choice (numeric) :");
            if (scanf("%d", &option) != 1 || getchar() != '\n')
            {
                printf("Invalid input! Please enter a valid one (numeric).\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            else
            {
                valid_number_flag = 1;
            }
        }

        switch (option)
        {
        case 1:
            add_account();
            break;

        case 2:
            delete_account();
            break;

        case 3:
            modify_account();
            break;

        case 4:
            query_search();
            break;

        case 5:
            Advanced_Search();
            break;

        case 6:
            account_withdraw();
            break;

        case 7:
            account_deposit();
            break;

        case 8:
            TransferMoney();
            break;

        case 9:
            Report();
            break;

        case 10:
            print();
            break;

        case 11:
            printf("Quitting the program ....");
            exit(3);

        default:
            printf("Invalid option! Please try again.\n");
            break;
        }


    }
    while (option != 11);
}


void main()
{
    int loggedIn = 0; // 0: Not logged in, 1: Logged in
    int option;
    printf("======= Menu =======\n\n");

    do
    {
        printf("1. LOGIN\n");
        printf("2. QUIT\n");
        int valid_number_flag = 0;
        while (valid_number_flag == 0)
        {
            fflush(stdin);
            printf("-----------------------------\n");
            printf("Enter your choice (numeric) :");
            if (scanf("%d", &option) != 1 ||
                    getchar() != '\n')//checking that the input is number not an alphabetical char
            {
                printf("Invalid input! Please enter a valid one (numeric).\n");
                while (getchar() != '\n'); // Clear input buffer
            }
            else
            {
                valid_number_flag = 1;
            }
        }
        switch (option)
        {
        case 1:
            login();
            loggedIn = 1; // Assuming successful login
            LoadAccountsFromFile();
            break;

        case 2:
            printf("Exiting...\n");
            return;

        default:
            printf("Invalid option! Please try again.\n");
            break;
        }
    }
    while (loggedIn != 1);

    if (loggedIn == 1)
    {
        Menu();
    }


    return;
}

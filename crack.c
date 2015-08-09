/*
 * Cracks DES passwords with brute force
 */
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define C_NAME "./crack"        //command name
#define S_LIMIT 70000           //words limit
#define C_LIMIT 40              //characters limit /* the current wordlist has words with more than 8 characters * / 
#define W_LIST "word_list"      //wordlist file

int p_crack(char* pwd, const char* passwd);
void yell(void);
void load_words(char(*words)[C_LIMIT]); //can't use char** instead, why?

int main(int argc, char* argv[])
{
    //lets yell at the user if there are too many or no arguments at all
    if (argc > 2 || argc == 1)
    {
        yell();
        return 1;
    }

    char password[C_LIMIT];

    if(p_crack(password, argv[1]))
        printf("Cracked password: %s\n", password);
    else
        printf("No passwords were found.\n");
    
    return 0;
}

int p_crack(char* pwd, const char* passwd)
{
    char c_password[C_LIMIT], salt[2]; 

    //the salt are the first 2 chars in the encrypted passwd
    for (int i = 0; i < 2; i++)
        salt[i] = passwd[i];

    char words[S_LIMIT][C_LIMIT] = {'\0'};
    load_words(words);

    //cracking starts here
    for (int i = 0; strlen(words[i]) != 0 && i < S_LIMIT; i++)
            {
                //encrypts the current password in words[i]
                strcpy(c_password, crypt(words[i], salt));  //crypt only takes the first 8 chars of the string
                                                            //which means if the string is longer than that
                                                            //the crypt function will only encrypt the first 8 chars
                                                            //and the rest will be lost
                                                            //for this kind of cracking, a wordlist with words no longer than 8 chars is needed
                
                //debug
                printf("Word: %-20s" "%s\n", words[i], c_password);

                //if the encrypted c_password and the passwd we want to crack are the same, that means words[i] is the cracked password 
                if (strcmp(c_password,  passwd) == 0) 
                {
                    //show processed words
                    printf("\nNumber of words processed:%d\n", i+1);

                    strcpy(pwd,  words[i]);
                    return 1;               //we succeded
                }
            }

    //if we didn't find any password lets return 0 
    return 0;
}

void yell(void)
{
    printf("That's not how you should use this program!\n"
            "How to use: %s [EncryptedPassword]\n"
            "e.g.:%s 50JkB0IlGZ0mw\n", C_NAME, C_NAME);
}

void load_words(char (*words)[C_LIMIT])
{

    FILE* w_list = fopen(W_LIST, "r");
    if(w_list == NULL)
    {
        printf("ERROR: Couldn't open the wordlist file \'%s\'\n"
                "Please verify that the file exist and run the program again.\n", W_LIST); 
        exit(EXIT_FAILURE);
    }
    else
    {
        //load all the words in the file to words[]
        for(int i = 0; fgets(words[i], C_LIMIT, w_list) != NULL && i < S_LIMIT; i++)
        {
            words[i][strlen(words[i]) - 1] = '\0';   //removes the '\n' char at the end that fgets writes 

            //debug
            //printf("%s\n", words[i]);
        }
    }

    fclose(w_list);
}


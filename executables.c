#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

// declaring function
void checkForExecutables(DIR *dir, struct dirent *entry, struct stat buf);

int main(int argc, char *argv[])
{
    DIR *dir;

    struct dirent *entry;

    struct stat buf;

    int colonCheck;

    // If no command line arguments are passed, print the executables in the current working directory
    if (argv[1] == NULL)
    {
        // set dir to the current working directory
        dir = opendir(".");
        // pass the dir to function to print all executables
        checkForExecutables(dir, entry, buf);
        // closing the directory as we have finished reading from it
        closedir(dir);

        return EXIT_SUCCESS;
    }

    // checking if passed arguent contains a colon
    if (strchr(argv[1], ':') != NULL)
    {
        colonCheck = 1;
    }

    if (colonCheck == 1)
    {   
        // we will tokenise the string in argv[1], splitting it when we find a colon
        char * token = strtok(argv[1], ":");

        while (token != NULL)
        {
            // the value in token is set as the current directory
            dir = opendir(token);
            // change to that directory
            chdir(token);
            // print statement to tell user what directory the executables are located in
            printf("\nExecutable files in: %s\n", token);
            // pass the dir to function to print all executables
            checkForExecutables(dir, entry, buf);
            // closing the directory as we have finished reading from it
            closedir(dir);
            // move to next token (in our case it will be the next directory)
            token = strtok(NULL, ":");

        }
        return EXIT_SUCCESS;
    }
    
    else
    {
        // for each commonad line argument passed, print all executables within that directory
        for (int i = 1; i < argc; i++)
        {
            // set dir to the directory passed via the command line argument
            dir = opendir(argv[i]);
            // change to that directory
            chdir(argv[i]);
            // print statement to tell user what directory the executables are located in
            printf("\nExecutable files in: %s\n", argv[i]);
            // pass the dir to function to print all executables
            checkForExecutables(dir, entry, buf);
            // closing the directory as we have finished reading from it
            closedir(dir);
        }
    }
    return EXIT_SUCCESS;
}

// declaring a function that will loop through the dir passed and print the executables files
void checkForExecutables(DIR *dir, struct dirent *entry, struct stat buf)
{
    while ((entry = readdir(dir)) != NULL)
    {
        // calling stat function for file in current dir
        stat(entry->d_name, &buf);

        // checking that the file is not a directory
        if(entry->d_type != DT_DIR)
        {
            // checking if file is executable 
            if (S_ISREG(buf.st_mode) && buf.st_mode & 0100)
            {
                // printing the executable's name
                printf("%s is executable\n", entry->d_name);
            }

        }
    }

}
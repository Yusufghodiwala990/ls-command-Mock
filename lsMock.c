/*######################################################################################

# Name: lsMock.c
# Written by: Yusuf Ghodiwala - February 2021
# Purpose: To be able to pass arguments to get specific information/attributes about a directory and it's files (mock 'ls' command)
# 

# Description of parameters to be passed:
#  argv : This will hold all the options chosen and the actual directory path the user wants to see attributes of.
#
# Usage:  "./lab3 [option] [integer] [path]. There are 4 options to choose -y, -s, -o, -l, the user can choose all 4 at once.
#           An example; ./lab3 -y 34 -s 6000 -l 2000 -o 2 /home/lab3/"
#
# DESCRIPTION OF THE OPTIONS;
# -y  = Younger Than - Maximum age a file needs be to be listed
# -o  = Older Than - Minimum age a file needs to be to be listed
# -s  = Smaller Than - Maximum size in bytes a file needs to be to be listed
# -l  = Larger Than - Minimum size in bytes a file needs to be to be listed
#
#
# # DATA DICTIONARY
# VARIABLES                          DESCRIPTION
# all flags for the options          For each option there are flags set to know what option was selected and to process
# *dp                                Pointer to the directory passed or current directory will be assumed if not passed
#  struct stat file_status           struct that will be populated by using the stat() system call
#  return_value                      integer to check if stat() call returned an error
#  char * filename                   Pointer to a file currently being processed
# option                             Integer used to store which option was selected
# *dirname                           Directory name
# dname                              array to store the directory path. Current directory will be assumed if nothing is passed
# option integers                    These variables will store the actual integer passed after selecting an option

######################################################################################
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // to use time_t functions
#include <sys/types.h>
#include <sys/stat.h>   // for stat()
#include <sys/types.h>
#include <getopt.h>     // to parse options from command line
#include <dirent.h> // to open directories
#include <ctype.h>
#include <string.h>
#include <unistd.h>


// defining a colour for column headers
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

;

// only function

// to display a file's attributes using stat()
int displayStatInfo(const struct stat *sb, char *fname);

int main(int argc, char *argv[])
{
    if(strlen(argv[argc-1]> 4096))
     {
        printf("Path is too long!");
        exit(1);
    }

    // a temp array to keep store the directory path from command line
    char *dname[4096];

    // default directory
    dname[1] = "./";

    //else use the one passed
    dname[2] = argv[argc - 1];

    // pointer to the directory name
    char *dirname;

    
    

    // pointer of type DIR (directory)
    DIR *dp;

    // struct which will contain attributes about the directory(files)
    struct dirent *dir;

    // struct of the file that will be populated by stat()
    struct stat file_status;

    // error checking stat() call
    int return_value;

    // the actual filename currently being processed in a directory
    char *filename;
   

   // all the flags and their respective integer values
    int younger_flag = 0;
    int younger_than = 0;

    int older_flag = 0;
    int older_than = 0;

    int larger_flag = 0;
    int larger_than = 0;

    int smaller_flag = 0;
    int smaller_than = 0;

    int pass;

    int option;
    // used gnu software manual for getopt() to write this loop

    // each ':' after an option indicates that there is an argument needed,
    //          an integer to be specific

    // i convert the value by using atoi(optarg). optarg is auto populated by getopt()
    while ((option = getopt(argc, argv, "y:l:o:s:")) != -1)
    {
        switch (option)
        {
        case 'y':
       
             // when younger than is selected
            younger_flag = 1;
            
            
            younger_than = atoi(optarg);
            break;

        case 'o':
            
            // when older than is selected
            older_flag = 1;

            
            older_than = atoi(optarg);
            
            break;
        case 'l':
            
            // when larger than option is selected
            larger_flag = 1;
           
            larger_than = atoi(optarg);
            
            break;
        case 's':
            // when smaller than option is slected
            smaller_flag = 1;
            smaller_than = atoi(optarg);
            
            break;

        case '?':

        // when no integer is passed after an option or incorrect/invalid option
        default:
            if (optopt == 'y' || optopt == 'l' || optopt == 'o' || optopt == 's')
                fprintf(stderr, "Option requires an argument. `-%c'.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x.\n", optopt);
            exit(2);
        }
    }
    

    // since each option will be accompanied by an int, argc will increment twice
    //   we use this to know if a path was passed

    // but we also check if nothing was passed, then we just print the current directory.
    if (argc % 2 != 0 && argc != 1 || argc == 1)
    {
        printf("\nAssuming the current directory...\n\n");
        dirname = dname[1];
    }
    

    // dname[] was declared earlier and now we can use (when a path is actually passed)
    else
    {
        dirname = dname[2];
        //printf("%s",dirname);
    }
   

   // printing the column headers, i used some colour.
    printf(ANSI_COLOR_RED "PERMISSIONS: TYPE: I-NODE:   SIZE:   Last change:              "
                          "UID:         FILE_NAME:\n" ANSI_COLOR_RESET);

    // the dp pointer will be null if we can't open the directory                      
    if ((dp = opendir(dirname)) == NULL)
    {

        fprintf(stderr, "Cannot open the directory!\n");
        printf("\n%s", dirname);

        exit(2);
    }
    else
    {
        
        // else we read the directory file by file
        while ((dir = readdir(dp)) != NULL)
        {

            // we don't care for inodes of 0 and entries which have information about
             //    the current directory and the parent directory
            if (dir->d_ino == 0 || !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
            {
                // do nothing
                // we don't want to process "." , ".." and inodes of 0
            }
            else
            {
                // storing the filename
                filename = dir->d_name;

                // allocating memory to store the full path(with directory name) to pass it to stat()
                //   otherwise it will fail
                char *full_filepath = (char *)malloc(1 + strlen(filename) + strlen(dirname));
                
                // copying the directory path
                strcpy(full_filepath, dirname);

                // concat-ing with the filename
                strcat(full_filepath, filename);

                

                return_value = stat(full_filepath, &file_status);

                // checking for stat() errors
                if (return_value == -1)
                    printf("\nstat failed");
                

                // when nothing was passed, we just print the current directory
                if (argc == 1)
                {
                    displayStatInfo(&file_status, filename);
                }

                // else we start processing options
                else
                {
                    // variable to store the current time
                    long now;

                    // variable to store seconds in a day
                    int seconds;
                    seconds = 86400;

                     // using the time struct to get the current time
                    time_t current_time;
                    current_time = time(NULL);
                    now = (long)current_time;

                    // retrieving the modification timestamp from a file using st_mtime
                    time_t modTime = file_status.st_mtime;

                    // to store the actual seconds that should have been passed for -y and -o
                    int sec_passed;

                    // calculating the age of the file
                    long int age;
                    

                    // storing the file size
                    long int size;
                    size = file_status.st_size;
                    


                    // flag checking for each option to see if we need to process them
                    if (younger_flag)
                    {   

                        // sec_passed is the number that will bring us to the age of the file
                        //   by multiplying the maximum days old a file can be. We multiply 
                        //   by seconds in a day
                        sec_passed = younger_than * seconds;

            // we caclulate the intended age by subtracting the current time and the number of days calc
                        age = now - sec_passed;

            // if the mod timestamp is greater than the intended max age the user wants, it is in the range
                        if (modTime > age)
                            {} // do nothing, keep checking other option
                        else
                            continue; // if it's not, immediately skip this iteration, and move on to the next file

                       
                    }
                    if (older_flag)
                    {
                        // we do the same thing with older than option, this case it's min age,
                        //  so we check if the modTime is < than the inteded min age, if so
                        //   it passes the check
                        sec_passed = older_than * seconds; 
                        age = now - sec_passed;
                        if (modTime < age)
                            {}
                        else
                            continue;     // doesn't match, move on to the next file
                        
                       
                    }

                    if (larger_flag)
                    {
                        // for larger than we check if the file size is greater than the value passed
                        if(size > larger_than)
                           {}
                        else
                           continue;
                        
                    }
                    if (smaller_than)
                    {
                        // for smaller than we check if the file size is less than the value passed
                        if(size < smaller_than)
                        {} // it passed, do nothing
                        
                        else
                           continue;
                    }
                    

                    // if we get this far, this means the file passed all the checks the user entered,
                    // we call displayStatInfo() to print. We send the struct, and the filename(just to print)
                    
                     displayStatInfo(&file_status, filename);
                
                }
            }
        }
    }
}



/*====================================================================== 
|   
|
|   Function: displayStatInfo
|
|   Written by: Jacques Beland - February 2016
|   Modified by : Yusuf Ghodiwala - March 2021
|
|   Purpose: display file attributes for the stat structure passed as an argument
|        
|
|   Description of parameters:
|    a stat structure returned by the stat() system call.
|    see sys/stat.h for details on the field(s) and what they contain.
|   
|    filename : just to print
|
|   Subroutines/libraries required:
|      none.
|
|
|------------------------------------------------------------------*/
void displayStatInfo(const struct stat *sb, char *fname)
{

 

   

    // Permissions

    // we do this process by AND-ing the necessary bit from st_mode to predefined set of bits
    //  that help us gather info about the permissions. In this case, we're using macros


    printf((S_ISDIR(sb->st_mode)) ? "d" : "-"); // although i'm printing file type as well, i added a column to
                                                //  match the ls command

    printf((sb->st_mode & S_IRUSR) ? "r" : "-");
    printf((sb->st_mode & S_IWUSR) ? "w" : "-");
    printf((sb->st_mode & S_IXUSR) ? "x" : "-");
    printf((sb->st_mode & S_IRGRP) ? "r" : "-");
    printf((sb->st_mode & S_IWGRP) ? "w" : "-");
    printf((sb->st_mode & S_IXGRP) ? "x" : "-");
    printf((sb->st_mode & S_IROTH) ? "r" : "-");
    printf((sb->st_mode & S_IWOTH) ? "w" : "-");
    printf((sb->st_mode & S_IXOTH) ? "x" : "-");

    switch (sb->st_mode & S_IFMT) // parse out the file type
    {
    case S_IFREG:
        printf("    REG ");   //Regular File
        break;
    case S_IFDIR:
        printf("    DIR ");  // Directory
        break;
    case S_IFCHR:
        printf("   CHD "); // character device
        break;
    case S_IFBLK:
        printf("   BD ");  // block device
        break;
    case S_IFLNK:
        printf("   SL "); // 
        break;
    case S_IFIFO:
        printf("   FIFO "); //FIFO OR PIPE
        break;
    case S_IFSOCK:
        printf("   SOCK ");  // Socket
        break;
    default:
        printf("   ?? ");  // unkown file type
        break;
    }

    printf("%6ld", (long)sb->st_ino); // print the file's inode info

    printf("%10lld  ", (long long)sb->st_size); // print the file size

    

    // had to remove newline character because ctime adds a newline, but it skews the output so turned that newLine
    //    to a null terminator

    char *t = ctime(&sb->st_mtime);
    if (t[strlen(t) - 1] == '\n')
        t[strlen(t) - 1] = '\0';

    printf("%24s", t); // last modified time
                       

    //UID
    printf("%5lld ", (long long)sb->st_uid);   // UID
    printf("%20s \n", fname);                  // filename

}

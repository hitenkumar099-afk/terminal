#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h> // For _chdir and _getcwd on Windows
#define chdir _chdir
#define getcwd _getcwd
#endif

#define CREATE_DIR(dir) mkdir(dir, 0777)
#define MAX_LENGTH 256

// list of file function
void list_file()
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    if (d)
    {
        printf("\n--- Files in Current Directory ---\n");
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] != '.')
            {
                char tag_buf[256] = "";
                get_tag(dir->d_name, tag_buf, sizeof(tag_buf));

                if (strlen(tag_buf) > 0)
                {
                    printf("Name: %s [%s] ", dir->d_name, tag_buf);
                }
                else
                {
                    printf("Name: %s ", dir->d_name);
                }
            }
            printf("----------------------------------\n");
        }
        closedir(d);
    }
    else
    {
        perror("Error While opening directory!\n");
    }
}
// renaming function
void rename_file(const char *old_name, const char *new_name)
{
    if (old_name == NULL || strlen(old_name) == 0 || new_name == NULL || strlen(new_name) == 0)
    {
        printf("Error! Please provide both the current name and the new name.\n");
        return;
    }

    if (rename(old_name, new_name) == 0)
    {
        printf("Name changed succesfully!\n");
    }
    else
    {
        perror("Error while renaming file");
    }
}

// create file function
void create_file(const char *filename)
{

    if (filename == NULL || strlen(filename) == 0)
    {
        printf("Error! Please enter a file name\n");
        return;
    }
    else
    {
        FILE *ptr = fopen(filename, "w");
        if (ptr == NULL)
        {
            printf("Error while creating file");
        }
        else
        {
            printf("Succesfully created: %s\n", filename);
            fclose(ptr);
        }
    }
}

// read file function
void read_file(const char *filename)
{

    if (filename == NULL || strlen(filename) == 0)
    {
        printf("Error enter a file name\n");
        return;
    }
    else
    {
        FILE *ptr = fopen(filename, "r");
        if (ptr == NULL)
        {
            printf("Error while creating file\n");
        }
        else
        {
            int ch;
            while ((ch = fgetc(ptr)) != EOF)
            {
                printf("\n Content of file ------------------------- \n");
                putchar(ch);

                printf("\n ----------------------- \n");
            }
            fclose(ptr);
        }
    }
}

// edit file function
void edit_file(const char *filename)
{

    if (filename == NULL || strlen(filename) == 0)
    {
        printf("enter while opening file");
        return;
    }

    FILE *ptr = fopen(filename, "a");
    if (ptr == NULL)
    {
        printf("Error while creating file");
    }
    else
    {
        char content[512];
        printf("Enter content: ");
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = '\0';
        fputs(content, ptr);
        fclose(ptr);
        printf("Data Written Succesfully!\n");
    }
}

// delete file function
void file_delete(const char *filename)
{
    if (filename == NULL || strlen(filename) == 0)
    {
        printf("Error while deleting file\n");
    }
    else if (unlink(filename) == 0)
    {
        printf("File Deleted!\n");
    }
    else
    {
        perror("Error while deleting file\n");
    }
}

// directory changing function
void change_dir(const char *next_path)
{
    if (next_path == NULL || strlen(next_path) == 0)
    {
        printf("Error while opening path");
        return;
    }

    if (chdir(next_path) != 0)
    {
        printf("Error while changing directoy");
    }
}

// restart program function
void restart_program(int argc, char *argv[])
{
    printf("Restarting file manager...\n");
    execvp(argv[0], argv);

    // If execvp returns, it means an error occurred
    perror("Error while restarting\n");
}

// tag file function
void tag_file(const char *filename, const char *note)
{

    if (filename == NULL || strlen(filename) == 0, strlen(note) == 0)
    {
        printf("Usage: tag <filename> <note>\n");
    }

    FILE *f = fopen(".file_tags", "a");
    if (f == NULL)
    {
        perror("Error! File not found");
        return;
    }

    fprintf(f, "%s : %s", filename, note);
    fclose(f);
    printf("Tag added succesfully");
}

// searching tag file

void get_tag(const char *filename, char *buffer, size_t size)
{
    FILE *f = fopen(".file_tags", "r");
    if (f == NULL)
    {
        perror("Error while opening file");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), f))
    {
        char *colon = strchr(line, ":");
        if (colon != NULL)
        {
            *colon = '\0';
            if (strcmp(line, filename) == 0)
            {
                strncpy(buffer, colon + 1, size);
                buffer[strcspn(buffer, "\n")] = '\0';
                fclose(f);
                return;
            }
        }
    }
    fclose(f);
}

int main(int argc, char *argv[])
{
    char input[512];
    char current_path[512];

    while (1)
    {
        getcwd(current_path, sizeof(current_path));
        printf("%s: ", current_path);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *command = strtok(input, " ");
        char *arg = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");
        if (command == NULL)
        {
            continue;
        }

        if (strcmp(command, "makefile") == 0)
        {
            create_file(arg);
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "list") == 0)
        {
            list_file();
        }
        else if (strcmp(command, "del") == 0)
        {
            file_delete(arg);
        }
        else if (strcmp(command, "edit") == 0)
        {
            edit_file(arg);
        }
        else if (strcmp(command, "change_dir") == 0)
        {
            change_dir(arg);
        }
        else if (strcmp(command, "read") == 0)
        {
            read_file(arg);
        }
        else if (strcmp(command, "rename") == 0)
        {
            rename_file(arg, arg2);
        }
        else if (strcmp(command, "restart") == 0)
        {
            restart_program(argc, argv);
        } else if(strcmp(command, "search_tag") == 0){
            get_tag(arg, arg2);
        }
        else
        {
            printf("Unkown command");
        }
    }
}
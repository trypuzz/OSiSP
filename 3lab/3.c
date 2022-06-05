#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>
#define buffer_len 16777216

char* scriptName;
int max_numb_proc;
int cur_num_proc;
//unsigned long long int perem=0;

char fixed_buf[buffer_len];


void errors_printing(const char *scr_name, const char *msg_err, const char *f_name)
{
    fprintf(stderr, "%s: %s %s\n", scr_name, msg_err, (f_name) ? f_name : "");
} 

void words_counting(char* filepath)
{
    errno = 0;
    FILE *file;
    int sym = 0;
    struct stat st;
    int words_count = 0;
    int bytes_count = 0;
    int flag = 1;
    //unsigned long long int perem = 0;
    file = fopen(filepath, "r");
    if(file == NULL)
    {
        fprintf(stderr, "%d : %s : %s :%s\n", getpid(), scriptName, strerror(errno), filepath);
        errno = 0;
        return;
    }

    size_t nread;
    /*nread = fread(fixed_buf, sizeof(char), buffer_len, file);
    if(!feof(file) && nread != buffer_len)
    {
        fprintf(stderr, "%d : %s : %s : %s\n", getpid(), scriptName, strerror(errno), filepath);
        errno = 0;
    }
    errno = 0;
    /*for(int i = 0; i < nread; i++)
        {
                bytes_count++;
                if (isspace(fixed_buf[i]))
                {
                        flag = 1;
                }
                else
                {
                        words_count += flag;
                        flag = 0;
                }
        }
*/
errno=0;
    while(nread = fread(fixed_buf, sizeof(char), buffer_len, file))
    {
	for(int i = 0; i < nread; i++)
    	{
        	bytes_count++;
        	if (isspace(fixed_buf[i]))
        	{
            		flag = 1;
        	}
        	else
        	{
            		words_count += flag;
            		flag = 0;
		}
    	}
    }
   //errno = 0;
    if (fclose(file) == EOF)
 		fprintf(stderr, "%d : %s : %s : %s\n", getpid(), scriptName, strerror(errno), filepath);
    errno = 0;
    printf("%d: %s %d %d\n",getpid(), filepath, bytes_count, words_count);
}

void process_get(char* filepath)
{
    pid_t pid;
    pid_t wait_result;

    if(cur_num_proc >= max_numb_proc)
    {
        wait_result = wait(NULL);
        if(wait_result == -1)
        {
            fprintf(stderr, "%d : %s : %s :%s\n", getpid(), scriptName, strerror(errno), filepath);
 			exit(0);
        }
        if(wait_result > 0)
        {
            cur_num_proc--;
        }
    }

    if(cur_num_proc < max_numb_proc)
    {
        pid = fork();

        if(pid == -1)
        {
            fprintf(stderr, "%d : %s : %s :%s\n", getpid(), scriptName, strerror(errno), filepath);
            errno = 0;
            return;
        }
        if(pid == 0)
        {
            words_counting(filepath);
            exit(0); 	
        }
        if(pid > 0)
        {
            cur_num_proc++;
        }
    }
}

void dirrectory_show(char* dir_name)
{
    struct dirent *dir_entry;
    char* fullpath;
    DIR *dir = opendir(dir_name);

    if (!dir) 
    {
        errors_printing(scriptName, strerror(errno), dir_name);      
        return;
    }

    while(dir_entry = readdir(dir))
    {
        fullpath = (char*)malloc(sizeof(char)*(strlen(dir_name)+strlen(dir_entry->d_name)+2));
        strcpy(fullpath, dir_name);
        strcat(fullpath, "/");
        strcat(fullpath, dir_entry->d_name);

        if(dir_entry->d_type == DT_REG)
        {
            process_get(fullpath);
        }
        else
        {
            if(dir_entry->d_type == DT_DIR && strcmp(".", dir_entry->d_name) && strcmp("..", dir_entry->d_name))
            {
                dirrectory_show(fullpath);
            }
        }
        errno = 0;
    }

    if(errno != 0)
    {
        errors_printing(scriptName, strerror(errno), fullpath);
    }

    if(closedir(dir) == -1)
    {
        errors_printing(scriptName, strerror(errno), fullpath); 
        free(fullpath); 
    }
}


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Not enough arguments!\n");
        return 0;
    }
    scriptName = basename(argv[0]);
    char* dir_name = argv[1];
    max_numb_proc = atoi(argv[2]);
    cur_num_proc = 1;
    int ret;

    dirrectory_show(dir_name);

    while(cur_num_proc != 0)
	{
		ret = wait(NULL);
		cur_num_proc--;
	}

    return 0;
}

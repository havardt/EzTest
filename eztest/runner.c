/**
 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#define TEST_RUNNER

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <getopt.h>
#include <assert.h>
#include <stdio.h>
#include <locale.h>
#include "eztest.h"

/* Macros */

#define __PROGRAM_NAME__ "EzTest"
#define __PROGRAM_VERSION__ "2.1.0"
#define __PROGRAM_AUTHOR__ "havardt"
#define __PROGRAM_WEBSITE__ "https://github.com/havardt/EzTest"

#define DEFAULT_OPTIONS { .no_color = false, .timer = false, .quiet = false, .skip = false }


/* Prototypes */

void print_usage ( FILE            *               );
int  parse_opt   ( struct  options *, int          );
int  handle_opts ( struct  options *, int, char ** );


/* Globals */

const struct option long_opts[] = {
    {"help"    , no_argument      , NULL, 'h'},
    {"version" , no_argument      , NULL, 'v'},
    {"no-color", no_argument      , NULL, 'c'},
    {"timer"   , no_argument      , NULL, 't'},
    {"quiet"   , no_argument      , NULL, 'q'},
    {"skip"    , required_argument, NULL, 's'},
    {0}
};

struct options opts = DEFAULT_OPTIONS;


int main(int argc, char **argv)
{
    if(handle_opts(&opts, argc, argv) != RESULT_OK)
    {
        return EXIT_FAILURE;
    }
    
    setlocale(LC_ALL, ""); // Needed to print wide chars/ strings.

    return (eztest_run(&opts) == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

/** 
 * Prints program usage.
 * 
 * @param fd A file descriptor representing the output location.
 */
void print_usage(FILE *fd)
{
    assert(fd != NULL);

    fprintf(fd, "\nUsage: %s [OPTIONS]\n\n"
                "Options:\n"
                " -v  --version   Print version number.\n"
                " -h  --help      Print help information.\n"
                " -c  --no-color  Only use default color when printing to screen.\n"
                " -t  --timer     Display execution time for each test.\n"
                " -q  --quiet     No output.\n"
                " -s  --skip      Skips all tests in the passed list of test suits.\n\n",
                __PROGRAM_NAME__);
}

/**
 * Parse application options.
 *  
 * @param opts Pointer to current options.
 * @param opt  The option to parse.
 * @return: On success @code RESULT_OK @endcode; otherwise @code RESULT_ERR @endcode .
 */
int parse_opt(struct options *opts, const int opt)
{
    switch(opt)
    {
        case 'v':
            printf("%s version %s\n", __PROGRAM_NAME__, __PROGRAM_VERSION__);
            exit(EXIT_SUCCESS);

        case 'h':
            print_usage(stdout); 
            exit(EXIT_SUCCESS);

        case 'c':
            opts->no_color = true;
            break;

        case 't':
            opts->timer = true;
            break;

        case 'q':
            opts->quiet = true;
            break;
        
        case 's':
            opts->skip = true;
            skip_list = optarg;
            break;

        default:
            return RESULT_ERR;
    }
    return RESULT_OK;
}

/**
 * Processes application options by parsing given options and setting the 
 * appropriate options.
 * 
 * @param opts Pointer to application options struct.
 * @param argc Argument count.
 * @param argv Arguments as strings.
 * @return @code RESULT_OK @endcode on success and @code RESULT_ERR @endocode on failure. 
 *         Failure may occur if the user has given an unknown option. 
 */
int handle_opts(struct options *opts, const int argc, char **argv)
{
    int opt, opt_index;
    while((opt = getopt_long(argc, argv, "vhctqs:", long_opts, &opt_index)) != -1)
    {
        if(parse_opt(opts, opt) != RESULT_OK)
        {
            print_usage(stderr); 
            return RESULT_ERR;
        }
    }
    return RESULT_OK;
}


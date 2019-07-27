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
#include "options.h"
#include "eztest.h"
#include "about.h"
#include "../common/result.h"

/* Macros */

#define DEFAULT_OPTIONS { .no_color = false }


/* Prototypes */

void      print_usage ( FILE            *               );
const int parse_opt   ( struct  options *, int          );
const int handle_opts ( struct  options *, int, char ** );


/* Globals */

const struct option long_opts[] = {
    {"help"     , 0, NULL, 'h'},
    {"version"  , 0, NULL, 'v'},
    {"no-color" , 0, NULL, 'c'},
    {0}
};


int main(int argc, char **argv)
{
    struct options opts = DEFAULT_OPTIONS;

    if(handle_opts(&opts, argc, argv) != RESULT_OK)
    {
        return EXIT_FAILURE;
    }

    eztest_run(&opts);
    
    return EXIT_SUCCESS;
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
                " -v --version  Print version number.\n"
                " -h --help     Print help information.\n"
                " -c --no-color Only use default color when printing to screen.\n\n",
                __PROGRAM_NAME__);
}

/**
 * Parse application options.
 *  
 * @param opts Pointer to current options.
 * @param opt  The option to parse.
 * @return: On success @code RESULT_OK @endcode; otherwise @code RESULT_ERR @endcode .
 */
const int parse_opt(struct options *opts, const int opt)
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
const int handle_opts(struct options *opts, const int argc, char **argv)
{
    int opt, opt_index;
    while((opt = getopt_long(argc, argv, "vhc", long_opts, &opt_index)) != -1)
    {
        if(parse_opt(opts, opt) != RESULT_OK)
        {
            print_usage(stderr); 
            return RESULT_ERR;
        }
    }
    return RESULT_OK;
}


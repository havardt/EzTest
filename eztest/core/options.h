/**
* 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#include <stdbool.h>

#ifndef OPTIONS_H
#define OPTIONS_H

/** Represents the application options for EzTest. */
struct options
{
    /** When set to @code true @endcode only default color is used when printing. */
    bool no_color;
    /** When set to @code true @endcode the test execution time is displayed for each test */
    bool timer;
};

#endif


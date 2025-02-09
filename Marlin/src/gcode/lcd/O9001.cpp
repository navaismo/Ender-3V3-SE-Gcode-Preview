#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"

/**
 *O9001: Update Printing Details JOB from OctoPrint in LCD
 *
 *Params:
 *   O9001|ET:10:20:59|PG:45|CL:10
 *    ET: Print Time Left
 *    PG: Progress
 *    CL: Current Layer
 */


void GcodeSuite::O9001()
{

    if (parser.string_arg && parser.string_arg[0] != '\0')
    {

        // Variables for storing the parsed values
        char eta_str[10] = {0}, progress_str[10] = {0}, current_layer_str[10] = {0};

        // Parse the O9001 line
        char *token = strtok(parser.string_arg, "|"); // Divided by "|"
        while (token != NULL)
        {
            if (strncmp(token, "ET:", 3) == 0)
            {
                strncpy(eta_str, token + 3, sizeof(eta_str) - 1);
            }
            else if (strncmp(token, "PG:", 3) == 0)
            {
                strncpy(progress_str, token + 3, sizeof(progress_str) - 1);
            }
            else if (strncmp(token, "CL:", 3) == 0)
            {
                strncpy(current_layer_str, token + 3, sizeof(current_layer_str) - 1);
            }
            token = strtok(NULL, "|"); // Read next value
        }

        // Save values ​​in global variables
        if (eta_str[0] != '\0' && progress_str[0] != '\0' && current_layer_str[0] != '\0')
            DWIN_SetPrintingDetails(eta_str, progress_str, current_layer_str);
    }
}

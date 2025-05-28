#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"
#include "../../module/probe.h"

/**
 * M5000: Set Printing Details JOB from OctoPrint in LCD
 *
 * Params:
 *    - Filename
 *    - Print Time
 *    - Print Time Left
 *    - Total Layers
 *    - Current Layer
 *    - B64 thumbnail
 *    - Progress
 */
bool O9000_collecting = false;
char filename[50] = {0};
char print_time[50] = {0};
char ptime_left[50] = {0};
char total_layers[50] = {0};
char curr_layer[50] = {0};
char thumbnail[50] = {0};
char progress[10] = {0};
char param_value[50] = {0};
Probe myprobe;

const char *getParsedValue(char *str)
{
  const char delimiter[] = "|";
  char *token;

  // Split the string to get the first part (before '|')
  token = strtok(str, delimiter);

  // Split again to get the second part (after '|')
  token = strtok(NULL, delimiter); // NULL tells strtok to continue with the same string

  if (token != NULL)
  {
    // Copy the second part into the param_value variable
    strncpy(param_value, token, sizeof(param_value) - 1);
    param_value[sizeof(param_value) - 1] = '\0'; // Ensure null termination
  }

  return param_value;
}

void GcodeSuite::O9000()
{

  if (parser.string_arg && parser.string_arg[0] != '\0')
  {
    char *my_string = parser.string_arg;
    //SERIAL_ECHOLNPAIR("Received: ", my_string);

    if (strcmp(my_string, "SC|") == 0)
    {
      // Received all params lets render
      TERN_(DWIN_CREALITY_LCD, DWIN_OctoPrintJob(filename, print_time, ptime_left, total_layers, curr_layer, thumbnail, progress));
      SERIAL_ECHOLN("O9000 sc-rendered");
    }
    else if (strstr(my_string, "SFN|") != NULL)
    {
      // Set FileName
      strncpy(filename, getParsedValue(my_string), sizeof(filename) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 1 filename set: ", filename);
    }
    else if (strstr(my_string, "SPT|") != NULL)
    {
      // Set Print Time
      strncpy(print_time, getParsedValue(my_string), sizeof(print_time) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 2 print_time set: ", print_time);
      
    }
    else if (strstr(my_string, "UPT|") != NULL)
    {
      // Update Print Time
      strncpy(print_time, getParsedValue(my_string), sizeof(print_time) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 2 print_time set: ", print_time);
      DWIN_OctoSetPrintTime(print_time);
    }
    else if (strstr(my_string, "SET|") != NULL)
    {
      // Set Print Time Left
      strncpy(ptime_left, getParsedValue(my_string), sizeof(ptime_left) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 3 ptime_left set: ", ptime_left);
    }
    else if (strstr(my_string, "STL|") != NULL)
    {
      // Set Total Layers
      strncpy(total_layers, getParsedValue(my_string), sizeof(total_layers) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 4  total_layers set: ", total_layers);
    }
    else if (strstr(my_string, "SCL|") != NULL)
    {
      // Set Current Layer
      strncpy(curr_layer, getParsedValue(my_string), sizeof(curr_layer) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 5 curr_layer set: ", curr_layer);
    }
    else if (strstr(my_string, "SPP|") != NULL)
    {
      // Set Progress
      strncpy(progress, getParsedValue(my_string), sizeof(progress) - 1);
      //SERIAL_ECHOLNPAIR("Parameter 6 progress set: ", progress);
    }
    else if (strstr(my_string, "PF|") != NULL)
    {
      // Print Finished
      //SERIAL_ECHOLN("Print Finished");
      TERN_(DWIN_CREALITY_LCD, DWIN_OctoJobFinish());
    }
    else if (strstr(my_string, "BU1|") != NULL)
    {
      
     // TERN_(DWIN_CREALITY_LCD, DWIN_OctoShowGCodeImage());
    }
     else if (strstr(my_string, "OCON|") != NULL)
    {
      serial_connection_active = true;
      TERN_(DWIN_CREALITY_LCD, Goto_MainMenu());
    }
     else if (strstr(my_string, "OCOFF|") != NULL)
    {
      serial_connection_active = false;
      TERN_(DWIN_CREALITY_LCD, Goto_MainMenu());
    }
    else if (strstr(my_string, "ONGPIC|") != NULL)
    {
      Show_Default_IMG = true;
      
    }
    else if (strstr(my_string, "OFFGPIC|") != NULL)
    {
      Show_Default_IMG = false;
      
    }
    else if (strstr(my_string, "PROBEON|") != NULL)
    {
      //gcode.process_subcommands_now_P(PSTR("G28")); //home
      gcode.process_subcommands_now_P(PSTR("G0 Z40 F7000"));
      delay(200);
      delay(200);
      bool res = myprobe.deploy();
      if (!res)
      {
        SERIAL_ECHOLN("Probe deployed");
      }
      else
      {
        SERIAL_ECHOLN("Probe failed to deploy");
      }
      
       
    }
    else if (strstr(my_string, "PROBEOFF|") != NULL)
    {
      //gcode.process_subcommands_now_P(PSTR("G28")); //home
      gcode.process_subcommands_now_P(PSTR("G0 Z40 F7000"));
      delay(200);
      delay(200);
      bool res = myprobe.stow();
      if (!res)
      {
        SERIAL_ECHOLN("Probe stowed");
      }
      else
      {
        SERIAL_ECHOLN("Probe failed to stow");
      }
    }
    else
    {
      SERIAL_ECHOLN("Invalid Command Argument...");
    }
  }
  else
  {
    SERIAL_ECHOLN("Empty Command Argument...");
  }

  // TODO THUMBNAIL RECBUFFER
}
#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"


/**
 * O9002: Fill Marlin Variable ImageMap with incoming data
 *
 * Params:
 *   O9002|data1,data2,data3...data20
 */
void GcodeSuite::O9002() {
    if (parser.string_arg && parser.string_arg[0] != '\0') {
        // Tokenize the incoming string argument
        char *token = strtok(parser.string_arg, ",");
        int index = 0;

        // Parse each token and store it in the ImageMap array
        while (token != nullptr && index < IMAGE_MAP_SIZE) {
            ImageMap[index] = static_cast<uint16_t>(atoi(token));
            token = strtok(nullptr, ",");
            index++;
        }

        // Handle any remaining tokens or errors if necessary
        if (index >= IMAGE_MAP_SIZE) {
            SERIAL_ECHOLN("ImageMap array is full, some data may be lost");
        }else{
            SERIAL_ECHOLN("ImageMap array has been filled successfully");
            DWIN_RenderImageMap();
        }
    }
}
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
 *   O9002|END
 */
void GcodeSuite::O9002() {
    if (parser.string_arg && parser.string_arg[0] != '\0') {
        // Check for end-of-data command
        if (strcmp(parser.string_arg, "END") == 0) {
            SERIAL_ECHOLN("ImageMap array has been filled successfully");
            DWIN_RenderOctoImageMap();
            return;
        }

        // Tokenize the incoming string argument using '|' as the delimiter
        char *token = strtok(parser.string_arg, "|");
        while (token != nullptr) {
            // Check if the token is the end-of-data command
            if (strcmp(token, "END") == 0) {
                SERIAL_ECHOLN("ImageMap array has been filled successfully");
                DWIN_RenderOctoImageMap();
                return;
            }

            // Tokenize the pixel data using ',' as the delimiter
            char *pixel_token = strtok(token, ",");
            int index = 0;

            // Parse each pixel token and store it in the ImageMap array
            while (pixel_token != nullptr && index < OctoIMAGE_MAP_SIZE) {
                OctoImageMap[index] = static_cast<uint16_t>(atoi(pixel_token));
                pixel_token = strtok(nullptr, ",");
                index++;
            }

            // Handle any remaining tokens or errors if necessary
            if (index >= OctoIMAGE_MAP_SIZE) {
                SERIAL_ECHOLN("ImageMap array is full, some data may be lost");
            }

            // Get the next token
            token = strtok(nullptr, "|");
        }
    }
}
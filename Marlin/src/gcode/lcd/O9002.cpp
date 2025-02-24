#include "../../inc/MarlinConfig.h"
#include "../../MarlinCore.h"
#include "../gcode.h"
#include "../parser.h"
#include "../../lcd/dwin/e3v2/dwin.h"
#include "../../lcd/marlinui.h"
#include "../../lcd/dwin/e3v2/lcd_rts.h"





static uint16_t current_line = 0;  // Track the current line
static uint16_t received_pixels = 0; // Track received pixels for the line


void GcodeSuite::O9002() {
    
    if (parser.string_arg && parser.string_arg[0] != '\0') {
        // START: Initialize the image reception
        if (strncmp(parser.string_arg, "START", 5) == 0) {
            Show_Default_IMG = false;
            initializeImageMap();
            Clear_Title_Bar();
            Draw_OctoTitle("Receiving Thumbnail, wait...");
            current_line = 0;
            received_pixels = 0;
            SERIAL_ECHOLN("O9002 START: Ready to receive lines.");
            return;
        }

        // END: Finalize the image transmission
        if (strcmp(parser.string_arg, "END") == 0) {
            SERIAL_ECHOLN("O9002 thumbnail-rendered");
            DWIN_RenderOctoTitle();
            return;
        }

        // CHUNK: Receive a portion of a line
        if (strncmp(parser.string_arg, "CHUNK", 5) == 0) {
            char *arg = parser.string_arg + 5;
            while (*arg && isspace(*arg)) arg++;

            // Tokenize the string for chunk parsing
            char *token = strtok(arg, ",");
            if (token != nullptr) {
                uint16_t line_number = atoi(token);

                // Tokenize for pixel offset
                token = strtok(nullptr, "|");
                if (token != nullptr) {
                    uint16_t pixel_offset = atoi(token);

                    // Start parsing the pixel data
                    token = strtok(nullptr, ",");
                    while (token != nullptr && pixel_offset < OctoIMAGE_WIDTH) {
                        // Ensure proper color value assignment
                        OctoImageLine[pixel_offset] = static_cast<uint16_t>(atoi(token));
                        received_pixels++;
                        pixel_offset++;

                        token = strtok(nullptr, ",");
                    }

                    // Debugging output for checking chunk data
                    // SERIAL_ECHOLNPAIR("O9002 CHUNK: Received line ", line_number);
                    // SERIAL_ECHOLNPAIR("Current received_pixels: ", received_pixels);

                    // Check if entire line has been received
                    if (received_pixels >= OctoIMAGE_WIDTH) {
                        // Call render function for the line
                        DWIN_RenderOctoLine(line_number);
                        received_pixels = 0; // Reset for next line
                    }
                }
            }
            return;
        }
    }
}
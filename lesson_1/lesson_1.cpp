#include <gpiod.h>
#include <iostream>
#include <unistd.h>  //for sleep

int main() {
    const char* chipname = "gpiochip0";    //the GPIO chip
    unsigned int line_num = 17;            // GPIO pin 17

    //Open the GPIO chip
    struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        return 1;
    }

    // Get the GPIO line
    struct gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    if (!line) {
        std::cerr << "Failed to get GPIO line" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the line as output
    if (gpiod_line_request_output(line, "gpio_led", 0) < 0) {
        std::cerr << "Failed to set GPIO line as output" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Turn the LED on for 3 seconds
    gpiod_line_set_value(line, 1); //turn LED on
    sleep(5);                      //wait 3 seconds
    gpiod_line_set_value(line, 0);  //turn LED off
    

    /*//Loop
    for (int i = 0; i < 5; i++) {
        gpiod_line_set_value(line, 1); //set high
        sleep(1);
        gpiod_line_set_value(line, 0); //set low
        sleep(1);
    }
    */

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return 0;
}
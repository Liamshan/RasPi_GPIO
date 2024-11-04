#include <gpiod.h>
#include <iostream>
#include<unistd.h> // for sleep

int main() {
    const char* chipname = "gpiochip0";  // This is the GPIO chip (usually gpiochip0)
    unsigned int line_num = 17;          // This is GPIO pin 17 (BCM numbering)

    // Open GIPO chip
    struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        return 1;
    }

    //Get the GPIO line
    struct gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    if (!line) {
        std::cerr << "failed to get GPIO line" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the line as output
    if (gpiod_line_request_output(line, "gpio_led", 0) < 0) {
        std::cerr << "Failed to set GPIO line as output" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Blink the LED 10 times
    for (int i = 0; i < 10; i++) {
        gpiod_line_set_value(line, 1); // Turn LED on
        sleep(1);                      // Wait 1 second
        gpiod_line_set_value(line, 0); // Turn LED off
        sleep(1);                      // Wait 1 second
    }

    // Release the line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);

    return 0;
}
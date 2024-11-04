#include <gpiod.h>
#include <iostream>
#include <unistd.h> // for sleep

int main() {
    const char* chipname = "gpiochip0";    // The GPIO chip
    unsigned int button_pin = 27;           // GPIO pin 27 for button input (BCM numbering)
    unsigned int led_pin = 17;              // GPIO pin 17 for LED output (BCM numbering)

    // Open GPIO chip
    struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        return 1;
    }

    // Get the GPIO line for the button
    struct gpiod_line* button_line = gpiod_chip_get_line(chip, button_pin);
    if (!button_line) {
        std::cerr << "Failed to get GPIO line for button" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Get the GPIO line for the LED
    struct gpiod_line* led_line = gpiod_chip_get_line(chip, led_pin);
    if (!led_line) {
        std::cerr << "Failed to get GPIO line for LED" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the button line as input with pull-up resistor
    if (gpiod_line_request_input_flags(button_line, "gpio_button", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) < 0) {
        std::cerr << "Failed to set GPIO line as input for button with pull-up" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the LED line as output
    if (gpiod_line_request_output(led_line, "gpio_led", 0) < 0) {
        std::cerr << "Failed to set GPIO line as output for LED" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }
    //______________________________________________________________________________
    //Test the LED
    gpiod_line_set_value(led_line, 1);
    sleep(2);
    gpiod_line_set_value(led_line, 0);
    sleep(1);

    // Monitor the button and control the LED with debounce
    std::cout << "Press the button to turn the LED on or off." << std::endl;
    int last_button_state = 1; //last state of button (1 = not pressed, 0 = pressed)
    
    while (true) {
        //read current button state
        int button_state = gpiod_line_get_value(button_line);
        if (button_state < 0) {
            std::cerr << "Failed to read button state" << std::endl;
            gpiod_chip_close(chip);
            return 1;
        }
        // If the button is pressed (active low)
        if (button_state == 0 && last_button_state == 1) {
            //wait 50 milliseconds
            usleep(50000);
            //re-read the button state to check if it is still pressed
            button_state = gpiod_line_get_value(button_line);

            if (button_state < 0) {
                std::cerr << "Failed to read button state" << std::endl;
                gpiod_chip_close(chip);
                return 1;
            }

            //if the button is still pressed after debounce
            if (button_state == 0) {
                //Read the current LED state
                int led_state = gpiod_line_get_value(led_line);
                if (led_state < 0) {
                    std::cerr << "failed to read LED state" << std::endl;
                    gpiod_chip_close(chip);
                    return 1;
                }
                //Toggle the LED state
                if(led_state == 0) {
                    gpiod_line_set_value(led_line, 1); //Turn LED on
                } else {
                    gpiod_line_set_value(led_line, 0); //Turn LED off
                }
            }
        }


        // Update the last button state
        last_button_state = button_state;

        // Small delay to avoid constant polling
        usleep(10000); // 10 milliseconds


    }

/* alright what we're looking at. get the gpio line for  */





    // Release the lines and close the chip
    gpiod_line_release(button_line);
    gpiod_line_release(led_line);
    gpiod_chip_close(chip);

    return 0;
}

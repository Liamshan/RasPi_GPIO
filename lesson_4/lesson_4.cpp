#include <gpiod.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>

int main() {
    // Initialize the wiringPi library
    wiringPiSetup();

    // Define GPIO pin for the servo (using wiringPi numbering)
    int servo_pin = 1; // GPIO 18 (BCM) is wiringPi pin 1

    // Set up software PWM on the servo pin
    if (softPwmCreate(servo_pin, 0, 200) != 0) {
        std::cerr << "Failed to initialize PWM on servo pin" << std::endl;
        return 1;
    }

    // Sweep the servo back and forth
    std::cout << "Sweeping the servo motor back and forth." << std::endl;
    while (true) {
        // Move to 0 degrees
        softPwmWrite(servo_pin, 5); // 5% duty cycle (0 degrees)
        sleep(1);

        // Move to 90 degrees
        softPwmWrite(servo_pin, 15); // 15% duty cycle (90 degrees)
        sleep(1);

        // Move to 180 degrees
        softPwmWrite(servo_pin, 25); // 25% duty cycle (180 degrees)
        sleep(1);
    }
    return 0;
}







/*OK, the purpose of this program is to actuate a servo to pre-defined locatiuons
we are learning the different libraries and methods for programming the pi
this may be more complicated than it needs to be but it's good to see the
different levels of abstraction. like the wiringpi library looks like it's very 
similar to the arduino, which might be fine, but the libgpiod might be cloder 
to the c low level, bare metal stuff. IDK what is better, but seems like
my long term goal is to make my own tailored and use case specific versions of
things like the raspi and the esp32 so i should be moving closer to bare metal, 
but, to start i need to make sure i understand the basics of these devices and functions. 
so i'll start by finishing this chatgpt tutorial, then maybe try to program some shit on
my own to get really comfortable with doing the arduino type stuff, then start messing with
bare metal applications, so eventually, i can build my own board to ddo things
in the meantime of learning the bare metal stuff on existing boards, i can be
learning how to create PCBs that do simple functions, like maybe a motor driver
or blinking whatever, that could be really cool, then we can move up to adding micro-processors
to our boards. but for now, we are just digitally moving a servo actuator. */
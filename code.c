#include <stdio.h>
#include <stdbool.h>
#include "pico/stdio.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"


/*-- Pin definitions --*/

// Grid row select
#define ROW_A 7
#define ROW_B 8
#define ROW_C 9
#define ROW_D 10

// Grid column data
#define DAT0 0
#define DAT1 1
#define DAT2 2
#define DAT3 3

// Buffer select
#define SEL_A 4
#define SEL_B 5
#define SEL_C 6

// Screen select
#define SCR_A 11
#define SCR_B 12
#define SCR_C 13
#define SCR_D 14
#define SCR10 15

// Segments
#define SEG_P 26
#define SEG_G 22
#define SEG_F 21
#define SEG_E 20
#define SEG_D 19
#define SEG_C 18
#define SEG_B 17
#define SEG_A 16

// Other pins
#define CLR_BTN 28
#define CLR_LED 27

/*-- Other definitions --*/
#define LOW false
#define HIGH true
#define INPUT true
#define OUTPUT false
#define LONG_PRESS 1000
#define SLEEP_TIME 10000000

// these initialize to all 0's by default
int accumulator[11];
int totalizer[11];
int key_down[11];
int addiator[10];
int last_addiator[10];
int rollover_switch[10];

int op_counter = 0;
int totalizer_dp = 0;
int accumulator_dp = 0;
int extra_btns = 0;
int nop = 0; // How many cycles past with no input

bool view_totalizer = false;
bool view_counter = false;

int ttlzr_pressed = 0;
int op_ctr_pressed = 0;
int dec_pressed = 0;
bool op_ctr_decimal = false;


/* --A--
 * F   B
 * --G--
 * E   C
 * --D--  p
 */


int segments(int value) {
    switch(value) {
        case 0:
            return 0b00111111; // Segments: . . F E D C B A
        case 1:
            return 0b00000110; // Segments: . . . . . C B .
        case 2:
            return 0b01011011; // Segments: . G . E D . B A
        case 3:
            return 0b01001111; // Segments: . G . . D C B A
        case 4:
            return 0b01100110; // Segments: . G F . . C B .
        case 5:
            return 0b01101101; // Segments: . G F . D C . A
        case 6:
            return 0b01111101; // Segments: . G F E D C . A
        case 7:
            return 0b00000111; // Segments: . . . . . C B A
        case 8:
            return 0b01111111; // Segments: . G F E D C B A
        case 9:
            return 0b01100111; // Segments: . G F . . C B A
        case 10:
            return 0b01011100; // Segments: . G . E D C . .  -  Offset 0's
        default:
            if((nop / LONG_PRESS) % 2)
                return 0b00011011; // Segs: . . . E D . B A  -  Something's wrong

            return 0b00101101; // Segments: . . F . D C . A  -  Something's wrong
    }
}

/*-- Functions --*/
void output_enable(int which) {
    // Selects which buffer should be read
    gpio_put(SEL_A, which & 0b001);
    gpio_put(SEL_B, which & 0b010);
    gpio_put(SEL_C, which & 0b100);

    sleep_us(10); // Chips need time to react
}

void write_digit(int digit) {
    // Writes a number to a given screen
    if(digit == 10) {
        gpio_put(SCR_A, LOW);
        gpio_put(SCR_B, LOW);
        gpio_put(SCR_C, LOW);
        gpio_put(SCR_D, LOW);
        gpio_put(SCR10, HIGH);
    } else {
        gpio_put(SCR_A, digit & 0b0001);
        gpio_put(SCR_B, digit & 0b0010);
        gpio_put(SCR_C, digit & 0b0100);
        gpio_put(SCR_D, digit & 0b1000);
        gpio_put(SCR10, LOW);
    }

    int which[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int which_sum = 0;
    if(view_totalizer) {
        // copy totalizer into 'which'
        for(int x = 0; x < 11; x += 1) {
            which[x] = totalizer[x];
            which_sum += which[x];
        }
    } else if(view_counter) {
        // convert number to list, right aligned
        int op2 = op_counter;
        for(int x = 0; x < 11; x += 1) {
            which[10 - x] = op2 % 10;
            op2 /= 10;
            which_sum += which[x];
        }
    } else {
        // copy accumulator into 'which'
        for(int x = 0; x < 11; x += 1) {
            which[x] = accumulator[x];
            which_sum += which[x];
        }
    }

    sleep_us(10); // Let chips react
    // Convert digit to segments, right to left
    int seg;
    bool off;

    if(which_sum) {
        seg = segments(which[10 - digit]);
    } else {
        seg = segments(10);
    }

    if((view_totalizer && digit == 0) ||
       (view_totalizer && digit == totalizer_dp != 0) ||
       (!view_totalizer && digit == accumulator_dp != 0) ||
       (view_counter) ||
       (op_ctr_decimal && ((1 << digit) & op_counter))
    ) {
        /* Decimal should be shown if:
         * - Totalizer is shown and current digit is 0,
         * - Totalizer is shown and the digit is where the decimal place is (0 is ignored)
         * - Accumulator is shown and the digit is where the decimal place is (0 is ignored)
         * - Viewing the operation counter
         * - Operation counter is in binary mode
         */
        seg |= 0b10000000;
    }

    switch(digit) {
        case 10:
        case 9:
        case 8:
        case 4:
        case 3:
        case 2:
            seg ^= 0b11111111; // Flip bits for common cathode
            off = HIGH;
            break;
        default:
            off = LOW;
    }

    int pins[8] = {
        SEG_A, SEG_B, SEG_C, SEG_D,
        SEG_E, SEG_F, SEG_G, SEG_P
    };
    for(int x = 0; x < 8; x += 1) {
        if(seg & 1) {
            gpio_put(pins[x], !off);
            sleep_us(5);
            gpio_put(pins[x], off);
            // Brightness should be the same for every segment, so one segment is lit at a time
        }
        seg >>= 1;
    }
}

void get_rollover_switches() {
    // Collects rollover switch positions
    output_enable(2);
    rollover_switch[0] = gpio_get(DAT3); // Digit 9
    rollover_switch[1] = gpio_get(DAT2); // Digit 8

    output_enable(6);
    rollover_switch[2] = gpio_get(DAT3); // Digit 7
    rollover_switch[3] = gpio_get(DAT2); // Digit 6
    rollover_switch[4] = gpio_get(DAT1); // Digit 5
    rollover_switch[5] = gpio_get(DAT0); // Digit 4

    output_enable(5);
    rollover_switch[6] = gpio_get(DAT3); // Digit 3
    rollover_switch[7] = gpio_get(DAT2); // Digit 2
    rollover_switch[8] = gpio_get(DAT1); // Digit 1
    rollover_switch[9] = gpio_get(DAT0); // Digit 0
}


bool add_stuff() {
    for(int x = 0; x < 10; x += 1)
        addiator[x] = 0;
    extra_btns = 0;

    output_enable(4);
    for(int row = 0; row < 10; row += 1) {
        gpio_put(ROW_A, row & 0b0001);
        gpio_put(ROW_B, row & 0b0010);
        gpio_put(ROW_C, row & 0b0100);
        gpio_put(ROW_D, row & 0b1000);
        sleep_us(5); // Chips need time to react

        addiator[9] = (!gpio_get(DAT3)) * (row + 1) * (bool)((!key_down[9]) & (1 << row)); // Ignore if still pressed from previous cycle
        addiator[8] = (!gpio_get(DAT2)) * (row + 1) * (bool)((!key_down[8]) & (1 << row));
        addiator[7] = (!gpio_get(DAT1)) * (row + 1) * (bool)((!key_down[7]) & (1 << row));
        addiator[6] = (!gpio_get(DAT0)) * (row + 1) * (bool)((!key_down[6]) & (1 << row));

        key_down[9] ^= (key_down[9] & ~(1 << row)) | (gpio_get(DAT3) << row); // Flip bit if mismatch
        key_down[8] ^= (key_down[8] & ~(1 << row)) | (gpio_get(DAT2) << row);
        key_down[7] ^= (key_down[7] & ~(1 << row)) | (gpio_get(DAT1) << row);
        key_down[6] ^= (key_down[6] & ~(1 << row)) | (gpio_get(DAT0) << row);
    }

    output_enable(3);
    for(int row = 0; row < 10; row += 1) {
        gpio_put(ROW_A, row & 0b0001);
        gpio_put(ROW_B, row & 0b0010);
        gpio_put(ROW_C, row & 0b0100);
        gpio_put(ROW_D, row & 0b1000);
        sleep_us(5); // Chips need time to react

        addiator[5] = (!gpio_get(DAT3)) * (row + 1) * (bool)((!key_down[5]) & (1 << row)); // Ignore if still pressed from previous cycle
        addiator[4] = (!gpio_get(DAT2)) * (row + 1) * (bool)((!key_down[4]) & (1 << row));
        addiator[3] = (!gpio_get(DAT1)) * (row + 1) * (bool)((!key_down[3]) & (1 << row));
        addiator[2] = (!gpio_get(DAT0)) * (row + 1) * (bool)((!key_down[2]) & (1 << row));

        key_down[5] ^= (key_down[5] & ~(1 << row)) | (gpio_get(DAT3) << row); // Flip bit if mismatch
        key_down[4] ^= (key_down[4] & ~(1 << row)) | (gpio_get(DAT2) << row);
        key_down[3] ^= (key_down[3] & ~(1 << row)) | (gpio_get(DAT1) << row);
        key_down[2] ^= (key_down[2] & ~(1 << row)) | (gpio_get(DAT0) << row);
    }

    output_enable(1);
    for(int row = 0; row < 10; row += 1) {
        gpio_put(ROW_A, row & 0b0001);
        gpio_put(ROW_B, row & 0b0010);
        gpio_put(ROW_C, row & 0b0100);
        gpio_put(ROW_D, row & 0b1000);
        sleep_us(5); // Chips need time to react

        addiator[1] = (!gpio_get(DAT2)) * (row + 1) * (bool)((!key_down[1]) & (1 << row)); // Ignore if still pressed from previous cycle
        addiator[0] = (!gpio_get(DAT1)) * (row + 1) * (bool)((!key_down[0]) & (1 << row));

        key_down[1] ^= (key_down[1] & ~(1 << row)) | (gpio_get(DAT3) << row); // Flip bit if mismatch
        key_down[0] ^= (key_down[0] & ~(1 << row)) | (gpio_get(DAT2) << row);

        extra_btns |= (!gpio_get(DAT0)) << row; // These are the 'extras' buttons, and we want long press logic
    }
    for(int x = 0; x < 10; x += 1) {
        if(addiator[x]) {
            nop = 0;
            return true;
        }
    }
    if(extra_btns)
        nop = 0;
    return false;
}


void screen_loop() {
    while(true) {
        if(nop > SLEEP_TIME) {
            gpio_put(CLR_LED, nop % 100);
            sleep_ms(100); // Deep sleep for 2nd core
            continue;
        } else if(nop == LONG_PRESS) {
            for(int x = 0; x < 10; x += 1)
                last_addiator[x] = 0;
        }
        for(int x = 0; x < 11; x += 1) {
            write_digit(x);
            sleep_us(10);
        }
        sleep_us(1000); // The more off time the dimmer, but the longer the battery life
    }
}

void loop() {
    int sum = 0;
    if(view_totalizer) {
        for(int x = 0; x < 11; x += 1)
            sum += totalizer[x];
    } else {
        for(int x = 0; x < 11; x += 1)
            sum += accumulator[x];
    }
    gpio_put(CLR_LED, sum);

    if(gpio_get(CLR_BTN)) {
        nop = 0;
        if(view_totalizer) {
            for(int x = 0; x < 11; x += 1)
                totalizer[x] = 0;
            totalizer_dp = 0;
        } else {
            for(int x = 0; x < 11; x += 1)
                accumulator[x] = 0;
            accumulator_dp = 0;
        }
        op_counter = 0;
        return;
    }

    if(add_stuff()) {
        get_rollover_switches();
        view_totalizer = false;
        view_counter = false;

        // Add the addiator to the accumulator
        for(int digit = 9; digit >= 0; digit -= 1) { // Go from back to front because of how rollovers work
            accumulator[digit + 1] += addiator[digit];
            if(!rollover_switch[digit])
                accumulator[digit] += accumulator[digit + 1] / 10; // Do the rollover
            accumulator[digit + 1] %= 10; // Ignore first digit
            if((last_addiator[digit] != addiator[digit]) && addiator[digit]) { // ignore 0's
                last_addiator[digit] = addiator[digit];
                op_counter = 0;
            }
        }

        // The op counter is only meant to give an idea of how many operations have occurred,
        // This is very difficult to implement on a key-driven device because of all the combinations of buttons and timings that occur
        // The original idea was from the Bohn Contex 10, which is NOT key-driven.
        int row = 0;
        for(int x = 0; x < 10; x += 1) {
            if(!row)
                row = accumulator[x];
            if(row != accumulator[x]) {
                row = op_counter + 1; // Not the same keys pressed
                break;
            }
        }
        op_counter = row; // If all keys pressed are on the same row, then thats the multiplier
    } else {
        nop += 1;
    }

    // View totalizer button
    if(extra_btns & 0b010) {
        if(!ttlzr_pressed) {
            view_totalizer = !view_totalizer;
        } else if(ttlzr_pressed == LONG_PRESS) {
            // Add accumulator to super totalizer, clear accumulator
            for(int digit = 11; digit >= 0; digit -= 1) {
                totalizer[digit] += accumulator[digit];
                if(totalizer[digit] > 10) {
                    if(digit)
                        totalizer[digit - 1] += totalizer[digit] / 10;
                    totalizer[digit - 1] %= 10;
                    accumulator[digit] = 0;
                }
            }
        }
        ttlzr_pressed += 1;
    } else {
        ttlzr_pressed = 0;
    }

    // Operation counter button
    if(extra_btns & 0b010) {
        if(!op_ctr_pressed)
            view_counter = !view_counter;
        else if(op_ctr_pressed == LONG_PRESS)
            op_counter = 0;
        op_ctr_pressed += 1;
    } else {
        op_ctr_pressed = 0;
    }

    // Decimal button
    if(extra_btns & 0b100) {
        if(!dec_pressed) {
            if(view_counter) {
                op_ctr_decimal = false;
            } else {
                if(view_totalizer)
                    totalizer_dp = (totalizer_dp + 1) % 11;
                else
                    accumulator_dp = (accumulator_dp + 1) % 11;
            }
        } else if(dec_pressed == LONG_PRESS) {
            op_ctr_decimal = true;
        } else if(dec_pressed == 2 * LONG_PRESS) {
            nop = SLEEP_TIME;
        }
    }
}


void setup() {
    /*-- Setup pins --*/
    // Grid row select
    gpio_set_function(ROW_A, GPIO_FUNC_PIO0);
    gpio_set_function(ROW_B, GPIO_FUNC_PIO0);
    gpio_set_function(ROW_C, GPIO_FUNC_PIO0);
    gpio_set_function(ROW_D, GPIO_FUNC_PIO0);
    gpio_set_input_enabled(ROW_A, OUTPUT);
    gpio_set_input_enabled(ROW_B, OUTPUT);
    gpio_set_input_enabled(ROW_C, OUTPUT);
    gpio_set_input_enabled(ROW_D, OUTPUT);

    // Grid column data
    gpio_set_function(DAT0, GPIO_FUNC_PIO0);
    gpio_set_function(DAT1, GPIO_FUNC_PIO0);
    gpio_set_function(DAT2, GPIO_FUNC_PIO0);
    gpio_set_function(DAT3, GPIO_FUNC_PIO0);
    gpio_set_input_enabled(DAT0, INPUT);
    gpio_set_input_enabled(DAT1, INPUT);
    gpio_set_input_enabled(DAT2, INPUT);
    gpio_set_input_enabled(DAT3, INPUT);

    // Buffer select
    gpio_set_function(SEL_A, GPIO_FUNC_PIO0);
    gpio_set_function(SEL_B, GPIO_FUNC_PIO0);
    gpio_set_function(SEL_C, GPIO_FUNC_PIO0);
    gpio_set_input_enabled(SEL_A, OUTPUT);
    gpio_set_input_enabled(SEL_B, OUTPUT);
    gpio_set_input_enabled(SEL_C, OUTPUT);

    // Screen select
    gpio_set_function(SCR_A, GPIO_FUNC_PIO0);
    gpio_set_function(SCR_B, GPIO_FUNC_PIO0);
    gpio_set_function(SCR_C, GPIO_FUNC_PIO0);
    gpio_set_function(SCR_D, GPIO_FUNC_PIO0);
    gpio_set_function(SCR10, GPIO_FUNC_PIO0);
    gpio_set_input_enabled(SCR_A, OUTPUT);
    gpio_set_input_enabled(SCR_B, OUTPUT);
    gpio_set_input_enabled(SCR_C, OUTPUT);
    gpio_set_input_enabled(SCR_D, OUTPUT);
    gpio_set_input_enabled(SCR10, OUTPUT);

    // Segments
    gpio_set_function(SEG_P, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_G, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_F, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_E, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_D, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_C, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_B, GPIO_FUNC_PIO1);
    gpio_set_function(SEG_A, GPIO_FUNC_PIO1);
    gpio_set_input_enabled(SEG_P, OUTPUT);
    gpio_set_input_enabled(SEG_G, OUTPUT);
    gpio_set_input_enabled(SEG_F, OUTPUT);
    gpio_set_input_enabled(SEG_E, OUTPUT);
    gpio_set_input_enabled(SEG_D, OUTPUT);
    gpio_set_input_enabled(SEG_C, OUTPUT);
    gpio_set_input_enabled(SEG_B, OUTPUT);
    gpio_set_input_enabled(SEG_A, OUTPUT);

    // Other pins
    gpio_set_function(CLR_BTN, GPIO_FUNC_PIO0);
    gpio_set_function(CLR_LED, GPIO_FUNC_PIO0);
    gpio_set_input_enabled(CLR_BTN, INPUT);
    gpio_set_input_enabled(CLR_LED, OUTPUT);
}

int main() {
    setup();

    // Launch the 'GPU' on the 2nd core
    multicore_launch_core1(screen_loop);
    // These cores are meant to run independently, so no startup time is needed


    while(true) {
        loop();
        sleep_ms(50); // I can only tap at 100ms at the very best
    }
}

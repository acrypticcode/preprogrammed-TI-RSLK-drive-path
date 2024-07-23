//////////////////////////
// Lab 1
// ENGR-2350 S24
// Names: Nicholas Danas and Curran Flanders
// Section: 1
// Side: B
// Seat: 25
//////////////////////////

#include "engr2350_msp432.h"
#include "lab1lib.h"

void GPIOInit();
void TestIO();
void ControlSystem();

uint8_t LEDFL = 0; // Two variables to store the state of
uint8_t LEDFR = 0; // the front left/right LEDs (on-car)
uint8_t ss1;
uint8_t pb1;
uint8_t a = 0;
uint8_t BMP0;
uint8_t BMP1;
uint8_t BMP2;
uint8_t BMP3;
uint8_t BMP4;
uint8_t BMP5;
int main (void){

    SysInit(); // Basic car initialization
    init_Sequence(); // Initializes the Lab1Lib Driver
    GPIOInit();

    printf("\r\n\n"
           "===========\r\n"
           "Lab 1 Start\r\n"
           "===========\r\n");

    while(1){
        //TestIO();
        // Used in Part A to test the IO
        ControlSystem(); // Used in Part B to implement the desired functionality
    }
}

void GPIOInit(){
    // Add initializations of inputs and outputs
    GPIO_setAsInputPin( GPIO_PORT_P3, GPIO_PIN5 );
    GPIO_setAsInputPin( GPIO_PORT_P5, GPIO_PIN6 );
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5 );

}

void TestIO(){
    // Add printf statement(s) for testing inputs

    // Example code for testing outputs
    while(1){
        uint8_t cmd = getchar();

        if(cmd == 'a'){
            GPIO_setOutputHighOnPin(GPIO_PORT_P8, GPIO_PIN0);
            // Turn LEDL On
        }else if(cmd == 'z'){
            GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN0);
            // Turn LEDL Off
        }// ...
        if(cmd == 's'){
            GPIO_setOutputHighOnPin(GPIO_PORT_P8, GPIO_PIN5);
            //Turn LEDFR On
        }else if(cmd == 'x'){
            GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN5);
            // Turn LEDFR off
        }
        if(cmd == 'q'){
            //Turn BiLED1 Red
            GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0);
        }else if(cmd == 'e'){
            //Turn BiLED1 Green
            GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);
        }else if(cmd == 'w'){
            //Turn BiLED1 Off
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1);
        }
        printf("cmd = %c\r",cmd);
    }
}

void ControlSystem(){
    while(1){
        ss1=GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5);
        if(ss1){
            if(status_Sequence()==100&a==0){
                a = 1;
                run_Sequence();

            }else{
            if(status_Sequence()==100&a==1){
                    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);
                    a = 0;
                    clear_Sequence();



            }else{
                GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0);

            }

        }
    }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1);
            BMP0 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0);
            BMP1 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN2);
            BMP2 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3);
            BMP3 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5);
            BMP4 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6);
            BMP5 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7);
            __delay_cycles(240e3);
            if(BMP0){
                record_Segment(2);
                GPIO_toggleOutputOnPin(GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5);
            }if(BMP1){
                record_Segment(1);
                GPIO_toggleOutputOnPin(GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5);
            }if(BMP2){
                record_Segment(0);
                GPIO_toggleOutputOnPin(GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5);
            }if(BMP3){
                record_Segment(127);
                GPIO_toggleOutputOnPin(GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5);
            }if(BMP4){
                record_Segment(-1);
                GPIO_toggleOutputOnPin(GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5);
            }if(BMP5){
                record_Segment(-2);
                GPIO_toggleOutputOnPin(GPIO_PORT_P8, GPIO_PIN0 | GPIO_PIN5);
            }

            while(BMP0|BMP1|BMP2|BMP3|BMP4|BMP5){
                BMP0 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0);
                BMP1 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN2);
                BMP2 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3);
                BMP3 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5);
                BMP4 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6);
                BMP5 = !GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7);
                __delay_cycles(240e3);
            }
            if (!(BMP0|BMP1|BMP2|BMP3|BMP4|BMP5)){

                GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6);
                if(pb1){
                    pop_Segment();
                    while(pb1)
                        GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6);
                }
            }
        }
}

}



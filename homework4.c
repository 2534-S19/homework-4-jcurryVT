#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

typedef enum {notThere, caseOne, caseTwo, caseThree, allThere} stateMachine;

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";
    int count;
    int transmitFlag;

    // TODO: Declare the variables that main uses to interact with your state machine.
    // Stops the Watchdog timer.
    WDT_A_hold(WDT_A_BASE); // pretty sure this is what stops  timer.  dobule check

    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.

    eUSCI_UART_ConfigV1 uartConfig = {

    //maybe how to do baud rate??
    //check with thweatt or something
            EUSCI_A_UART_CLOCKSOURCE_SMCLK,
            19, 8, 0x55,
            EUSCI_A_UART_NO_PARITY,        //no parity
            EUSCI_A_UART_LSB_FIRST, //this is the default. idk if ive gotta put it in
            EUSCI_A_UART_ONE_STOP_BIT, //1 stop bit
            EUSCI_A_UART_MODE,
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
            EUSCI_A_UART_8_BIT_LEN, //8bit payload
            };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
//dobule check this

    //how do i know what pin i need
    extern void GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    extern void GPIO_setAsPeripheralModuleFunctionOutputPin (GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    // TODO: Initialize EUSCI_A0
    extern bool UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    //extern bool UART_initModule(uint32_t moduleInstance, const eUSCI_UART_Config *config);
    UART_enableModule(EUSCI_A0_BASE);

    while (1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
       // void UART_transmitData(uint32_t moduleInstance, uint_fast8_t transmitData); //RELATIVE TO THE BOARD

    if (( UART_getInterruptStatus(EUSCI_A0_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
         //how do i use this
    {
        rChar = (UART_receiveData(EUSCI_A0_BASE)); //RECEIVE function
    }
    else
        {
            rChar=0xFF;
        }
            // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
            //       Check the transmit interrupt flag prior to transmitting the character.
    if (rChar != 0xFF)
    {
        if ((UART_getInterruptStatus(EUSCI_A0_BASE,EUSCI_A_UART_RECEIVE_TRANSMIT_FLAG) == EUSCI_A_UART_RECEIVE_TRANSMIT_FLAG)

        {
            rChar = //TRANSMIT function
            charFSM(rChar); //is this gonna check the character
        }
    }

    if (charFSM(rChar)==1)
                {
                    transmitFlag == UART_getInterruptStatus(EUSCI_A0_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

                    //make this a variable

                }

            // TODO: If the FSM indicates a successful string entry, transmit the response string.
            //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
            //       Make sure to reset the success variable after transmission.
        if (transmitFlag == EUSCI_A_UART_RECEIVE_TRANSMIT_FLAG)
            rChar= UART_receiveData(EUSCI_A0_BASE); //RECEIVE function is uart -> laptop cause this is dumb

        }
    }

    void initBoard()
    {
        WDT_A_hold(WDT_A_BASE);
    }

// TODO: FSM for detecting character sequence.

//figure out a specifc question for thweattt
    bool charFSM(char rChar)
    {
        static stateMachine nowState = notThere;

        bool sendIt = false;

        switch (nowState) {
               case notThere:
                   if (rChar == 2)
                       nowState = caseOne; // moves on if there's a 2 and stays if not
                   else
                       nowState == notThere;
                   break;

               case caseOne:
                   if (rChar == 5)
                       nowState = caseTwo; //moves on if sees a 5
                   if (rChar == 2)
                       nowState = caseOne; //goes back on itself to caseOne if sees 2
                   else
                       nowState = notThere; //if not moves back to nothing

                   break;

               case caseTwo:
                   if (rChar == 3)
                       nowState = caseThree;
                   if (rChar == 2)
                       nowState = caseOne; //goes back to caseOne if sees 2
                   else
                       nowState = notThere;
                   break;

               case caseThree:
                   if (rChar == 4)
                   {
                       nowState = allThere;
                       sendIt = true;
                   }
                   if (rChar == 2)
                        nowState = caseOne; //goes back to caseOne if sees 2
                   else
                       nowState = notThere;
                   break;

               case allThere:
                   sendIt=true;
                   nowState = notThere; //starts again after it sends the value out

                   break;
           }

           if (sendIt)
           {
               //display the thing
              rChar= UART_receiveData(EUSCI_A0_BASE); //RECEIVE function is uart -> laptop cause this is dumb
           }
           else
           {
               nowState=notThere;
           }
        return sendIt;
    }

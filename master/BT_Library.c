#define Bluetooth_ReceiverFifo        (*(volatile unsigned char *)(0xFF210220))
#define Bluetooth_TransmitterFifo     (*(volatile unsigned char *)(0xFF210220))
#define Bluetooth_InterruptEnableReg  (*(volatile unsigned char *)(0xFF210222))
#define Bluetooth_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210224))
#define Bluetooth_FifoControlReg (*(volatile unsigned char *)(0xFF210224))
#define Bluetooth_LineControlReg (*(volatile unsigned char *)(0xFF210226))
#define Bluetooth_ModemControlReg (*(volatile unsigned char *)(0xFF210228))
#define Bluetooth_LineStatusReg (*(volatile unsigned char *)(0xFF21022A))
#define Bluetooth_ModemStatusReg (*(volatile unsigned char *)(0xFF21022C))
#define Bluetooth_ScratchReg (*(volatile unsigned char *)(0xFF21022E))
#define Bluetooth_DivisorLatchLSB (*(volatile unsigned char *)(0xFF210220))
#define Bluetooth_DivisorLatchMSB (*(volatile unsigned char *)(0xFF210222))

#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Init_BT(void);
int putcharBT(int  c);
int   getcharBT(void);
int BT_TestForReceivedData(void);
void BT_Flush(void);
void BTOutMessage(char *message);
void BTFactoryReset(void);


int main() {
	Init_BT();
	BTFactoryReset();
}

void Init_BT( void ){
       // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    Bluetooth_LineControlReg = 0x80;
    // set Divisor latch (LSB and MSB) with correct value for required baud rate
    Bluetooth_DivisorLatchLSB = 0x1B;
    Bluetooth_DivisorLatchMSB = 0x00;
    // set bit 7 of Line control register back to 0 and
    // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    Bluetooth_LineControlReg= 0x03;
    // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    Bluetooth_FifoControlReg = 0x06;
    // Now Clear all bits in the FiFo control registers
    Bluetooth_FifoControlReg = 0x00;
}
int putcharBT( int  c ){
    // wait for Transmitter Holding Register bit (5) of line status register to be '1‘
    // indicating we can write to the device
    while((Bluetooth_LineStatusReg &  0x20) != 0x20){
        ;
    }
    // write character to Transmitter fifo register 
    Bluetooth_TransmitterFifo = c;
    // return the character we printed
    return c;
}
int   getcharBT ( void ){
    // wait for Data Ready bit (0) of line status register to be '1'
    while((Bluetooth_LineStatusReg & 0x01)!= 0x01){
	    ;
	}
    // read
    int character = (int) Bluetooth_ReceiverFifo;
    // return new character from ReceiverFiFo register
    return character;
}
// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int   BT_TestForReceivedData( void ){
    // if Bluetooth LineStatusReg bit 0 is set to 1
    // return TRUE, otherwise return FALSE
    if((Bluetooth_LineStatusReg & 0x01) == 0x01){
        return 1;
    }
    else return 0;
}
//
// flush the BT UART receive buffer by removing any unread characters
//
void   BT_Flush( void ){
    // while bit 0 of Line Status Register == ‘1’ (i.e. data available)
    int flush;
    while ((Bluetooth_LineStatusReg & 0x01) == 0x01){
        // read unwanted char out of fifo receiver buffer
	    flush = (int) Bluetooth_ReceiverFifo;
	}
    // read unwanted char out of FIFO receive buffer
    //return;	// no more characters, so return
}

void BTOutMessage(char *message){
	//Inspired from Stack Overflow how to itreate over a string in C

    while(*message){
		putcharBT(*message++);
        //putcharBT(*message);
    }
}

void BTFactoryReset(void) {
	// wait for 1 second between command
	// enter these commands in upper case

	/*  $$$ enter command mode
		SF,1 factory reset
		SN,Device1 set device name to “Device1”
		SP,1234 set 4 digit pin to “1234”
		R,1<CR> reboot BT controller
	*/

	char c, Message[100];
	int i;

	while (1) {
		printf("\r\nEnter Message for Bluetooth Controller:");

		gets(Message);   // get command string from user keyboard
		BTOutMessage(Message);  // write string to BT device


		// if the command string was NOT "$$$" send \r\n
		if (strcmp(Message, "$$$") != 0) { // $$$ puts BT module into command mode
			putcharBT('\r');
			putcharBT('\n');
		}
		//printf("Did it work?");
		// now read back acknowledge string from device and display on console,
		// will timeout after no communication for about 2 seconds
		for (i = 0; i < 2000000; i++) {
			if (BT_TestForReceivedData() == 1) {
				c = getcharBT();
				putchar(c);
				i = 0;         // reset timer if we got something back
			}
		}
	}
}


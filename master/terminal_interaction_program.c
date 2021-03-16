void BTFactoryReset(void){
    // wait for 1 second between command
    // enter these commands in upper case
    
    /*  $$$ enter command mode
        SF,1 factory reset
        SN,Device1 set device name to “Device1”
        SP,1234 set 4 digit pin to “1234”
        R,1<CR> reboot BT controller
    */

    char c, Message[100] ;
    int i;

    while(1) {
        printf("\r\nEnter Message for Bluetooth Controller:") ;

        gets(Message);   // get command string from user keyboard
        BTOutMessage(Message) ;  // write string to BT device

    
        // if the command string was NOT "$$$" send \r\n
        if(strcmp(Message, "$$$") != 0){ // $$$ puts BT module into command mode
            putcharBT('\r') ;
            putcharBT('\n') ;
        }
   
    // now read back acknowledge string from device and display on console,
    // will timeout after no communication for about 2 seconds
        for(i = 0; i < 2000000; i ++) {
            if(testBT() == 1)    {
                c = getcharBT() ;
                putchar(c) ;
                i=0 ;         // reset timer if we got something back
            }
        }
    }
}
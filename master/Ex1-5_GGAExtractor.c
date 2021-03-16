#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define GPS_ReceiverFifo        (*(volatile unsigned char *)(0xFF210210))
#define GPS_TransmitterFifo     (*(volatile unsigned char *)(0xFF210210))
#define GPS_InterruptEnableReg  (*(volatile unsigned char *)(0xFF210212))
#define GPS_InterruptIdentificationReg (*(volatile unsigned char *)(0xFF210214))
#define GPS_FifoControlReg		(*(volatile unsigned char *)(0xFF210214))
#define GPS_LineControlReg		(*(volatile unsigned char *)(0xFF210216))
#define GPS_ModemControlReg		(*(volatile unsigned char *)(0xFF210218))
#define GPS_LineStatusReg		(*(volatile unsigned char *)(0xFF21021A))
#define GPS_ModemStatusReg		(*(volatile unsigned char *)(0xFF21021C))
#define GPS_ScratchReg			(*(volatile unsigned char *)(0xFF21021E))
#define GPS_DivisorLatchLSB		(*(volatile unsigned char *)(0xFF210210))
#define GPS_DivisorLatchMSB		(*(volatile unsigned char *)(0xFF210212))

#define HEX0_1      (*(volatile unsigned int *)(0xFF200030))
#define HEX2_3      (*(volatile unsigned int *)(0xFF200040))
#define HEX4_5      (*(volatile unsigned int *)(0xFF200050))

//for GPS RS232 serial port
void Init_GPS(void);
int putcharGPS(int c);
int getcharGPS(void);
int GPSTestForReceivedData(void);
void GPSFlush(void);

//for GPS funcationalities
void GPSLogDump(void);
//void GGAdecode(char* raw, struct PositionInfo* info);
int CtD(char c);
void DisplayLogDump(char* raw);

//FOR TESTs
char putcharTEST(char c);
char getcharTEST(int index);
int power10(int exp);

/*
Struct for holding the decoded GGA message
*/
struct PositionInfo {
	char time[7];	//format: HHMMSS(24h) in UTC(+0) zone
	int lat;		//Positive = North, Negative = South,	this value is the x10^6 version of actual position
	int lon;		//Positive = East, Negative = West,	this value is the x10^6 version of actual position
	int alt;		//in meters referenced with sea level,	this value is the cast to int version of actual position (eg. 39.9 -> 39)
};

char putcharTEST(char c) {
	printf("%c", c);
	return c;
}
char getcharTEST(int index) {
	char* raw = "$GPGGA,064951.000,2307.1256,N,12016.4438,E,1,8,0.95,39.9,M,,*65\r\n$PMTKLOX,0,43*6E\r\n$PMTKLOX,1,0,0100010B,1F000000,0F000000,0000100B,00000000,7FFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00FC8C1C,0B37464F,027FD670,42DC9EC6,4113007A,1A37464F,027FD670,42DC9EC6,4113006B*25\r\n$PMTKLOX,2";
	
	char c = raw[index];
	return c;
}



/*
Initialize GPS serial port to default settings
*/
void Init_GPS(void)
{
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	GPS_LineControlReg = 0x80;
	// set Divisor latch (LSB and MSB) with correct value for required baud rate
	GPS_DivisorLatchLSB = 0x45;
	GPS_DivisorLatchMSB = 0x01;
	// set bit 7 of Line control register back to 0 and
	// program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
	GPS_LineControlReg = 0x03;
	// Reset the Fifo¡¯s in the FiFo Control Reg by setting bits 1 & 2
	GPS_FifoControlReg = 0x06;
	// Now Clear all bits in the FiFo control registers
	GPS_FifoControlReg = 0x00;
}
/*
Put a character into the Write FiFo for GPS chip to read
*/
int putcharGPS(int c) {
	// wait for Transmitter Holding Register bit (5) of line status register to be '1'
	// indicating we can write to the device
	while ((GPS_LineStatusReg & 0x20) != 0x20) {
		;
	}
	// write character to Transmitter fifo register
	GPS_TransmitterFifo = c;
	// return the character we printed
	return c;
}
/*
Returns the next character in the read fifo
*/
int getcharGPS(void) {
	// wait for Data Ready bit (0) of line status register to be '1'
	while ((GPS_LineStatusReg & 0x01) != 0x01) {
		;
	}
	// read new character from ReceiverFiFo register
	int character = (int)GPS_ReceiverFifo;
	// return new character
	return character;
}
// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int GPSTestForReceivedData(void)
{
	// if RS232_LineStatusReg bit 0 is set to 1
	//return TRUE, otherwise return FALSE
	if ((GPS_LineStatusReg & 0x01) == 0x01) {
		return TRUE;
	}
	else return FALSE;
}
/*
Clean the Read FiFo and dispose all unread data
*/
void GPSFlush(void)
{
	// while bit 0 of Line Status Register == ¡®1¡¯
	int flush;
	while ((GPS_LineStatusReg & 0x01) == 0x01) {
		// read unwanted char out of fifo receiver buffer
		flush = (int)GPS_ReceiverFifo;
	}
	return; // no more characters so return
}



/*
For math operation: base^exp
only works for ints due to arm processor limitation
*/

int power10(int exp) {
	int result = 1;
	int i = 0;
	if (exp > 0) {
		for (i = exp; i > 0; i--) {
			result = result * 10;
		}
		return result;
	}
	else if (exp == 0) {
		return result;
	}
	printf("\nmath: power(): error, returned 0.\n");
	return 0;
}


/*
Char to Decimal Number Convertor

Handles all char possiblities up to Hex(0~F) representation
and returns corresponding decimal 
number in int for further computation

Invalid inputs simply results 0 being returned
*/
int CtD(char c){
    switch (c){
    case('0'):
        return 0;
    case('1'):
        return 1;
    case('2'):
        return 2;
    case('3'):
        return 3;
    case('4'):
        return 4;
    case('5'):
        return 5;
    case('6'):
        return 6;
    case('7'):
        return 7;
    case('8'):
        return 8;
    case('9'):
        return 9;
    case('A'):
        return 10;
	case('a'):
		return 10;
	case('B'):
        return 11;
	case('b'):
		return 11;
	case('C'):
        return 12;
	case('c'):
		return 12;
    case('D'):
        return 13;
	case('d'):
		return 13;
	case('E'):
		return 14;
	case('e'):
		return 14;
    case('F'):
        return 15;
    case('f'):
        return 15;
	case('-'):
		return -1;
    default:
        return 0;
    }
}


/*
GPGGA Decoder
call this to handle if you know the message type is GGA
need pass a 'PositionInfo' struct to it to it can report results

Modifies "info".
*/
int GGAdecode(char* raw, struct PositionInfo* info){
    if(info == NULL){
        printf("GGA Decoder: given PositionInfo struct is NULL.\n");
        return 1;
    }

    int i, j;		//Auxlary Loop counter
    int sum;         //computing buffer for double values
    char* buffer;		//a tempary buffer for holding message fields
    int stage = 0;      //the comma counter, there should be 14 in total
    int commas[14];     //locations of commas, used for field extrection.

    //check header
	buffer = "$GPGGA";
    for(i=0; i<6; i++){
        if(raw[i] != buffer[i]){
            //printf("GGA Decoder: the input data is not a GGA message.\n");
            return 1;
        }
    }
    
    //scanning the field indexes
    for(i=6; i<100; i++){
        //end if all fileds are recorded
        if(stage>14 || raw[i] == '*'){
            break;
        }

        //record the fields' start and end index
        if(raw[i] == ','){
            commas[stage] = i;
			stage++;
        }
    }
    
    //Extract: Time
    j = 0;
    for(i=commas[0]+1; i<commas[1]; i++){
        if(raw[i] == '.'){
            break;
        }else{
            (info->time)[j] = raw[i];
            j++;
        }
    }
	(info->time)[j] = '\0';


    //Extract: Latitude
	sum = 0; i = 0;
    j = 0;
    for(i=commas[2]-1; i>commas[1]; i--){
        if(raw[i] == '.'){
            continue;
        }else{
			sum += power10(j) * CtD(raw[i]);
			if (j == 5) {
				sum = sum / 60;
			}
			j++;
			
        }
    }
    if(commas[2]+1 != commas[3]){
		if (raw[commas[2] + 1] == 'S' || raw[commas[2] + 1] == 's') {
			info->lat = -1 * sum;
		}
		else {
			info->lat = sum;
		}
    }else {
		info->lat = sum;
	}
    
    //Extract: Longitude
    sum = 0;
	i = 0; j = 0;
    for(i=commas[4]-1; i>commas[3]; i--){
		if(raw[i] == '.'){
            continue;
        }else{
			sum += power10(j) * CtD(raw[i]);
			if (j == 5) {
				sum = sum / 60;
			}
			j++;
        }
    }
    if(commas[4]+1 != commas[5]){
        if(raw[commas[4]+1] == 'W' || raw[commas[4]+1] == 'w'){
			info->lon = -1 * sum;
		}else {
		info->lon = sum;
		}
	}else{
		info->lon = sum;
	}
    
    //Extract: Altitude
    sum = 0;
    j = 0;
    for(i=commas[9]-1; i>commas[8]; i--){
        if(raw[i] == '.'){
            sum = 0;
            j=0;
        }else{
            sum += power10(j) * CtD(raw[i]);
            j++;
        }
    }
    if(commas[4]+1 != commas[5]){
        if(raw[commas[2]+1] == 'K' || raw[commas[2]+1] == 'k'){
            info->alt = 1000 * sum;
        }else{
            info->alt = sum;
        }
    }else{
        info->alt = sum;
    }


    return 0;
}


/*
GPS GGA feedback monitor: the wrapper for serial GPS port
*/
void GGAmonitor(struct PositionInfo* info) {
	int i = 0;
	char raw[1000];

	do {
START:	raw[0] = getcharGPS();
		//ignore incomplete messages
		if (raw[0] != '$') {
			i = 0;
			goto START;
		}
		else {
			goto PROCEED;
		}

PROCEED:
		while (raw[i] != '*') {
			i++;
			raw[i] = getcharGPS();
		}
	} while (GGAdecode(raw, info));

	return;
}


/*
Displays the PMTKLOX dump data strings
*/
void DisplayLogDump(char* raw) {
	int i, j;
	char* end = "$PMTKLOX,2";
	char entryHead = '$';
	int endfound = 0;		//I hate it when C does not have a boolean data type

	printf("\n");
	i = 0;
	//print all chars of the log
	while(endfound == 0){
		//log ending check on each '$' sign
		if (raw[i] == entryHead) {
			for (j = 0; j <= 9; j++) {
				if (raw[i+j] == end[j]) {
					endfound = 1;
				}
				else {
					endfound = 0;
					break;
				}
			}
		}

		printf("%c", raw[i]);
		i++;
	}
	
	printf("PMTKLOX,2\n");
	return;
}


/*
Function to order GPS chip to dump out the log
and display it on the terminal
*/
void GPSLogDump(void) {
	GPSFlush();
	
	int i, j;
	char* cmd = "$PMTK622,1*29\r\n";
	int end = 0;
	int end2 = 0;
	char* terminate = "$PMTKLOX,2";
	char* terminate2 = "$G";
	char rawbuffer[10];		//FIFO buffer for monitoring the end of logging

	//sending the commmand to GPS chip
	printf("\n>GPS COMMAND: ");
	for (i = 0; i < 16; i++) {
		putcharGPS(cmd[i]);
		putcharTEST(cmd[i]);
	}

	GPSFlush();
	printf("\n>GPS LOG DATA:\n");
	//reading the output from GPS
	//TEST PARAM: i
	//i = 0;
	while(1) {
		
		//elemental shift buffer to left once
		end = 0;
		end2 = 0;
		for (j = 0; j < 9; j++) {
			rawbuffer[j] = rawbuffer[j+1];
		}
		rawbuffer[9] = getcharGPS();
		
		//check ending
		for (j = 0; j < 10; j++) {
			if (rawbuffer[j] == terminate[j]) {
				end++;
			}else {;}
		}
		for (j = 0; j < 2; j++) {
			if (rawbuffer[j] == terminate2[j]) {
				end2++;
			}
			else {;}
		}

		//print next char on terminal
		printf("%c", rawbuffer[9]);

		//end found, break printing
		if (end == 10 || end2 == 2) {
			printf("\n\n");
			break;
		}

		//TEST PARAM: i
		//i++;
	}
	printf(">GPS LOG ENDs\n");
	return;

}




/*
Funcation TEST
*/
int main(void) {
	/*
$PMTKLOX,0,43*6E
$PMTKLOX,1,0,0100010B,1F000000,0F000000,0000100B,00000000,7FFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00FC8C1C,0B37464F,027FD670,42DC9EC6,4113007A,1A37464F,027FD670,42DC9EC6,4113006B*25
$PMTKLOX,1,1,2037464F,027FD670,42DD9EC6,41130050,2437464F,027FD670,42DD9EC6,41130054,2837464F,027FD670,42DD9EC6,41130058,2B37464F,027FD670,42DD9EC6,4113005B,2E37464F,027FD670,42DD9EC6,4113005E,3D37464F,027FD670,42DC9EC6,4113004C*59
$PMTKLOX,1,2,4C37464F,027FD670,42DC9EC6,4113003D,5B37464F,027FD670,42DC9EC6,4113002A,6A37464F,027FD670,42DD9EC6,4113001A,7937464F,027FD670,42DD9EC6,41130009,8837464F,027FD670,42DD9EC6,411300F8,9737464F,027FD670,42DD9EC6,411300E7*5C
$PMTKLOX,1,3,A637464F,027FD670,42DD9EC6,411300D6,B537464F,027FD670,42DD9EC6,411300C5,C437464F,027FD670,42DD9EC6,411300B4,D337464F,027FD670,42DD9EC6,411300A3,E237464F,027FD670,42DD9EC6,41130092,F137464F,027FD670,42DD9EC6,41130081*59
$PMTKLOX,1,4,0038464F,027FD670,42DD9EC6,4113007F,0F38464F,027FD670,42DC9EC6,41130071,1E38464F,027FD670,42DC9EC6,41130060,2D38464F,027FD670,42DC9EC6,41130053,3C38464F,027FD670,42DC9EC6,41130042,4B38464F,027FD670,42DD9EC6,41130034*58
$PMTKLOX,1,5,5A38464F,027FD670,42DD9EC6,41130025,6938464F,027FD670,42DC9EC6,41130017,7838464F,027FD670,42DC9EC6,41130006,8738464F,027FD670,42DC9EC6,411300F9,9638464F,027FD670,42DC9EC6,411300E8,A538464F,027FD670,42DD9EC6,411300DA*5D
$PMTKLOX,1,6,B438464F,027FD670,42DC9EC6,411300CA,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF*58
$PMTKLOX,1,7,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF*58
$PMTKLOX,2*47

	*/
	char* raw = "$GPGGA,064951.000,2307.1256,N,12016.4438,E,1,8,0.95,39.9,M,,*65\r\n$PMTKLOX,0,43*6E\r\n$PMTKLOX,1,0,0100010B,1F000000,0F000000,0000100B,00000000,7FFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,FFFFFFFF,00FC8C1C,0B37464F,027FD670,42DC9EC6,4113007A,1A37464F,027FD670,42DC9EC6,4113006B*25\r\n$PMTKLOX,2";
	struct PositionInfo decoded;
	//GGAdecode(raw, &decoded);

	//printf("Decoded Fields: \n");
	//printf("time: %s\n", decoded.time);
	//printf("lat: %d \n", decoded.lat);
	//printf("lon: %d \n", decoded.lon);
	//printf("alt: %d \n", decoded.alt);
	//HEX0_1 = decoded.alt & 0x000000FF;
	//HEX2_3 = (decoded.alt & 0x0000FF00) >> 2;
	//HEX4_5 = (decoded.alt & 0x00FF0000) >> 4;

	//DisplayLogDump(raw);
	///////////////////////////////////////////
	

	Init_GPS();
	printf("\n\nGPSLOGDUMP TEST: \n");

	//GPSLogDump();

	printf("\n\nGPS GGA TEST: \n");

	while (1) {
		GGAmonitor(&decoded);
		printf("Decoded Fields: \n");
		printf("time: %s\n", decoded.time);
		printf("lat: %d \n", decoded.lat);
		printf("lon: %d \n", decoded.lon);
		printf("alt: %d \n", decoded.alt);

		HEX0_1 = decoded.alt & 0x000000FF;
		HEX2_3 = (decoded.alt & 0x0000FF00) >> 2;
		HEX4_5 = (decoded.alt & 0x00FF0000) >> 4;



	}
	
	system("PAUSE");
	return 0;
}



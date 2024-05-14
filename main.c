// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON     // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000
//defining RC2 and RC1 as RS, EN
#define RS RC2
#define EN RC1

//function declarations
void init(void);
void i2c_init(const unsigned long);
void i2c_write(unsigned char);
int i2c_read(int);
void i2c_start();
void i2c_wait();
void i2c_stop();
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void i2c_repeated_start();
int bcd_2_bcd(int);
int dec_2_bcd(int);
void settime(void);
void update(void);

//declaring arrays with respective data
char msg1[5] = {"TIME:"};
char msg2[5] = {"DATE:"};
char am[2] = {"AM"};
char pm[2] = {"PM"};
//storing days in array
const char* arr[] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};

//declaring the required variables
int i,j,k;
int sec=57;
int min=59;
int hour=11;
int date=28;
int month=04;
int year=24;
int day=7;
char sec1,sec2,min1,min2,hour1,hour2,date1,date2,month1,month2,year1,year2;

//main function
void main()
{
    init();                                                   //calling init() function
    i2c_init(100);                                            //calling i2c_init() function
    settime();                                                //calling settime() function
    while(1)                                                  //while loop
    {
        update();                                             //calling update function                                        //stop function
        //logic to get the one's digit and ten's digit
        sec1=sec/10;
        sec2=sec%10;
        min1=min/10;
        min2=min%10;
        hour1=hour/10;
        hour2=hour%10;
        date1=date/10;
        date2=date%10;
        month1=month/10;
        month2=month%10;
        year1=year/10;
        year2=year%10;
        
        //initializing lcd display to 1st line and 1st character
        lcd_command(0x80);
        for(i=0;i<5;i++)                //for loop to display the "Time:" message 
            lcd_data(msg1[i]);
        //displaying time "hour - min - sec"
        lcd_data(hour1+'0');            
        lcd_data(hour2+'0');
        lcd_data(0x2D);
        lcd_data(min1+'0');
        lcd_data(min2+'0');
        lcd_data(0x2D);
        lcd_data(sec1+'0');
        lcd_data(sec2+'0');
        lcd_data(' ');
        
        if(hour >= 12 )            //checking hour >= 12 or not
            for (j=0;j<2;j++)
            {
                lcd_data(pm[j]);    // if yes, printing AM
            }
        else
        {
            for(k=0;k<2;k++)
            {
                lcd_data(am[k]);  //printing PM
            }
        }
        lcd_command(0xC0);     //command for 2nd line , 1st character
        for(i=0;i<5;i++)       //displaying "DATE:"
            lcd_data(msg2[i]);
        
        //displaying date as "DATE-MONTH-YEAR"
        lcd_data(date1+'0');
        lcd_data(date2+'0');
        lcd_data(0x2D);
        lcd_data(month1+'0');
        lcd_data(month2+'0');
        lcd_data(0x2D);
        lcd_data(year1+'0');
        lcd_data(year2+'0');
        
        // printing day
        lcd_data(arr[day-1][0]);
        lcd_data(arr[day-1][1]);
        lcd_data(arr[day-1][2]);  
    }
}
void init(void)
{
 TRISD = 0x00;    //setting PORT B as input    
 TRISC = 0x18;    //setting PORT C as output
 PORTD = 0x00;    //setting PORT D as output
 lcd_command(0x38);   //setting lcd as 2 lines, 5x7 dot lcd
 __delay_ms(100);        //generating delay for 100ms
  lcd_command(0x38);   //setting lcd as 2 lines, 5x7 dot lcd
 __delay_ms(100);         //generating delay for 100ms
  lcd_command(0x38);   //setting lcd as 2 lines, 5x7 dot lcd
 __delay_ms(100);         //generating delay for 100ms
  lcd_command(0x38);  //setting lcd as 2 lines, 5x7 dot lcd
 __delay_ms(100);        //generating delay for 100ms
 lcd_command(0x0C);  //setting display on and curson off
 __delay_ms(100);        //generating delay for 100ms
 lcd_command(0x06);    //setting entry mode, increments to right shift
 __delay_ms(100);       //generating delay for 100ms
 lcd_command(0x01);     //setting clear display
 __delay_ms(100);       //generating delay for 100ms
}
void lcd_command(unsigned char cmd)
{
    RS=0;      //making RS as 0 
    PORTD=cmd;       //assigning the command
    EN=1;      //making EN as 1
    EN=0;      //making EN as 0
    __delay_ms(5);   //delay for 5 ms5
}
void lcd_data(unsigned char data)
{
    RS=1;      //making RS as 0 
    PORTD=data;       //assigning the data
    EN=1;      //making EN as 1
    EN=0;      //making EN as 0
    __delay_ms(5);   //delay for 5 ms
}


void i2c_init(const unsigned long baud)                  //function defintion of I2C_Master_Init 
{
    SSPCON = 0x28;               //00101000, Enabling serial port and setting as I2C as Master Device , Enabling SDA and SCL
    SSPSTAT = 0x00;                     //Intializing SSPSTAT as 00000000
    SSPCON2 = 0x00;              //initislizing SSPCON2 as 00000000
    SSPADD = (_XTAL_FREQ/(4*baud*100))-1;   //calculating the frequency and assigning to SSPADD
    
}    

void i2c_wait()                       //Master_Wait Function
{
   while(SSPCON2 & 0x1F || SSPSTAT & 0x04);    //Checking whether transmission is going on or not, checking all the bits are not setted
}


void i2c_start(){                      //i2c_start function definition
    i2c_wait();                        //calling Wait Function
    SEN = 1;                                  //Enabling Start Bit
}

void i2c_repeated_start(){             //i2c_repeated_start function definition
    i2c_wait();                        //calling Wait Function
    RSEN = 1;                                //Enabling RSEN bit
}

void i2c_stop(){                     //i2c_stop function definition
    i2c_wait();                      //Calling Wait Function
    PEN = 1;                                //Enabling Stop Bit
}

void i2c_write(unsigned char temp)             //i2c_write function
{
    i2c_wait();                          //Calling Wait Function
    SSPBUF=temp;                         //assigning temp to SSPBUF
}
int i2c_read(int ack)                        //i2c_read function
{
    int value;                            //declaring variable value
    i2c_wait();                          //calling wait function
    RCEN=1;                              //making RCEN as 1
    i2c_wait();                          //calling wait function
    value=SSPBUF;                        //reading data from SSPBUF      
    i2c_wait();                          //calling wait function
    ACKDT=(ack)?0:1;                     //setting nack, ack bit
    ACKEN=1;                             //enabling ACk/NACK sequence
    return value;                        //returning value
}

int dec_2_bcd(int temp)            //function to convert decimal to BCD
{ 
   return ((temp/10)<<4)+(temp%10);

}
int bcd_2_dec(int temp)             //function to convert BCD to decimal
{ 
   return ((temp>>4)*10)+(temp&0x0F);
}


void settime(void)
{
    i2c_start();            //calling start function
    i2c_write(0xD0);       //sending address
    i2c_write(0x00);       //sending register address
    i2c_write(dec_2_bcd(sec));         //writing secs and converting to bcd
    i2c_write(dec_2_bcd(min));                //writing secs and converting to bcd
    i2c_write(0x60 | (dec_2_bcd(hour)));      //writing hour and converting to bcd
    i2c_write(dec_2_bcd(day));                //writing day and converting to bcd
    i2c_write(dec_2_bcd(date));               //writing date and converting to bcd
    i2c_write(dec_2_bcd(month));              //writing month and converting to bcd
    i2c_write(dec_2_bcd(year));               //writing year and converting to bcd
    i2c_stop();
}
void update(void)
{
    i2c_start();                        // Start I2C communication
    i2c_write(0xD0);                    // Writing device address for write operation
    i2c_write(0);                       // Address of register
    i2c_stop();                         // Stop I2C communication
    i2c_start();                        // Start I2C communication 
    i2c_write(0xD1);                    // Writing device address for read operation   
    sec = (bcd_2_dec(i2c_read(1)));      // Reading and converting seconds
    min = (bcd_2_dec(i2c_read(1)));      // Reading and converting minutes
    int hours = i2c_read(1);           // Read hours   
    hour = bcd_2_dec(((hours & 0x1F) | ((hours >> 5) & 0x60)));   // Extracting hour value (12-hour format)
    day = (bcd_2_dec(i2c_read(1)));      // Reading and converting day
    date = (bcd_2_dec(i2c_read(1)));     // Reading and converting date
    month = (bcd_2_dec(i2c_read(1)));    // Reading and converting month
    year = (bcd_2_dec(i2c_read(1)));     // Reading and converting year   
    i2c_stop();                         // Stop I2C communication
    i2c_start();                        //start i2c communication
    i2c_write(0xD1);                    //writing address
    i2c_read(1);                        //i2c_read
    i2c_stop();                         //stop i2c_communication
}

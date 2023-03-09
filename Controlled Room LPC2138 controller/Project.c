/********************************************************************************
                                 HEADER FILES
*********************************************************************************/
#include<lpc213x.h>
#define RS  0x00020000                  /* RS  - P1.17                          */
#define RW  0X00040000                  /* R/W - P1.18                          */    
#define EN  0X00080000                  /* E   - P1.19                          */
#define CLR 0X00FE0000

unsigned int adc_value=0;

/********************************************************************************
* Function    : Delay                                                           *
*                                                                               *
* Description : Function for Delay                                              *
*                                                                               *
* Parameters  : x - Delay time                                                  *
*********************************************************************************/
int Delay(unsigned int x)
{
 x=x*12000;
 while(x!=0)
 {
  x--;
 }
 return 0;
}

/********************************************************************************
* Function    : LCD_Command                                                     *
*                                                                               *
* Description : Function to give command to LCD                                 *
*                                                                               *
* Parameters  : command - command to be given                                   *
*********************************************************************************/
void LCD_Command(char command)
{
 int Temp;				           
 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1CLR = RS;                           /* RS=0 for command register            */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (command & 0xF0) << 16;       /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */
}

/********************************************************************************
* Function    : LCD_Command1                                                    *
*                                                                               *
* Description : Function to give command to LCD                                 *
*                                                                               *
* Parameters  : command1 - command to be given                                  *
*********************************************************************************/
void LCD_Command1(char command1)
{
 int Temp;
 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1CLR = RS;                           /* RS=0 for command register            */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (command1 & 0xF0) << 16;      /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */

 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1CLR = RS;                           /* RS=0 for command register            */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (command1 & 0x0F) << 20;      /* Taking the second nibble of command  */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */
}

/********************************************************************************
* Function    : LCD_Data                                                        *
*                                                                               *
* Description : Function to display single character on LCD                     *
*                                                                               *
* Parameters  : data - character to be displayed                                *
*********************************************************************************/
 void LCD_Data(char data)  
{
 int Temp;
 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1SET = RS;                           /* RS=1 for data register               */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (data & 0xF0) << 16;          /* Taking the first nibble of command   */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */

 IO1CLR = CLR;                          /* Clearing the port pins               */
 IO1SET = EN;                           /* Enable pin high                      */
 IO1SET = RS;                           /* RS=1 for data register               */
 IO1CLR = RW;                           /* R/W=0 for write                      */
 Temp   = (data & 0x0F)<<20;            /* Taking the second nibble of command  */
 IO1SET = IO1SET | Temp;                /* Writing it to data line              */
 Delay(2);
 IO1CLR = EN;                           /* Enable pin low to give H-L pulse     */
}

/********************************************************************************
* Function    : LCD_String                                                      *
*                                                                               *
* Description : Function to display string on LCD                               *
*                                                                               *
* Parameters  : String to be displayed                                          *
*********************************************************************************/ 
void LCD_String(unsigned char *dat)
{
 while(*dat!='\0')                      /* Check for termination character      */
 {
  LCD_Data(*dat);                       /* Display the character on LCD         */
  dat++;                                /* Increment the pointer                */
 }
}

/********************************************************************************
* Function    : LCD_Init                                                        *
*                                                                               *
* Description : Function to initialize LCD                                      *
*                                                                               *
* Parameters  : None                                                            *
*********************************************************************************/
void LCD_Init(void)
{
 Delay(15);
 LCD_Command(0x30);
 Delay(10);	
 LCD_Command(0x30);
 Delay(5);	
 LCD_Command(0x30);
 LCD_Command(0x20);
 LCD_Command1(0x28);
 LCD_Command1(0x01);                    /* Clear display                        */
 LCD_Command1(0x06);                    /* Auto increment                       */
 LCD_Command1(0x0C);                    /* Cursor off                           */
}

/********************************************************************************
* Function    : ADC_Conversion                                                  *
*                                                                               *
* Description : Function to get ADC reading                                     *
*                                                                               *
* Parameters  : None                                                            *
*********************************************************************************/
int ADC_Conversion()
{
 int ab;                                /* Variable to store ADC value          */
 Delay(1);
 AD0CR = AD0CR|0x01000000;              /* Start conversion                     */
 while((AD0DR&0x80000000)!=0x80000000); /* Wait here till conversion is over    */ 
 ab    = (AD0DR&0x0000FFC0);            /* Extracting the result                */
 ab    = (ab>>6);                       /* Shift 6 bits right                   */
 return ab;                             /* Return the result                    */
}

/********************************************************************************
* Function    : Int_ASCII                                                       *
*                                                                               *
* Description : Function to store integer value in array                        *
*                                                                               *
* Parameters  : value - value to be stored                                      *									
*               cnt   - no: of digits                                           *
*********************************************************************************/
void Int_ASCII(int value,char cnt)
{
 int i = 0;                             /* Local variables                      */
 char array[7];
 int values;
 values = value;

 for(i=1; i<=cnt; i++)                  /* Store the received value in array    */
 {
  array[i] = values%10;
  values   = values/10;
 }

 for(i=cnt; i>=1; i--)                  /* Display it on LCD                    */
 {
  LCD_Data(array[i]+'0');
 }
}

/********************************************************************************
* Function    : ADC_Check                                                       *
*                                                                               *
* Description : Function to take ADC reading & display it on LCD                *
*                                                                               *
* Parameters  : None                                                            *
*********************************************************************************/
void ADC_Check()
{
	IO0DIR |= 0x000000FF;
		IO1DIR &= (~(1<<16));
		IO0CLR = 0x000000FF;
	IO1DIR |= (1<<28);
 AD0CR=0x00200604;                      /* PDN=1,CLKDIV=6,channel=AD0.2         */
 LCD_Command1(0x80);
 //LCD_String("ADC Checking....");
 //LCD_Command1(0xC0);
 //LCD_String("Temp: ");
 adc_value=ADC_Conversion();            /* Get the result of conversion         */
 //adc_value = adc_value*330/1023;
// LCD_Command1(0xC4);                    /* 2nd row, 5th location                */
 Int_ASCII(adc_value,2);                /* Display the result on LCD            */
	 if (!(IO1PIN & (1 << 16)))
        {
					LCD_String("Movement On");
					Delay(10);
				}
		else {
					LCD_String("Movement OFF");
					Delay(10);
		}			
				LCD_Command1(0xC0);
				//LCD_String("Temp: ");
			 //adc_value=ADC_Conversion();  
	if (adc_value > 0){
		  LCD_String("Light On");
			Delay(20);
			IO1PIN ^= 0x10000000;
	} 
	else LCD_String(" OFF");
 Delay(100);
}

/********************************************************************************
* Function??? : PORT_Initial???????????????????????????????????????????????     *
*??????????????????????????????????????????????????????????????????????????     *
* Description : Function to initialize PORTs ?   ??????????????????????????     *
*??????????????????????????????????????????????????????????????????????????     *
* Parameters? : None???????????????????????????????????????????????????????     *
*********************************************************************************/
void PORT_Initial(void)
{
IO1DIR  = 0x00FE0000;                  /* LCD pins set as o/p?????????         */
PINSEL0 = 0x00000000;
PINSEL1 = 0x05000000;                  /* ADC function for 0.28 & 0.29         */
PINSEL2 = 0x00000000;
}
unsigned int ultrasonic_read()
{
    int b,c;   
 T1TC=0;   T1PR=32;
 T1TCR = 0x00;
      
    IO0SET =(1<<0);
    Delay(100);
    IO0CLR =(1<<0);
    while(!(IO0PIN &(1<<1)));        
 T1TCR = 0x01;
 c=T1TC;
 while((IO0PIN &(1<<1)));
 T1TCR = 0x00;
 b=T1TC;
 return (b-c);
}
void LCDWriteInt(int val,unsigned int field_length)
{

 char str[5]={0,0,0,0,0};
 int i=4,j=0;
 while(val)
 {
 str[i]=val%10;
 val=val/10;
 i--;
 }
 if(field_length==-1)
  while(str[j]==0) j++;
 else
  j=5-field_length;

 if(val<0) LCD_Data('-');
 for(i=j;i<5;i++)
 {
 LCD_Data(48+str[i]);
 }
}

/********************************************************************************
                                 MAIN FUNCTION
*********************************************************************************/	
int main()
{
	unsigned int x;
 PORT_Initial();                        /* Initialise the ports                 */
 LCD_Init();                            /* Initialise LCD                       */
 LCD_Command1(0x80);                    /* 1st row, 1st location                */
 LCD_String("ADC Checking.");
 LCD_Command1(0x01);                    /* Clear screen                         */
 while(1)
 {
   ADC_Check();
   LCD_Command1(0x01); 	 /* Take ADC reading                     */
 }
}

/*****************************? END OF PROGRAM? *********************************/
//
// I2C_EEPROM : using I2C1 to access EEPROM
//
// EVB    : Nu-LB-NUC140
// MCU    : NUC140VE3CN (LQFP100)
// EEPROM : 24LC64
//          I2C@400KHz (2.5~5.5V)
//
// EEPROM I2C connections:
// I2C1-SCL/PA11
// I2C1-SDA/PA10
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "LCD.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "sound.h"
#include "LCD.h"
#include "i2c.h"
int8_t movX=1, dirX=0; // pixel per step! 
int8_t movY=1, dirY=0;
char text[16] = "                ";
unsigned char pic[]={0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};
unsigned char ballp[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
struct pi{
int x;
int y;
unsigned char *p;

};
typedef struct pi pi;
pi p1={0,0,pic};
pi p2={0,0,pic};
pi ball={64,32,ballp};

int score1,score2;



// Global variables
volatile uint8_t g_u8DeviceAddr;
volatile uint8_t g_au8TxData[3];
volatile uint8_t g_u8RxData;
volatile uint8_t g_u8DataLen;
volatile uint8_t g_u8EndFlag = 0;
int test_mode=0;
typedef void (*I2C_FUNC)(uint32_t u32Status);
static I2C_FUNC s_I2C1HandlerFn = NULL;

typedef struct pass{
	int index;
	int value[4];
	int xpos;
	int ypos;

}number;
number password;
number guess;
volatile uint8_t KEY_Flag;
volatile uint32_t index_5ms, cnt_5ms, cnt_1s, cnt_100ms,index_key_scan;
int digit[4];

int mov_state;
void setDirectionUp() {
    dirX += 0;
    dirY += -1;
	mov_state=1;
}

void setDirectionLeft() {
    dirX += -1;
    dirY += 0;
	mov_state=2;
}

void setDirectionRight() {
    dirX += 1;
    dirY += 0;
	mov_state=3;
}

void setDirectionDown() {
    dirX += 0;
    dirY += 1;
	mov_state=4;
}


void draw_Bmp_axb(int16_t x, int16_t y, uint16_t fgColor, uint16_t bgColor, unsigned char bitmap[], uint8_t a, uint8_t b) {
    uint8_t t, i, j, k, kx, ky;
    uint8_t bytes_per_col = (b + 7) / 8; 

    if (x < (LCD_Xmax - (a - 1)) && y < (LCD_Ymax - (b - 1))) {
        for (j = 0; j < bytes_per_col; j++) {
            for (i = 0; i < a; i++) {        
                kx = x + i;                 
                t = bitmap[i + j * a];      

                
                for (k = 0; k < 8; k++) {   
                    ky = y + j * 8 + k;    
                    if (ky >= y + b) break; 
                    if (t & (0x01 << k))    
                        draw_Pixel(kx, ky, fgColor, bgColor); 
                }
            }
        }
    }
}
volatile uint32_t count_Line, count_Char;
void TMR1_IRQHandler(void)
{	
	
	
//	draw_Bmp_axb(p1.x, p1.y, 0,0, p1.p, 16,4);
	
	
	
	cnt_5ms++;
	index_5ms = cnt_5ms % 4;
	CloseSevenSegment();
	if (index_5ms == 0) {			
	ShowSevenSegment(0,digit[0]);		
	
	}	
	if (index_5ms == 1)  {
	ShowSevenSegment(1,digit[1]);		
	}	
	if (index_5ms == 2)  {
	ShowSevenSegment(2,digit[2]);		
	}
	if (index_5ms == 3)  {
	ShowSevenSegment(3,digit[3]);
	}		
	if (cnt_5ms % 20 == 0) 
	{
		cnt_100ms++;
		index_key_scan = cnt_100ms++ % 3;
		if (index_key_scan == 0)
		{
			PA0=1; PA1=1; PA2=1; PA3=1; PA4=1; PA5=0;
		}
		if (index_key_scan == 1)
		{
			PA0=1; PA1=1; PA2=1; PA3=1; PA4=0; PA5=1;
		}
		if (index_key_scan == 2)
		{
			PA0=1; PA1=1; PA2=1; PA3=0; PA4=1; PA5=1;
		}
		NVIC_EnableIRQ(GPAB_IRQn);
	}
	
	if (cnt_5ms % 200 == 0) cnt_1s++;	

  TIMER_ClearIntFlag(TIMER1); // Clear Timer1 time-out interrupt flag
}

void GPAB_IRQHandler(void)
{
		NVIC_DisableIRQ(GPAB_IRQn);
	
if (PA->ISRC & BIT0) {        // check if PA0 interrupt occurred
		PA0=1;
	  PA->ISRC |= BIT0;         // clear PA0 interrupt status
	
if (PA3==0) { KEY_Flag =3; PA3=1;}
if (PA4==0) { KEY_Flag =6; PA4=1;}
if (PA5==0) { KEY_Flag =9; PA5=1;}
return;			
} 
if (PA->ISRC & BIT1) { // check if PA1 interrupt occurred
		PA1=1;
	  PA->ISRC |= BIT1;         // clear PA1 interrupt status  
if (PA3==0) { KEY_Flag =2; PA3=1;}
if (PA4==0) { KEY_Flag =5; PA4=1;}
if (PA5==0) { KEY_Flag =8; PA5=1;} 
return;				
} 
if (PA->ISRC & BIT2) { // check if PB14 interrupt occurred
		PA2=1;
	  PA->ISRC |= BIT2;         // clear PA interrupt status  
if (PA3==0) { KEY_Flag =1; PA3=1;}
if (PA4==0) { KEY_Flag =4; PA4=1;}
if (PA5==0) { KEY_Flag =7; PA5=1;}
return;				
}                     // else it is unexpected interrupts
PA->ISRC = PA->ISRC;	      // clear all GPB pins
}


void Init_Timer1(void)
{
  TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 200);
  TIMER_EnableInt(TIMER1);
  NVIC_EnableIRQ(TMR1_IRQn);
  TIMER_Start(TIMER1);
}

void Init_KEY(void)
{
		GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 |BIT3 | BIT4 | BIT5), GPIO_MODE_QUASI);
		GPIO_EnableInt(PA, 0, GPIO_INT_LOW);
		GPIO_EnableInt(PA, 1, GPIO_INT_LOW);
		GPIO_EnableInt(PA, 2, GPIO_INT_LOW);		
		NVIC_EnableIRQ(GPAB_IRQn);   
	  NVIC_SetPriority(GPAB_IRQn,3);
		GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_256);
		GPIO_ENABLE_DEBOUNCE(PA, (BIT0 | BIT1 | BIT2));			
}


void I2C1_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GET_STATUS(I2C1);
    if(I2C_GET_TIMEOUT_FLAG(I2C1))
    {
        I2C_ClearTimeoutFlag(I2C1); // Clear I2C1 Timeout Flag
    }
    else
    {
        if(s_I2C1HandlerFn != NULL)
            s_I2C1HandlerFn(u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Rx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterRx(uint32_t u32Status)
{
    if(u32Status == 0x08)                       // START has been transmitted and prepare SLA+W
    {
        I2C_SET_DATA(I2C1, (g_u8DeviceAddr << 1));    // Write SLA+W to Register I2CDAT
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x18)                  // SLA+W has been transmitted and ACK has been received
    {
        I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x20)                  // SLA+W has been transmitted and NACK has been received
    {
        I2C_STOP(I2C1);
        I2C_START(I2C1);
    }
    else if(u32Status == 0x28)                  // DATA has been transmitted and ACK has been received
    {
        if(g_u8DataLen != 2)
        {
            I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STA_SI);
        }
    }
    else if(u32Status == 0x10)                  // Repeat START has been transmitted and prepare SLA+R
    {
        I2C_SET_DATA(I2C1, ((g_u8DeviceAddr << 1) | 0x01));   // Write SLA+R to Register I2CDAT 
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x40)                  // SLA+R has been transmitted and ACK has been received
    {
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x58)                  // DATA has been received and NACK has been returned
    {
        g_u8RxData = (unsigned char) I2C_GET_DATA(I2C1);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STO_SI);
        g_u8EndFlag = 1;
    }
    else
    {
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Tx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterTx(uint32_t u32Status)
{
    if(u32Status == 0x08)                        // START has been transmitted
    {
        I2C_SET_DATA(I2C1, g_u8DeviceAddr << 1); // Write SLA+W to Register I2CDAT
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x18)                   // SLA+W has been transmitted and ACK has been received
    {
        I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x20)                  // SLA+W has been transmitted and NACK has been received
    {
        I2C_STOP(I2C1);
        I2C_START(I2C1);
    }
    else if(u32Status == 0x28)                  // DATA has been transmitted and ACK has been received
    {
        if(g_u8DataLen != 3)
        {
            I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STO_SI);
            g_u8EndFlag = 1;
        }
    }
    else
    {
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

void Close_I2C(void)
{
    I2C_DisableInt(I2C1);       // Disable I2C1 interrupt generation
    NVIC_DisableIRQ(I2C1_IRQn); // Disable NVIC I2C1 interrupt input
    I2C_Close(I2C1);            // Disable I2C1 control module
    CLK_DisableModuleClock(I2C1_MODULE); // Disable I2C module clock
}

void Init_I2C(void)
{
    I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);
    printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C1)); // get I2C1 clock

    I2C_EnableInt(I2C1);       // Enable I2C1 interrupt generation
    NVIC_EnableIRQ(I2C1_IRQn); // Enable NVIC I2C1 interrupt input
}

void EEPROM_Write(uint16_t addr, uint8_t data)
{
	uint8_t i;
  g_u8DeviceAddr = 0x50; // 24LC64 device address = 0xA0
	g_au8TxData[0] = (uint8_t)((addr & 0xFF00) >> 8);
  g_au8TxData[1] = (uint8_t)(addr & 0x00FF);
	g_au8TxData[2] = data;

  g_u8DataLen = 0;
  g_u8EndFlag = 0;

  s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterTx; // I2C function to write data to slave
  I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STA); // I2C as master sends START signal 
  while(g_u8EndFlag == 0);                  // Wait I2C Tx Finish 
  g_u8EndFlag = 0;
}

uint8_t EEPROM_Read(uint16_t addr)
{
  s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterRx; // I2C function to read data from slave
  g_u8DataLen = 0;
  g_u8DeviceAddr = 0x50;
	g_au8TxData[0] = (uint8_t)((addr & 0xFF00) >> 8);
  g_au8TxData[1] = (uint8_t)(addr & 0x00FF);
	
  I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STA);      
  while(g_u8EndFlag == 0);// Wait I2C Rx Finish
  g_u8EndFlag = 0;
	
	return g_u8RxData;
}





uint32_t i;

void EINT1_IRQHandler(void)// IRQ handler of GPIO
{
    GPIO_CLR_INT_FLAG(PB, BIT15);	// Clear GPIO interrupt flag
		test_mode=1;
		PA12=1;PA13=1;PA14=0;
	clear_LCD();
	
}
void splitDigits(int32_t num, int dig[4]) {
    dig[0] = num / 1000;        // Thousands place
    dig[1] = (num / 100) % 10;  // Hundreds place
    dig[2] = (num / 10) % 10;   // Tens place
    dig[3] = num % 10;          // Units place
}
void Init_EXTINT(void)// init the GPIO Set as interrupt
{
    // Configure EINT1 pin and enable interrupt by rising and falling edge trigger
    GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT);// as an input
    GPIO_EnableEINT1(PB, 15, GPIO_INT_RISING); // RISING, FALLING, BOTH_EDGE, HIGH, LOW
    NVIC_EnableIRQ(EINT1_IRQn);//NVIC can hear

    // Enable interrupt de-bounce function and select de-bounce sampling cycle time
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PB, BIT15);
}

int rdm_seed;
int show_table[10];
int rdm_array[4];
int rdm_value;
int j;
void boot_rdm(int seed, int *result){
	
    bool used[10]; // ???????????
    int count = 0;           // ??????????

    srand(seed); // ??????

    while (count < 4) {
        int digit = rand() % 10; // ??0-9???
        if (!used[digit]) {      // ????????
            result[count] = digit;
            used[digit] = true; // ????????
            count++;
        }
    
}


}
int count_down;
int rounddown;

bool gamestart;

void TMR0_IRQHandler(void) //do when catch tmr0 interrupt
{	
	//PB12=0;
	//PD12=0;
	
  TIMER_ClearIntFlag(TIMER0); // Clear Timer1 time-out interrupt flag
	count_down++;
	count_down=count_down%60;
	digit[0]=count_down%10;
	digit[1]=count_down/10;
rounddown--;
	if(rounddown==0){
	gamestart=1;
		rounddown=3;
	}
	//need to leave this kind of interrupt
	
}
void Init_Timer0(void) //battle timer
{
  TIMER_Open(TIMER0, TMR0_OPERATING_MODE, 1);// set timer configuration
  TIMER_EnableInt(TIMER0);// this timer can throw interrupt
  NVIC_EnableIRQ(TMR0_IRQn); //NVIC can catch this interrupt
  
}
void print_Line5x7(int8_t line, char text[])
{
	int8_t i;
	for (i=0;i<strlen(text);i++) 
		printC_5x7(i*8,line*11,text[i]);
}
volatile uint32_t u32ADCvalue;
volatile uint8_t u8ADF;	
void ADC_IRQHandler(void) //when ADC interrupt, do this 
{		//every time when ADC finish its conversion, will do interrupt
    uint32_t u32Flag;

    // Get ADC conversion finish interrupt flag
    u32Flag = ADC_GET_INT_FLAG(ADC, ADC_ADF_INT);
	
    if(u32Flag & ADC_ADF_INT)
        u8ADF = 1;
		//when IRQ is ok, u8AGF=1

    ADC_CLR_INT_FLAG(ADC, u32Flag);
		/*
		just check, don't have to understand
		*/
}

void Init_ADC(void)
{
    ADC_Open(ADC, ADC_INPUT_MODE, ADC_OPERATION_MODE, ADC_CHANNEL_MASK);
	//which ADC,  what input mode, op mode , which channel(pin) should measure(for here, is variable resistance)
    ADC_POWER_ON(ADC);
	//turn on ADC
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
}
void Init_PWM(void)
{
    PWM_ConfigOutputChannel(PWM0, PWM_CH0, 60, 50);
		
    PWM_ConfigOutputChannel(PWM0, PWM_CH1, 60, 50);
    PWM_EnableOutput(PWM0, (PWM_CH_0_MASK | PWM_CH_1_MASK));
    PWM_Start(PWM0, (PWM_CH_0_MASK | PWM_CH_1_MASK));
}
volatile uint32_t d1,d2,d3,d4;
bool touchp2=false;
bool touchp1=0;
void istouchp2(){
	//PD12=0;
//if((ball.y+8==p2.y)&&(ball.x>=p2.x)&&(ball.x<=p2.x+16)){
	if((ball.y+8>=p2.y)&&(ball.x>=p2.x)&&(ball.x<=p2.x+16)){

touchp2=TRUE;
}
	//touchp2=0;
	
};
void playwall(){
	
PWM_ConfigOutputChannel(PWM1, PWM_CH0, 226, 90);
 CLK_SysTickDelay(16000);
	PWM_ConfigOutputChannel(PWM1, PWM_CH0, 226, 100);
}
void playpaddle(){
	
PWM_ConfigOutputChannel(PWM1, PWM_CH0, 459, 90);
 CLK_SysTickDelay(96000);
	PWM_ConfigOutputChannel(PWM1, PWM_CH0, 226, 100);
}
void playpoint(){
	
PWM_ConfigOutputChannel(PWM1, PWM_CH0, 490, 90);
 CLK_SysTickDelay(257000);
	PWM_ConfigOutputChannel(PWM1, PWM_CH0, 226, 100);
}
void istouchp1(){
	//PD12=0;
//if((ball.y+8==p2.y)&&(ball.x>=p2.x)&&(ball.x<=p2.x+16)){
	if((ball.y<=p1.y+4)&&(ball.x>=p1.x)&&(ball.x<=p1.x+16)){

touchp1=TRUE;
}
	//touchp2=0;
	
};
void playgameover(){
	 int music_round=0;
	
	for(music_round=0;music_round<90;music_round++){
		//	PWM_ConfigOutputChannel(PWM1, PWM_CH0, game_over_song_tone[music_round], 90); // 0=Buzzer ON
		
			
		 // CLK_SysTickDelay(game_over_song_beat[music_round]);
	PWM_ConfigOutputChannel(PWM1, PWM_CH0, game_over_song_tone[music_round], 90); // 0=Buzzer ON
		
			
		  CLK_SysTickDelay(game_over_song_beat[music_round]);
	
	}
	 //music_round++;
	// music_round=music_round%92;
	 
 
 };

void Init_UART(void)
{
  UART_Open(UART0, 115200);                     // set UART0 baud rate
  UART_ENABLE_INT(UART0, UART_IER_RDA_IEN_Msk); // enable UART0 interrupt (triggerred by Read-Data-Available)
  NVIC_EnableIRQ(UART02_IRQn);	
//PD12=0;	// enable Cortex-M0 NVIC interrupt for UART02
}

int px[4];

int pindex=0;
void UART02_IRQHandler(void)
{
	
	uint8_t c;
	uint32_t u32IntSts = UART0->ISR;// read  the isr
PD12=0;
	if(u32IntSts & UART_IS_RX_READY(UART0)) // check ISR on & RX is ready , Rx is not empty!
  {
		while (!(UART0->FSR & UART_FSR_RX_EMPTY_Msk)){ // check RX is not empty
			
			
			//FIFO state register  
			//warning!�@this is & , not && , so this is bitwise operation not logical
			c = UART_READ(UART0); // read UART RX data
			//read from buffer
			//printC(8, 16, c);
			p1.x=p1.x*10+c-'0';
			pindex++;
			
			
			if(pindex>=3){
			p1.x=128-p1.x;
			}
			
			
			
			
		
			
		
			
			
	
			
					
				
			
		}
		//exit while when buffer is empty
	}
}
uint8_t ASSCII;
int32_t main(void)
{
  u8ADF = 0;
	
	 Init_PWM();
	
	
	
	
	
Init_EXTINT();
  SYS_Init();//initial
   Init_I2C();
	 Init_Timer1();
	Init_KEY();
init_LCD();
clear_LCD();
	  OpenSevenSegment();
	Init_ADC();   
			    PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
    PWM_Start(PWM1, PWM_CH_0_MASK);	
	
	Init_UART();
	Init_Timer0();
	rdm_seed=EEPROM_Read(0)+1397;

	boot_rdm(rdm_seed,rdm_array);
	
	rdm_value=rdm_array[0]*1000+rdm_array[1]*100+rdm_array[2]*10+rdm_array[3];
	EEPROM_Write(0, rdm_value);
	sprintf(text,"  Press any key");
	for (i=0;i<strlen(text);i++) 
		printC_5x7(i*8,25,text[i]);
	sprintf(text,"    to start   ");
		for (i=0;i<strlen(text);i++) 
		printC_5x7(i*8,40,text[i]);
	// print_Line5x7(3,text);
	//printS("Press any key");
	
	while(KEY_Flag==0){
	};
	clear_LCD();
	
	

	
  s_I2C1HandlerFn = NULL;
  Close_I2C(); // close I2C
	//while(test_mode==0){};
	 TIMER_Start(TIMER0);
	
	
	 draw_Bmp_axb(p1.x, p1.y, 1,0, p1.p, 16,4);
	p2.x=128-16;
	p2.y=60;
	 draw_Bmp_axb(p2.x, p2.y, 1,0, p1.p, 16,4);
	
	p1.y=0;
	
	ASSCII=p2.x/100;
	
	
	 draw_Bmp_axb(ball.x, ball.y, 1,0, ball.p, 8,8);
	
	if(rdm_value%4==0){
		setDirectionUp();
		 setDirectionLeft();
	}
	else if(rdm_value%4==1){
		setDirectionUp();
		setDirectionRight();
	}
	else if(rdm_value%4==2){
		setDirectionRight();
		setDirectionDown();
	}
		else{
			setDirectionDown();
		 setDirectionLeft();
		};
	
	
	while(score1!=3&&score2!=3){ 
		digit[2]=score2;
		digit[3]=score1;
		 CLK_SysTickDelay(50000);
		
		

	

		//p2.x=128-16-p1.x;
	p2.y=64-4-p1.y;		
		  draw_Bmp_axb(p2.x, p2.y, 0,0, p1.p, 16,4);
	
		draw_Bmp_axb(p1.x, p1.y, 0,0, p1.p, 16,4);
		
		
		draw_Bmp_axb(ball.x, ball.y, 0,0, ball.p, 8,8);
		
		/*
		ball.x+=dirX * movX; 
		ball.y+= dirY * movY; 
		*/
		
		if(ball.y>LCD_Ymax-8){
				ball.y=LCD_Ymax-8;
			dirY=-1;
			score1++;
			playpoint();
			
		}
		if(ball.y<0){
			ball.y=0;
			dirY=1;
			score2++;
				playpoint();
		}
		if(ball.x>LCD_Xmax-8){
			ball.x=LCD_Xmax-8;
			dirX=-1;
			playwall();
		}
		if(ball.x<0){
			ball.x=0;
			dirX=1;
			playwall();
		}
		
		
		
		
		
		
	//	rdm_value=1735;
		//splitDigits(rdm_value,digit);
		 ADC_START_CONV(ADC);
			
      while (u8ADF == 0);
      u32ADCvalue = ADC_GET_CONVERSION_DATA(ADC, 7);
	
		
	//	p1.x=u32ADCvalue*112/4096;
		draw_Bmp_axb(p1.x, p1.y, 1,0, p1.p, 16,4);
			p2.x=128-16-u32ADCvalue*112/4096;;
	p2.y=64-4-p1.y;
	
splitDigits(p2.x,px);
		draw_Bmp_axb(p1.x, p1.y, 0,0, p1.p, 16,4);
		ASSCII=px[0]+'0';
	UART_Write(UART0, &ASSCII, 1);
ASSCII=px[1]+'0';
	UART_Write(UART0, &ASSCII, 1);
		ASSCII=px[2]+'0';
	UART_Write(UART0, &ASSCII, 1);
		 draw_Bmp_axb(p1.x, p1.y, 1,0, p1.p, 16,4);
		
	  draw_Bmp_axb(p2.x, p2.y, 1,0, p1.p, 16,4);
	 //	printC(16,32,'4');
	istouchp2();
	
		if(touchp2){
			touchp2=0;
			//printC(16,32,'8');
			ball.y=LCD_Ymax-12;
			dirY=-1;
			playpaddle();
			//PD12=0;
			
		
			
		
		}
		istouchp1();
	
		if(touchp1){
			touchp1=0;
			//printC(16,32,'8');
			ball.y=4;
			dirY=1;
			playpaddle();
		//	PD12=0;
			
		
			
		
		}
		ball.x+=dirX * movX; 
		ball.y+= dirY * movY; 
		draw_Bmp_axb(ball.x, ball.y, 1,0, ball.p, 8,8);
		
		
		
	//PWM_ConfigOutputChannel(PWM1, PWM_CH0, u32ADCvalue, 90);
	}

	clear_LCD();
	while(1){
	playgameover();
	}
	}





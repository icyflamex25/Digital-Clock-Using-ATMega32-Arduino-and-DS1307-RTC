#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD PORTA 
#define EN 7
#define RW 6
#define RS 5

void lcdcmd(unsigned char cmd)
{
	PORTD &=~(1<<RS);
	PORTD &=~(1<<RW);
	LCD=cmd&0xF0;
	PORTD |=(1<<EN);
	_delay_ms(1);
	PORTD &=~(1<<EN);
	LCD = cmd<<4;
	PORTD |=(1<<EN);
	_delay_ms(1);
	PORTD &=~(1<<EN);
}

 void lcddata(unsigned char data)
 {
	 PORTD |=(1<<RS);
	 PORTD &=~(1<<RW);
	 LCD=data&0xF0;
	 PORTD |=(1<<EN);
	 _delay_ms(1);
	 PORTD &=~(1<<EN);
	 LCD=data<<4;
	 PORTD |=(1<<EN);
	 _delay_ms(1);
	 PORTD &=~(1<<EN);
 }
 
void lcd_init()
{
	
	DDRA=0XFF;
	DDRD=0xFF;
	PORTD &=~(1<<EN);
	lcdcmd(0x33);
	lcdcmd(0x32);
	lcdcmd(0x28);
	lcdcmd(0x0E);
	lcdcmd(0x01);
	_delay_ms(2);
}
void lcd_print(char *str)
{
	unsigned char i=0;
	while(str[i]!=0)
	{
		lcddata(str[i]);
		i++;
	}
}

void i2c_init(void)
{
	TWSR=0x00;
	TWBR=0X48;
	TWCR=0X04; 
}

void i2c_start(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}

void i2c_write(unsigned char data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}

unsigned char i2c_read(unsigned char ackVal)
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(ackVal<<TWEA);
	while((TWCR&(1<<TWINT))==0);
	return TWDR;
}

void i2c_stop()
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	for(int x=0;x<100;x++);
}

void rtc_init(void)
{
	i2c_init();
	i2c_start();
	i2c_write(0xD0);
	i2c_write(0x07);
	i2c_write(0x00);
	i2c_stop();
}

void rtc_setTime(unsigned char h,unsigned char m,unsigned char s)
{
	i2c_start();
	i2c_write(0xD0);
	i2c_write(0x00);
	i2c_write(s);
	i2c_write(m);
	i2c_write(h);
	i2c_stop();
}

void rtc_gettime(unsigned char *h,unsigned char *m,unsigned char *s)
{
	i2c_start();
	i2c_write(0xD0);
	i2c_write(0x00);
	i2c_stop();
	
	i2c_start();
	i2c_write(0xD1);
	*s=i2c_read(1);
	*m=i2c_read(1);
	*h=i2c_read(0);
	i2c_stop();
}

void send_packedBCD(unsigned char data)
{
	lcddata(0x30+(data>>4));
	_delay_ms(1);
	lcddata(0x30+(data&0x0F));
}

int main(void)
{
	unsigned char i,j,k;
	lcd_init();
	lcd_print((char *)"Time:");
	rtc_init();
	rtc_setTime(0x19,0x45,0x30);
	while(1)
	{
		lcdcmd(0x85);
		rtc_gettime(&i,&j,&k);
		send_packedBCD(i);
		lcd_print((char *)":");
		send_packedBCD(j);
		lcd_print((char *)":");
		send_packedBCD(k);
	}
	return 0;
}
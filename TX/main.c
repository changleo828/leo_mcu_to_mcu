#include <avr/io.h>
#include <util/delay.h>

#define bool unsigned char


#define bit_InSCL (0)
#define bit_InSDA (1)
#define bit_OutSCL (2)
#define bit_OutSDA (3)

#define OutSCL_1 PORTC |= (1<<bit_OutSCL);
#define OutSDA_1 PORTC |= (1<<bit_OutSDA);
#define OutSCL_0 PORTC &= ~(1<<bit_OutSCL);
#define OutSDA_0 PORTC &= ~(1<<bit_OutSDA);


void SendOneByte(unsigned char Data)
{
	unsigned char i;
	unsigned char temp = Data;
	
	OutSCL_0;
	for(i = 0;i < 8;i++){
	    if(temp & (0x80>>i)){
	        OutSDA_1;
	    }else{
	        OutSDA_0;
	    }
	    OutSCL_0;
	    OutSCL_1;
	}
}


#define IsInSDA() ((PINB >> bit_InSDA) & 0x01)

volatile bool Flag_InSCL_UP;

bool IsInSCLUp(void)
{
	bool temp;
	
	temp = (PINB >> bit_InSCL) & 0x01;
	if(Flag_InSCL_UP != temp){
	    Flag_InSCL_UP = temp;
	    if(temp == 1)
	        return 1;
	    else
	        return 0;
	}else{
	    return 0;
	}
}
unsigned char ReceiveOneByte(void)
{
    unsigned char i = 0;
    unsigned char temp = 0;
    do{
        if(IsInSCLUp()){
            temp <<= 1;
            i++;
		    if(IsInSDA())
		        temp |= 0x01;
		}    
    }while(i<8);
    return temp;
}
int main(void)
{
    unsigned char t;
	DDRC = 0xFF;
	DDRB = 0x00;
	Flag_InSCL_UP = 0;
	while(1)
	{
		t = ReceiveOneByte();
		SendOneByte(t);
	}
	return 0;
}
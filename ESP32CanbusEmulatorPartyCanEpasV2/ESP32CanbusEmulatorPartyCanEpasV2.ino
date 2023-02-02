//ver = 1; Working! uses brake signal (0x118) to start sending frames needed to chage RTR to = 0 (was 1 in can due)
//ver = 2; starts sending frames immediately - esp32 designed to be powered from EPAS wake pin

//CanTX pin - gpio 22
//CanRX pin - gpio 23

#include <esp32_can.h>

//Timers-------------------------------------------------------------------------------------
unsigned long timer10ms = 0;        // will store last time was updated
unsigned long timer100ms = 0;        // will store last time was updated
unsigned long timer1000ms = 0;        // will store last time was updated

//0x31 variables
byte crc0x31 =0xDA; //Set start crc to 0xDA when counter0x31==0x50;
byte counter0x31 = 0x50;

//0x51 variables
byte crc0x51 = 0xF3; //Set start crc to 0xF3 when counter0x51==0x60;
byte counter0x51 = 0x60;

//0x370 variables
byte counterByte6_0x370=0x20;
byte counterByte7_0x370=0x3E;  // counterByte6_0x370+0x1e(30)

//0x392 variables
byte EPAS3P_matrixIndex =0x00;

CAN_FRAME outframe;

void Frames10MS()
{

//EPAS Party 0x31
        outframe.id = 0x31;            // Set our transmission address ID
        outframe.length = 3;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=crc0x31;
        outframe.data.uint8[1]=counter0x31; //counter running from 0x50 (80) to 0x5F (95)
        outframe.data.uint8[2]=0x02;//always 0x02
        Can0.sendFrame(outframe); 
          //Byte 1 calc
          counter0x31++;
          //if (counter0x31==0x60)
          //{counter0x31=0x50;}
              //Byte 0 Look up table
              switch (counter0x31) {
                  case 0x50: crc0x31=0xDA; break; //when counter0x31==50 then crc0x31=0xDA;
                  case 0x51: crc0x31=0x96; break; 
                  case 0x52: crc0x31=0x42; break; 
                  case 0x53: crc0x31=0x0E; break;
                  case 0x54: crc0x31=0xF7; break;
                  case 0x55: crc0x31=0xBB; break; 
                  case 0x56: crc0x31=0x6F; break;
                  case 0x57: crc0x31=0x23; break;
                  case 0x58: crc0x31=0x80; break;
                  case 0x59: crc0x31=0xCC; break;
                  case 0x5A: crc0x31=0x18; break;
                  case 0x5B: crc0x31=0x54; break;
                  case 0x5C: crc0x31=0xAD; break;
                  case 0x5D: crc0x31=0xE1; break;
                  case 0x5E: crc0x31=0x35; break;
                  case 0x5F: crc0x31=0x79; break; 
                  case 0x60: crc0x31=0xDA; counter0x31=0x50;  break;
                }

//EPAS Party 0x51
        outframe.id = 0x51;            // Set our transmission address ID
        outframe.length = 3;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=crc0x51;
        outframe.data.uint8[1]=counter0x51; //increment +1, counter running from 0x60 (96) to 0x6F (111)
        outframe.data.uint8[2]=0x0a;//always 0x0A
        Can0.sendFrame(outframe); 
          //Byte 1 calc
          counter0x51++;
          if (counter0x51==0x70)
          {counter0x51=0x60;}
              //Byte 0 Look up table
              switch (counter0x51) {
                  case 0x60: crc0x51=0xF3; break; 
                  case 0x61: crc0x51=0xBF; break; 
                  case 0x62: crc0x51=0x6B; break; 
                  case 0x63: crc0x51=0x27; break;
                  case 0x64: crc0x51=0xDE; break;
                  case 0x65: crc0x51=0x92; break; 
                  case 0x66: crc0x51=0x46; break;
                  case 0x67: crc0x51=0x0A; break;
                  case 0x68: crc0x51=0xA9; break;
                  case 0x69: crc0x51=0xE5; break;
                  case 0x6A: crc0x51=0x31; break;
                  case 0x6B: crc0x51=0x7D; break;
                  case 0x6C: crc0x51=0x84; break;
                  case 0x6D: crc0x51=0xC8; break;
                  case 0x6E: crc0x51=0x1C; break;
                  case 0x6F: crc0x51=0x50; break; 
                }

//EPAS Party 0x370
        outframe.id = 0x370;            // Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=0x31;
        outframe.data.uint8[1]=0xFF;
        outframe.data.uint8[2]=0x08;
        outframe.data.uint8[3]=0x08;
        outframe.data.uint8[4]=0x20;
        outframe.data.uint8[5]=0x4B;
        outframe.data.uint8[6]= counterByte6_0x370;
        outframe.data.uint8[7]= counterByte7_0x370;
        Can0.sendFrame(outframe); 
          //byte 6 calc
          counterByte6_0x370++;
          if (counterByte6_0x370==0x30){ counterByte6_0x370=0x20;}
          //byte 7 calc
          counterByte7_0x370= counterByte6_0x370+0x1E;
                
}//end Frames 10ms-------------------------------------------------------------------



void Frames1000MS()
{

//EPAS Party 0x331  
        outframe.id = 0x331;            // EPAS3P_info, Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=0x0B; //PCBAID_ASSYID_USAGEID
        outframe.data.uint8[1]=0x00;
        outframe.data.uint8[2]=0x01;
        outframe.data.uint8[3]=0x01;
        outframe.data.uint8[4]=0x01;
        outframe.data.uint8[5]=0x00;
        outframe.data.uint8[6]=0x00;
        outframe.data.uint8[7]=0x00;
        Can0.sendFrame(outframe); 

        outframe.id = 0x331;            // EPAS3P_info, Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=0x0D; //APP_CRC
        outframe.data.uint8[1]=0x00;
        outframe.data.uint8[2]=0x00;
        outframe.data.uint8[3]=0x00;
        outframe.data.uint8[4]=0xB5;
        outframe.data.uint8[5]=0xB5;
        outframe.data.uint8[6]=0x7B;
        outframe.data.uint8[7]=0x44;
        Can0.sendFrame(outframe); 

        outframe.id = 0x331;            // EPAS3P_info, Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=0x14; //UDS_PROTOCOL_BOOTCRCC
        outframe.data.uint8[1]=0x05;
        outframe.data.uint8[2]=0x00;
        outframe.data.uint8[3]=0x00;
        outframe.data.uint8[4]=0x41;
        outframe.data.uint8[5]=0x00;
        outframe.data.uint8[6]=0x65;
        outframe.data.uint8[7]=0x50;
        Can0.sendFrame(outframe); 

        outframe.id = 0x331;            // EPAS3P_info, Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=0x0A; //BUILD_HWID_COMPONENTID
        outframe.data.uint8[1]=0x01;
        outframe.data.uint8[2]=0x00;
        outframe.data.uint8[3]=0x00;
        outframe.data.uint8[4]=0x00;
        outframe.data.uint8[5]=0x00;
        outframe.data.uint8[6]=0x31;
        outframe.data.uint8[7]=0x00;
        Can0.sendFrame(outframe); 


//EPAS Party 0x392
        outframe.id = 0x392;            // Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=EPAS3P_matrixIndex;
        outframe.data.uint8[1]=0x00;
        outframe.data.uint8[2]=0x00;
        outframe.data.uint8[3]=0x00;
        outframe.data.uint8[4]=0x00;
        outframe.data.uint8[5]=0x00;
        outframe.data.uint8[6]=0x00;
        outframe.data.uint8[7]=0x00;
        Can0.sendFrame(outframe); 

        //Calculate next index number
        EPAS3P_matrixIndex++;
          if (EPAS3P_matrixIndex==0x03)
          {EPAS3P_matrixIndex=0x00;}

//EPAS Party 0x3D1
        outframe.id = 0x3D1;            // Set our transmission address ID
        outframe.length = 8;            // Data payload
        outframe.extended = 0;          // Extended addresses - 0=11-bit 1=29bit
        outframe.rtr=0;                 //No request
        outframe.data.uint8[0]=0x91;
        outframe.data.uint8[1]=0x91;
        outframe.data.uint8[2]=0xFB;
        outframe.data.uint8[3]=0x07;
        outframe.data.uint8[4]=0x7D;
        outframe.data.uint8[5]=0x00;
        outframe.data.uint8[6]=0x00;
        outframe.data.uint8[7]=0x00;
        Can0.sendFrame(outframe); 

}//end Frames 1000ms-------------------------------------------------------------------- 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
//Serial.begin(115200);  

CAN_cfg.tx_pin_id = GPIO_NUM_22;
CAN_cfg.rx_pin_id = GPIO_NUM_23;
CAN0.begin(500000);// Initialize CAN0 and set baud rate.

}//end void setup--------------------------------------------------------------------------------------------

void loop(){
if (millis() >= timer10ms + 10) {
timer10ms=millis();
Frames10MS();
//Serial.println("Frames10MS sent..."); 
}

if (millis() >= timer1000ms + 1000) {
timer1000ms=millis();
Frames1000MS();
//Serial.println("Frames1000MS sent..."); 
}

}//end void loop---------------------------------------------------------------------------------------------

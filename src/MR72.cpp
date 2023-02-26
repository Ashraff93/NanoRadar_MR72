#include "MR72.h"
#include "Crc8.h"

extern float Param_Value_Avoid;
extern unsigned long PORT_SENSOR_TIMEOUT_MS;

/** Begin HardwareSerial
 * @brief Initialization HardwareSerial
 * @param hwStream Serial Number
 * @param baudRate Set to 115200
 * @return length of the message in bytes (excluding serial stream start sign)
 */
void MR72::begin(uint32_t baudRate){
  if (hwStream){
    hwStream->begin(baudRate);
    /**Smoothing configuration
     * @brief Smothing funtion begin
      * // Sec_c.begin(SMOOTHED_AVERAGE, 10);  
      * // Sec_l.begin(SMOOTHED_AVERAGE, 10);  
      * // Sec_r.begin(SMOOTHED_AVERAGE, 10); 
    */   
  }
}
/** Decode raw value from sensor
 * @brief Initialization HardwareSerial
 * @param Head byte 2Btyes Fixed value，’T’ ’H’ T ascii for a decimal 84 (0x54) H ascii for a decimal 72(0x48) 
 * @param D1 2Btyes The nearest target of Sector 2, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last
 * @param D2 2Btyes The nearest target of Sector 3, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last
 * @param D3 2Btyes The obstacle distance of 90 degree sector, if data invalid, writes 0xFFFF, the high 8-bit bytec omes first, and the low 8-bit byte comes last
 * @param D4 2Btyes The obstacle distance of 135 degree sector, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last
 * @param D5 2Btyes The obstacle distance of 180 degree sector, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last
 * @param D6 2Btyes The obstacle distance of 225 degree sector, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last
 * @param D7 2Btyes The obstacle distance of 270 degree sector, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last
 * @param D8 2Btyes The nearest target of Sector 1, if data invalid, writes 0xFFFF, the high 8-bit byte comes first, and the low 8-bit byte comes last CRC8 1Btyes CRC8 check, the checking algorithm please see CRC8 checking program
 * @param hwStream Serial Number
 * @param baudRate Set to 115200
 * @return length of the message in bytes (excluding serial stream start sign)
 */
uint8_t MR72::IncomingNewMessage(){
  if (hwStream->available() > 0){
    uint8_t inChar = hwStream->read();
    if (Index_val == 0){
      if (inChar == 'T'){//Looking for frame start 'T' as header char
        FramereceivedFS[Index_val++] = inChar;// we should always read 19 bytes THxxxxxxxxxxxxxxxxMC
      }
    }
    else if ((Index_val > 0) && (Index_val < 20)){
      FramereceivedFS[Index_val++] = inChar;//Gathering data
    }   
    if (Index_val == 19){// Check if the frame is two-by-two-pixel
      if (crc8(FramereceivedFS, 19) == FramereceivedFS[19]){//check if message has right CR
        Distance_Uart_sc  = (FramereceivedFS[2]<<8) + FramereceivedFS[3];//The nearest target of sector2
        Distance_Uart_sl = (FramereceivedFS[4]<<8) + FramereceivedFS[5];//The nearest target of sector3
        Distance_Uart_sr = (FramereceivedFS[16]<<8) + FramereceivedFS[17];//The nearest target of sector1
        /**Smoothing configuration
         * @brief Smothing Function
        * //  uint16_t Uart_sc  = (FramereceivedFS[2]<<8) + FramereceivedFS[3];//The nearest target of sector2
        * //  uint16_t Uart_sr = (FramereceivedFS[4]<<8) + FramereceivedFS[5];//The nearest target of sector3
        * //  uint16_t Uart_sl = (FramereceivedFS[16]<<8) + FramereceivedFS[17];//The nearest target of sector1
        * //  Sec_c.add(Uart_sc); // Add the new value to  sensor value stores
        * //  Sec_l.add(Uart_sl); // Add the new value to  sensor value stores
        * //  Sec_r.add(Uart_sr); // Add the new value to  sensor value stores
        * //  Distance_Uart_sc = Sec_c.get(); // Get the smoothed values
        * //  Distance_Uart_sl = Sec_l.get(); // Get the smoothed values
        * //  Distance_Uart_sr = Sec_r.get(); // Get the smoothed values
        */
        //To keep sending distance sensor for 
        if(Distance_Uart_sc > 4000){
          Distance_Uart_sc = 4000; //Replace value if exceed 40 meter
        }
        if(Distance_Uart_sl > 4000){
           Distance_Uart_sl = 4000;//Replace value if exceed 40 meter
        }
        if(Distance_Uart_sr > 4000){
           Distance_Uart_sr = 4000;//Replace value if exceed 40 meter
        }
        Obstacle_avoidance();//Check obs condiftion
        _last_distance_received_fs = millis(); //Set current milis
        Is_Data_MRA72_Event = true;//Set Is_Data_MRA72_Event to true
        Index_val = 0;//Reset Index_val
        FramereceivedFS[0] = 0;//Reset FramereceivedFS
      }
      else{//If Check crc fail, reset all
        Index_val = 0;
        FramereceivedFS[0] = 0;
          //Serial.println("CRC checks failed. Couldn't find valid frame in buffer length");
      }
    }
  }
  else if ((_last_distance_received_fs == 0) || (millis() - _last_distance_received_fs > PORT_SENSOR_TIMEOUT_MS)) {
    //If serrial not respond in 2 second, turn off led and set Is_Data_MRA72_Event to false
    Is_Data_MRA72_Event = false;  //Set Is_Data_MRA72_Event to false
  }
  return 0;
}



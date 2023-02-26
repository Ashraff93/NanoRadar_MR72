
/*
@author: Ruhaizan Fazreen Ashraff
@email: ruhaizanfazreen@gmail.com


  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
*/
#ifndef MR72_H
#define MR72_H

#include <Arduino.h> //Declared weak in Arduino.h to allow user redefinitions.
#include "Smoothed.h" //#include "Smoothed.h" To Smoothing Inconsistant Value 

class MR72 {
    // Class Member Variables
    // Constructor - creates a MR72
  private:
    HardwareSerial* hwStream; //Pointer class HardwareSerial hwStream

  public:
    MR72( HardwareSerial& device) {
      hwStream = &device;
      ;
    }//Pass reference to Serial object into a class
    // 
    void begin(uint32_t baudRate); //Void Function Parameter begin declaration
    uint8_t IncomingNewMessage(); //Void Function Parameter IncomingNewMessage declaration
    void Obstacle_avoidance(); //Variable Obstacle_avoidance declaration
    
    /**Smoothing configuration
     * @brief Smothing funtion begin
    * //Smoothed <uint16_t> Sec_c; //Create  instances of the class to use.
    * //Smoothed <uint16_t> Sec_l; //Create  instances of the class to use.
    * //Smoothed <uint16_t> Sec_r; //Create  instances of the class to use. 
    */
    //#Variable configuration#//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //#######################
    uint8_t FramereceivedFS[20]; //Reserve buffer
    uint16_t Distance_Uart_sl, Distance_Uart_sr, Distance_Uart_sc; 
    uint32_t _last_distance_received_fs; //Variable _last_distance_received_fs declaration
    uint8_t Index_val; //Index_Val declaration for index counting
    long Confirmation_Count;

    //#Boolen configuration#//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //#######################
    bool Is_Data_MRA72_Event = false; //bool Is_Data_MRA72_Event declaration
};

#endif
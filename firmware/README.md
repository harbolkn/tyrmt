#Firmware - Arduino program

This code should be uploaded to the Arduino.

##Current Functionality
  DATA SWITCH triggers transmission
    Transmission occurs pushing data from file over bluetooth
    One way communication, nothing is read from the computer
  IMU SWITCH triggers IMU recording
    IMU gathers quaternion data
    Prints 4 values to a line in file on SD
  All other cases do nothing

##Ideas
  Should data recording take presidence over data transmission?
  User must turn off recording to transmit?

  DATA
    Transmit data on read from serial
    Implement read function
    Should listen even when collecting data
    If signaled during collect, return ERROR
    Don't listen during transmit
    Start listening again after
    Listen during standby and stall

    Transmit all files on device
    Delete files after transmit

    Blink transmit led(red) when transmitting

  IMU
    3 second hold to start recording
    blink led (yellow) at each second to signal start
    keep led on during record
    1 second hold to stop recording
    turn off led when stopped

    Name file after time started recording

  Standby
    5 second button hold to turn off
    turn off Active LED (green)
    move to standby state after 3 minutes of inactivity
    must keep count of inactive time every loop (time check compare to start inactive time)

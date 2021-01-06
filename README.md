# esp8266-tiny-airplane

RC projecto, a tiny Airplane with a esp8266 TTGO T-Thruster board joined to Android app controller or M5StickC joystick.

## TODO

- [x] migrated basic code from [ESP8266 C Plane](https://www.instructables.com/WIFI-CONTROLLED-RC-PLANE/)
- [x] migrated CanAirIO OTA Handler to ESP8266
- [x] PlatformIO project with OTA support and FTDI (USB) support
- [x] Tested with original Android APK ok
- [ ] UDP control with nano protos 

![TTGO_T_Thruster_ESP8266_board](images/TTGO_T_Thrusted_ESP8266.jpeg)

## FTDI connection

For upload the code, you need have a serial connection, with a simple FTDI or another ESP8266 you can do it. The next image show the basic connection to USB FTDI:

![TTGO_T_Thruster_ESP8266_board](images/esp8266_to_FTDI.jpg)





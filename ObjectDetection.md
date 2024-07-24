# Object Detection Instructions:
Credit: https://www.sensingthecity.com/esp32-cam-real-time-object-detection/

# 1 Wire Parts
![image](https://github.com/user-attachments/assets/e78f9c93-956d-41d4-b6a7-06811025da35)

# 2 Install Board Manager
1. Add this “https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json” to File > Preferences > Additional Board Manager URL

![image](https://github.com/user-attachments/assets/3bc8ab41-fd02-427f-802e-e667b66f903a)

2. Open Board Manager (Tools > Board > Board Manager) and install esp32 library

![image](https://github.com/user-attachments/assets/69dede8d-8279-4ebb-a393-a087d7f93425)

# 3 Install Library
1. Go to this github and download library as zip file: https://github.com/yoursunny/esp32cam

![image](https://github.com/user-attachments/assets/e3a29d93-36a3-4b29-a44d-3d60703eb401)

2. Install library (Sketch > Include library > Add zip file)

![image](https://github.com/user-attachments/assets/4565a7c2-e073-4ec0-861e-08123e1463be)


# 4 Upload code to ESP32

1. Download the upload code folder and open it in the Arduino IDE.
 
2. connect I01 and GND of ESP32-CAM before uploading code

![image](https://github.com/user-attachments/assets/37e00f8d-4c93-4e5d-90d6-2059352c7b7d)

3. connect module with computer and check your FTDI’s port

![image](https://github.com/user-attachments/assets/290343ee-bca7-4dca-aaac-ee508cbf71fb)

4. set your board as ESP32 Wrover Module

![image](https://github.com/user-attachments/assets/4ae9de08-85b8-4966-a16e-6b93990da260)

5. set your port of your FTDI

![image](https://github.com/user-attachments/assets/913bea59-4c0d-4323-899e-678175362874)

6. set your flash frequency to 40MHz (Tools > Flash Frequency)

7. set your partition scheme like below

![image](https://github.com/user-attachments/assets/d4da6700-68ab-4965-8ba9-699ae836ff95)

8. set upload speed like below

![image](https://github.com/user-attachments/assets/7f49cfba-4b73-4677-a418-032551a2735f)

9. upload

![image](https://github.com/user-attachments/assets/df9e105b-04d1-4f13-b033-0483355baa01)

10. disconnect I01 and GND of ESP32-CAM

![image](https://github.com/user-attachments/assets/ba09d301-73c9-4854-9083-0d0c308f0f28)

11. push reset button to run your code

![image](https://github.com/user-attachments/assets/6791a0e5-db87-435f-b19e-41f5f98a840d)

12. check your serial monitor 115200 band

![image](https://github.com/user-attachments/assets/8b6109b4-5d13-4d74-91bb-7d706ea2f63f)

# 5 Run Object detection code

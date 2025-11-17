# Deck TeleOp

Deck TeleOp os a project aimed at using a Steam deck as a remote controller communicating wirelessly to an ESP-32 microcontroller over HTTP. The Steam Deck connects to an access point (AP) hosted by the ESP-32 and transmits the state of the gamepad controls to the ESP-32 which will then bind those actions to useful functions.


## Resources
- Valve Steam Deck
- ESP-32 S3 Dev Board

These resources will be used to connect to a variety of hardware on the ESP-32. Examples will be provided for the following to serve as a starting point for projects
- Blue LED with matching current limiting resistor (vendor dependent)
- SG90 servo
- 5V DC motor, with L298N H-bridge motor driver

## Getting Started
1. Install PyCharm on Steam Deck using Desktop mode through the web browser
2. Install Anydesk on main PC and Steam Deck (not required, but used to remotely control the Steam Deck from your main PC)
   - From the app store in Desktop mode
   - From your PC's web browser
3. Launch Anydesk (if desired) on both the Steam Deck and your PC
4. On your PC, enter the code shown on the Steam Deck and Click Connect. On the Steam Deck, accept the connection to start remote access
5. Launch PyCharm and start a new project. Create a main.py file to use for the desired example
6. copy the provided python code into the project
  - If missing any packages, click on terminal (not the python terminal) and run `pip install <whatever package>` and press enter. Repeat for each missing package
7. Run main.py file. If using gamepad controls, press the 'hamburger menu' button on the top right-hand side of the Steam Deck for 3 seconds to switch from desktop input mode to gamepad input mode
<img width="601" height="337" alt="image" src="https://github.com/user-attachments/assets/af1fad82-832a-42c2-bb50-d8d0024bc20b" />
8. In your Arduino IDE, upload the sketch to the ESP-32. This should start and access point for the Steam Deck to connect to.
  - Note down the IP address shown in the serial monitor, you may need to update the python script with this information and restart the script.
9. Connect the steam deck to the newly created AP. The password to the network is stored in your Arduino sketch.
  - Note that upon successful connection, any remote desktop control (AnyDesk in my case) will disconnect.
10. Success! You should see a gamepad preview that updates when a button is pressed. Depending on the eample code provided, you can also verify that arduino is working by going to your IP address in a browser and publishing the expected commands to manually trigger a reaction from the ESP-32

# Air Mouse - Fingers

![IMG_5269 HEIC](https://user-images.githubusercontent.com/94001472/177040157-737f1778-3771-4cc7-83c8-f245cec48298.jpg)


## Project

The project consists of a wearable mouse realized for the Embedded Systems project (2021/2022) – Computer Engineering, University of Salerno.
Using the accelerometers placed on the fingers, it is possible to interpret hand gestures in the following commands:
- Left Click
- Double Left Click
- Zoom In / Out (CTRL +/-)
- Alt + Tab

Furthermore, all commands performed with the mouse are recorded in a text file saved on a Micro SD card.


## Protocols and Interfaces
| Interfaces |                                                              |
| ---------- | ------------------------------------------------------------ |
| I2C1       | I²C port used for the accelerometers with addresses 0x00 on index, 0x02 on thumb. |
| I2C2       | I²C port used for the Real Time Clock module.                |
| SPI3       | SPI port used for the Micro SD reader.                       |
| USART2     | Serial port of the board used for communication with the computer. |

**On board LED:** Visual indicator of the system status:

- **ON** for “System ready” 
- **OFF** for “System un-ready”
## Software Architecture

![Architettura](https://user-images.githubusercontent.com/94001472/177040407-05c53b63-4f33-445c-80c6-68bcac26c0b9.png)


## Fritzing Scheme
![air mouse_bb](https://user-images.githubusercontent.com/94001472/177040185-c93f2e46-fbfe-44fa-a87f-c8d41847ac4d.png)

## Sotware Protocol: MCU - PC interaction
The interaction with the computer takes place via the serial port. The commands sent via serial are interpreted by the xdotool software available on Linux systems. The used commands are:

| Actions          | Commands                                                |
| ---------------- | ------------------------------------------------------- |
| Movements        | *xdotool mousemove_relative – 𝛥x 𝛥y*                    |
| Left Click       | *xdotool click 1*                                       |
| Double Click     | *xdotool click --repeat 2 1*                            |
| Zoom In          | *xdotool key Ctrl + plus*                               |
| Zoom Out         | *xdotool key Ctrl + minus*                              |
| Alt + Tab        | *xdotool keydown alt key Tab*                           |
| Window switching | *xdotool keydown Tab && sleep 0.1 && xdotool keyup Tab* |
| ALT+TAB Release  | *xdotool keyup alt*                                     |



## Video Tutorial

You can find a tutorial of how it works [here](https://www.youtube.com/watch?v=9bjMumtYkMI)


## Authors
Speranza Ranieri: [@httpsperanza](https://github.com/httpsperanza)

Ernesto Mancusi: [@ermancusi](https://github.com/ermancusi)

Giuseppe Parrella: [@gparrella12](https://github.com/gparrella12)

Francesco Sonnessa: [@s0nFra](https://github.com/s0nFra)



## How to Run
It's necessary to clone the repository in the STM32CubeIDE and run the project.
On PC side, you can run the command executor with the following commang: `sudo ./commandScript.sh`. After that, the commands received from the serial port are executed (note that in the script the default serial port is /dev/ttyACM0 , you can modify it by inserting the correct serial port detected in your PC for STM32). 

You can also execute the mouse commands by opening a Linux terminal and executing the following commands:
1.	`sudo bash`
2.	`exec<>/dev/ttyACM0`



## Who can contribute to this project?
Since this is an assignment project, only the authors can contribute to this project. Despite this, you are still free to use and distribute the code contained in this repository for your own projects according to **GNU GPL v3**.

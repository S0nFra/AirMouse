# Air Mouse - Fingers



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



## Gestures

<p align="center">|<img src="D:\OneDrive - Università di Salerno\UNISA\Magistrale\Secondo semestre\Sistemi Embedded\Project SE\Report & Presentation\Report\Documentation Figures\Figures\Figure 2\1 edited.jpg" alt="1 edited" style="width:300px;" />| <img src="D:\OneDrive - Università di Salerno\UNISA\Magistrale\Secondo semestre\Sistemi Embedded\Project SE\Report & Presentation\Report\Documentation Figures\Figures\Figure 2\2 edited.jpg" alt="2 edited" style="width:300px;" />|</p>

<p align="center"><i>movement to the right</i></p>



## Video Tutorial

You can find a tutorial of how it works [here](https://www.youtube.com/watch?v=9bjMumtYkMI)



## Authors
Speranza Ranieri: [@httpsperanza](https://github.com/httpsperanza)

Ernesto Mancusi: [@ermancusi](https://github.com/ermancusi)

Giuseppe Parrella: [@gparrella12](https://github.com/gparrella12)

Francesco Sonnessa: [@s0nFra](https://github.com/s0nFra)



## How to Run
It's necessary to clone the repository in the STM32ClubeIDE and run the project.




## Who can contribute to this project?
Since this is an assignment project, only the authors can contribute to this project. Despite this, you are still free to use and distribute the code contained in this repository for your own projects according to **GNU GPL v3**.
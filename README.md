This project is a implementation of a CHIP-8 emulator written in pure C.  
CHIP-8 is a virtual machine from the 1970s, often used as a starting point for emulator development.

## Features

- Full CHIP-8 CPU implementation
- 4KB memory model
- Opcode decoding and execution
- Input handling (keyboard)
- Graphics rendering (64x32 display) using SDL2
- Unit tests using Google Test (gtest)
- Containerized environment using Docker

<div align="center">
  <img src="images/pingpong.png" width="250"/>
  <img src="images/tetris.png" width="250"/>
  <img src="images/tank.png" width="250"/>
  <p><em>Chip-8 interpreter running classic ROMs: Ping Pong, Tetris, Tank</em></p>
</div>

This project includes unit tests using **Google Test**.

```bash
make test

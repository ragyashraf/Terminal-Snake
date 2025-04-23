
# 🐍 Terminal Snake (C++ + ncurses)

A fully animated, high-speed Snake game that runs entirely in your terminal — written in C++ with a clean, modular architecture and smooth visual updates.

This isn’t your average snake clone. It’s built to feel polished, fast, and satisfying with frame-based animation and real-time input handling.


---

## ✨ Features

- 🎮 Classic snake gameplay with real-time input
- 💥 Smooth animations using ncurses
- 🧠 Frame-based game loop and state system
- 🎯 Multiple difficulty levels and speed settings
- 🏆 High score tracking (stored locally)
- 🔄 Pause/resume and restart options
- 📈 Live score display during gameplay
- 💻 Runs on any terminal that supports ANSI/ncurses

---

## 🎮 Controls

| Action          | Key            |
|----------------|----------------|
| Move           | Arrow Keys / WASD |
| Pause/Resume   | `P`            |
| Quit Game      | `Q`            |
| Select/Menu    | `Enter`        |

---

## 🛠️ Build Instructions

### Prerequisites

- A C++11+ compiler (like `g++`)
- `ncurses` library

### For Ubuntu/Debian:

```bash
sudo apt install g++ libncurses5-dev
```

### For macOS (with Homebrew):

```bash
brew install ncurses
```

---

### 🚀 To Run the Game:

```bash
# Clone the repo
git clone https://github.com/yourusername/terminal-snake.git
cd terminal-snake

# Build it
make

# Run it!
./snake
```

---

## 📂 File Structure

```
terminal-snake/
├── src/
│   ├── main.cpp
│   ├── game.cpp
│   ├── snake.cpp
│   ├── apple.cpp
│   └── input.cpp
├── include/
│   └── *.h files
├── assets/
│   └── highscore.txt
├── Makefile
└── README.md
```

---

## 🔧 Credits

Made by **Ragy Ashraf**  
Discord Bots ⚙️ | AI ⚡ | Games 🎮 | Terminal Art 💻  
[GitHub →](https://github.com/ragyashraf)

---

> 🧠 _"This project helped me dive deeper into memory-safe rendering, real-time input, and modular design in C++. First time doing a full game in terminal with this level of polish."_  

---

## 🐍 Terminal Snake Vibes (ASCII Style)

```
       ____              _        
      / ___| _ __   __ _| | _____ 
      \___ \| '_ \ / _` | |/ / _ \
       ___) | | | | (_| |   <  __/
      |____/|_| |_|\__,_|_|\_\___|
              
     >>>   o o o O O O O *  <<<

```

---

## 🪪 License

MIT License

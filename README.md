# ⚡ TriviaProject

> Fast, fun, and hackable trivia game platform in C/C++

---

[![GitHub stars](https://img.shields.io/github/stars/Itay-Newman/TriviaProject?style=social)](https://github.com/Itay-Newman/TriviaProject/stargazers)
[![Forks](https://img.shields.io/github/forks/Itay-Newman/TriviaProject?style=social)](https://github.com/Itay-Newman/TriviaProject/network/members)
[![Last Commit](https://img.shields.io/github/last-commit/Itay-Newman/TriviaProject)](https://github.com/Itay-Newman/TriviaProject/commits/main)
[![License](https://img.shields.io/github/license/Itay-Newman/TriviaProject)](./LICENSE)
<!-- Add CI/build badges if/when available -->

---

## 📢 Important Architecture Note

**This repository contains two distinct Visual Studio 2022 solutions:**

1. **Trivia Server (C++ Solution):**
   - Located in: `Trivia Server/`
   - Solution file: [`Trivia-Server.sln`](https://github.com/Itay-Newman/TriviaProject/blob/AddHwFiles/Trivia%20Server/Trivia-Server.sln)
   - Implements the core trivia game server logic in C++.
   - Includes C++ source files (`*.cpp`, `*.h`), Visual Studio C++ project files (`Trivia.vcxproj`), and server-side logic.

2. **Trivia Client (WPF Solution):**
   - Located in: `Trivia Client/`
   - Intended for the graphical client, typically built with WPF (Windows Presentation Foundation).
   - (The provided file listing for the client is partial, but the structure implies a WPF or similar client solution is present and separate from the C++ server.)
   - Designed to provide a modern graphical user interface for interacting with the trivia server.

**These are separate solutions and should be opened and built independently in Visual Studio 2022.**  
**Do not confuse the C++ server project with the WPF client project.**

---

## 🎯 Features

- Fast and efficient C-based trivia game engine
- Extensible with C++ and Python components
- Command-line interface for local or networked play
- Modular question/answer framework
- Designed for learning, hacking, and extension
- Clean and simple architecture
- - C++ backend server for handling trivia logic and networking
- Intended WPF (C#) or Python client for user interaction
- Modular, extensible codebase
- Solutions are logically and physically separated for clarity and maintainability

## 📂 Folder Structure

```
TriviaProject/
├── HW/                # Documentation, diagrams, and homework files
├── Trivia Server/     # C++ backend server solution (Visual Studio C++)
├── Trivia Client/     # WPF client solution (Visual Studio WPF, C#)
```

- [Trivia Server/](https://github.com/Itay-Newman/TriviaProject/tree/AddHwFiles/Trivia%20Server): C++ source and solution files
- [Trivia Client/](https://github.com/Itay-Newman/TriviaProject/tree/AddHwFiles/Trivia%20Client): WPF or Python client (see repo for details)
- [HW/](https://github.com/Itay-Newman/TriviaProject/tree/AddHwFiles/HW): Diagrams, documents, and supporting materials
## 🛠️ Installation & Setup

### Prerequisites

- Visual Studio 2022\19 (Definitively recommend 2022 but whatever floats your boat)
- Make (recommended)
- Python (if using Python scripts)

## 🤖 Tech Stack

- **C** (core logic, 9%) There is more, but I didn't write it, it's JSON.hpp and SQLite3.c
- **C++** (extensions, 90%)
- **Python** (scripts/utilities, 1%)

## 🧠 Known Issues / TODO

- Some features may be incomplete or experimental.
- See source for TODO or FIXME comments.
- Open an issue for bugs or suggestions!
- There can only be one game at once.

## 🛠️ Building & Running

- Open the appropriate solution (`Trivia-Server.sln` for the server, the relevant solution file for the client) in Visual Studio 2022.
- Build and run each solution independently.
- (See source code and documentation for additional setup/configuration.)

## 🙌 Contributing

1. Fork and clone the repo
2. Create a feature branch (`git checkout -b feature/your-feature`)
3. Commit changes and push (`git push`)
4. Open a pull request

See `CONTRIBUTING.md` if available, or open an issue with any questions.

## 🪪 License

Distributed under the MIT License. See [LICENSE](./LICENSE) for details.

---

## 👑 Badges

- [x] Build status (add CI for badge)
- [x] License
- [x] Last commit
- [x] Stars
- [x] Forks

> Have fun and may your trivia reign supreme!

````markdown
# Expression Evaluator Compiler Project

## Overview

The **Expression Evaluator Compiler** project is a simple compiler designed to evaluate arithmetic expressions, manage variable declarations, and visualize parse trees. Built as part of a **Project-Based Learning (PBL)** initiative, it integrates a **C++ backend** with a **web-based frontend** using a **Node.js server**, demonstrating core compiler principles, full-stack development, and automated testing.

The project supports expression evaluation, variable scoping, error logging, and parse tree visualization—providing an interactive tool for learning about compilers.

---

## ✨ Features

- **Expression Evaluation**: Evaluates arithmetic expressions with operators `+`, `-`, `*`, `/`, `%`, and `^`.  
  Example:  
  ```c++
  x = 5 + 3 * 2;  // assigns x = 11
````

* **Variable Declaration**: Supports `int` variable declarations.
  Example:

  ```c++
  int a, b = 10;
  ```

* **Variable Scoping**: Handles local and global scopes, supporting variable shadowing.

* **Error Logging**: Logs errors (e.g., undeclared variables) to `cpp/error_log.txt`.

* **Parse Tree Visualization**: Generates and visualizes parse trees using **Graphviz**, displayed as PNG in the UI.

* **Symbol Table Display**: Shows variable names, types, values, and scopes in the UI.

* **Frontend Validation**: Ensures user inputs are syntactically correct (e.g., expressions must end with `;`).

* **Automated Testing**: Validates functionality using `test.mjs`, covering evaluation, scoping, and error handling.

---

## 🛠 Prerequisites

* [Node.js](https://nodejs.org/) (v14 or higher)
* C++ Compiler (`g++`)
* [Graphviz](https://graphviz.org/download/) (ensure `dot` command is in PATH)
* OS: Windows/Linux/macOS (tested on Windows)

---

## ⚙️ Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/compiler-pbl-project.git
cd compiler-pbl-project
```

### 2. Install Node.js Dependencies

```bash
npm install
```

> Installs `express` for the server and `node-fetch` for testing.

### 3. Compile the C++ Program

#### For Windows:

```bash
g++ cpp/p1.cpp -o cpp/p1.exe
```

#### For Linux/macOS:

```bash
g++ cpp/p1.cpp -o cpp/p1
```

### 4. Install Graphviz

* Download and install from: [https://graphviz.org/download/](https://graphviz.org/download/)
* Ensure `dot` command is accessible from the terminal.

### 5. Start the Server

```bash
node server.js
```

> The server will run at [http://localhost:3000](http://localhost:3000)

---

## 💻 Usage

### Access the Web Interface

* Open a browser and go to: [http://localhost:3000](http://localhost:3000)

### Use the UI To:

* Enter expressions (e.g., `x = 5 + 3;`)
* Declare variables (e.g., `int x, y = 10;`)
* Display the symbol table
* Visualize parse trees
* Enter/exit scopes to test variable scoping

### Run Automated Tests

With the server running, open a new terminal:

```bash
node test.mjs
```

> This runs tests for variable declaration, expression evaluation, scope handling, and error logging.

---

## 📁 Folder Structure

```
compiler-pbl-project/
├── cpp/                    # C++ backend files
│   ├── p1.cpp             # Main C++ program
│   ├── p1.exe             # Compiled executable (Windows)
│   ├── parse_tree.dot     # DOT file for parse tree
│   ├── parse_tree.png     # Parse tree image
│   └── error_log.txt      # Error logs
├── public/                 # Frontend
│   ├── index.html         # HTML UI
│   ├── script.js          # Frontend JS logic
│   └── styles.css         # Styling
├── server.js              # Node.js backend
├── test.mjs               # Automated test script
├── test_cases.md          # Manual test cases
├── package.json           # Project metadata
└── README.md              # Documentation
```

---

## ✅ Testing and Validation

### Manual Testing

* Covered in `test_cases.md`:
  Includes tests for declarations, evaluation, scoping, error logging, tree visualization, and UI validation.

### Automated Testing

* `test.mjs` verifies critical functionality across all modules.

> ✅ All tests passed as of **May 19, 2025, 10:11 AM IST**.

---

## 🧠 Key Challenges and Solutions

* **C++-Node.js Communication**:
  Fixed output delays using `std::cout << std::unitbuf` in `p1.cpp` and added dynamic waiting in `server.js`.

* **ESM Compatibility**:
  Migrated to ESM (`import`) syntax in `server.js` and `test.mjs`. Set `"type": "module"` in `package.json`.

* **Exponentiation Bug**:
  Replaced `std::pow` with custom `intPow` function to correctly handle integer powers (e.g., `2 ^ 3` → `8`).

---

## 🚀 Future Improvements

* Add support for additional data types (e.g., `float`, `double`)
* Implement a more robust communication protocol (e.g., end-of-command markers)
* Deploy the application to a remote server for live access

---

## 📄 License

This project is developed solely for educational purposes as part of a PBL evaluation. **Not licensed for commercial use.**

---

## 📬 Contact

For access to the repository or more information, contact:

**Shaurya Pundir**
📧 [anjanavikrampundir@gmail.com](mailto:anjanavikrampundir@gmail.com)

---




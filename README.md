# PSI (Pascal Simple Interpreter)

PSI is a lightweight Pascal interpreter built entirely in **pure C++**.  
It allows you to run Pascal programs directly from the command line without the need for external compilers or virtual machines.

## Features
- Parses and executes standard Pascal code  
- Handles variables, functions, and control flow  
- Simple command-line interface for quick execution  
- No external dependencies (just a C++ compiler and `make`)  

## Getting Started

### Prerequisites
- **C++ compiler** (GCC/Clang recommended)
- **Make** (for building)

### Installation
Clone the repository and build the interpreter:
```bash
git clone https://github.com/PhilipGill402/PSI.git
cd PSI
make clean && make
```

### Usage
To run a Pascal program, execute:
```bash
./psi path/to/your_program.pas
```

For example:
```bash
./psi examples/helloworld.pas
```

### Example Pascal File
```pascal
program Main;
var a, b: integer;

function Square(x: integer): integer;
begin
    Square := x * x;
end;

begin
    a := 5;
    b := Square(a);
    writeln("Square of ", a, " is ", b);
end.
```

Run it:
```bash
./psi examples/square.pas
```

## Project Structure
- `src/` – Core interpreter source files  
- `include/` – Header files  
- `Makefile` – Build configuration  
- `examples/` – Sample Pascal programs  

## Contributing
Contributions are welcome!  
- Fork the repo and create a feature branch  
- Follow existing coding conventions (pure C++11 or later)  
- Submit a pull request with a clear description

## License
This project is licensed under the **MIT License** – feel free to use, modify, and distribute.

# 2D CAD Tool

A **Qt-based 2D CAD application** for drawing, manipulating, and managing basic 2D shapes (lines, rectangles, circles).  
Supports **undo/redo, zooming, panning, resizing, shape duplication, and JSON-based save/load functionality**.  

## **Features**
✅ Draw **Lines, Rectangles, Circles**  
✅ **Move, Resize, Duplicate, Delete** Shapes  
✅ **Undo/Redo** (Using `QUndoStack`)  
✅ **Pan & Zoom** (Middle Mouse Drag, Ctrl + Scroll)  
✅ **Save/Load** to/from **JSON Format**  
✅ **Right-click Context Menu** for Shape Actions  

---
## Screenshot
![image](https://github.com/user-attachments/assets/59774936-0048-4614-8a05-4f977a282f81)



## Setup & Build Instructions
### **Prerequisites**
- **Qt (Tested with Qt 6.x)**
- **CMake (Tested with version 3.x)**
- **C++ Compiler** (GCC, Clang, or MSVC)


### **Build Steps**
```sh
git clone https://github.com/SaadAhmad747/2D-CAD-Tool.git
cd 2D-CAD-Tool
mkdir build && cd build
cmake ..
make
./CADTool  # Run the application
```
## How This Project Was Created

### Project Setup
- **Created a Qt Widgets Application**:
  - Used Qt Creator to generate a Qt Widgets project.
  - Designed the UI with Qt Designer (`mainwindow.ui`).

### Implemented Core Components
- **Canvas (CanvasView)**:
  - Used `QGraphicsScene` and `QGraphicsView` to handle drawing.
- **Shape Classes**:
  - Implemented `QGraphicsItem`-based classes for each shape (rectangle, circle, line).

### Integrated Event Handling
- **Mouse Events**:
  - To draw, move, and resize shapes.
- **Keyboard Shortcuts**:
  - Undo (`Ctrl+Z`), Redo (`Ctrl+Y`), Delete (`Del`).

### Implemented Undo/Redo System
- Used `QUndoStack` to manage shape creation, deletion, and resizing.

### Added Save & Load Feature
- **Save**: Shapes are saved in JSON format.
- **Load**: Shapes are reconstructed in `CanvasView` from JSON data.

### Final Testing & Bug Fixes
- Debugged resizing issues.
- Ensured zoom and pan work correctly.
- Fixed undo/redo inconsistencies.

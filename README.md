# cube-opengl

# Project Build Instructions

This code is written and built using **Visual Studio 2022** and utilizes **OpenGL 3.3**. Please follow the steps below to configure and build the project.

## Dependencies
The project uses the following libraries:
- **GLFW**: Configured using **CMake**, then build the solution in **Visual Studio 2022**.
- **freetype**
- **GLAD**
- **GLM**
- **STB**

## Configuration Steps

### 1. Include Directories
Manually configure the paths for the libraries as follows:

1. Go to **Properties** -> **VC++ Directories** -> **Include Directories**.
2. Add the path to the libraries under `Libraries/include/`.

### 2. Library Directories
Similarly, configure the library paths:

1. Go to **Properties** -> **VC++ Directories** -> **Library Directories**.
2. Add the path to the libraries under `Libraries/lib/`.

### 3. Additional Dependencies
1. Go to **Linker** -> **Input** -> **Additional Dependencies**.
2. Add the following:
   - `glfw3.lib`
   - `opengl32.lib`

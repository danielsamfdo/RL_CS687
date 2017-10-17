CLion uses the CMake build system to configure projects so you need to configure a CMakeLists.txt file to define the project. There should be one in build/CLion that should work for Linux systems. 

Unzip the hw1.zip in whatever <project_dir> you want.
Place the CMakeLists.txt file in the project root: <project_dir>/hw1/ 
Create the folders: "build/CLion/output" if they are not already created. "build/CLion" will be where the project code runs. 

Open CLion and open the project from the CMakeLists.txt file.
Got into Settings->Build, Execution, Deployment->CMake
Edit the Debug config's generation path to build "build/CLion/Debug"
Add the following cmake options "-DUSE_MULTITHREADING=ON -DDISPLAY_GRAPHICS=ON" or switch these to OFF if you don't want to use them. 
Click Ok.
delete the cmake-build-debug folder created in the root directory at creation (this is replaced with "build/CLion/Debug")

To build:
Click on Run->build

Setup Run config:
Click on Run->Edit Configurations...
Select RLCPP if it exists if not create a new application with RLCPP as the target and the executable being RLCPP. 
Modify the working directory to be "<path_to_project>/build/CLion" (the code needs to be run in a folder that also has an "output" folder)
Hit Ok.

Then click Run

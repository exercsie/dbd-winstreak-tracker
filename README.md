# dbd-winstreak-tracker

## Dependencies
- [Dear ImGui](https://github.com/ocornut/imgui) (Installed as submodule): Graphics User Interface.
- [GLFW](https://www.glfw.org/): creates window.
- OpenGL: renders GUI.

## Install GLFW and OpenGL files:
### Fedora
```
sudo dnf install glfw-devel mesa-libGL-devel
```

### Arch
```
sudo pacman -S glfw mesa
```

### Debian
```
sudo apt install libglfw3 libglfw3-dev libgl1-mesa-dev
```

## Build
### CLI
```
git clone --recurse-submodules https://github.com/exercsie/dbd-winstreak-tracker
cd dbd-winstreak-tracker
make trackerCLI
```

### GUI
```
git clone --recurse-submodules https://github.com/exercsie/dbd-winstreak-tracker
cd dbd-winstreak-tracker
make trackerGUI
```

## Install
### CLI
```
sudo cp Build/trackerCLI /usr/local/bin
```

### GUI 
```
sudo cp Build/trackerGUI /usr/local/bin
```

**Now type "trackerCLI" or "trackerGUI" in any directory to run the program.**

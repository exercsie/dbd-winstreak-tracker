# dbd-winstreak-tracker

## Dependencies
- [Dear ImGui](https://github.com/ocornut/imgui) (Installed as submodule): Graphics User Interface.
- [GLFW](https://www.glfw.org/): creates window.
- OpenGL: renders GUI.

# Install GLFW and OpenGL files:
### Fedora
```
sudo dnf install glfw-devel mesa-libGL-devel
```

### Arch
```
sudo pacman -S glfw mesa
```

## Build
```
git clone --recurse-submodules https://github.com/exercsie/dbd-winstreak-tracker
cd dbd-winstreak-tracker
make
```

## Install
```
cd Build
sudo cp tracker /usr/local/bin
```

Now type, "tracker" in any directory to run the program.

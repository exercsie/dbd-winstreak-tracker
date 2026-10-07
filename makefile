IMGUI = Dependencies/imgui

GUI_FLAGS = -I${IMGUI} -I${IMGUI}/backends

# CLI version
TRACKER_FILES = \
	src/Main.cpp \
	src/Tracker.cpp

# GUI version
MYGUI_FILES = \
	src/Gui.cpp

IMGUI_FILES = \
	$(IMGUI)/imgui.cpp \
	$(IMGUI)/imgui_draw.cpp \
	$(IMGUI)/imgui_tables.cpp \
	$(IMGUI)/imgui_widgets.cpp \
	$(IMGUI)/backends/imgui_impl_glfw.cpp \
	$(IMGUI)/backends/imgui_impl_opengl3.cpp

CLI_SUFFIX_FLAGS = -std=c++23
GUI_SUFFIX_FLAGS = -lglfw -lGL -ldl -std=c++23

trackerCLI:
	g++ ${TRACKER_FILES} -o Build/trackerCLI ${CLI_SUFFIX_FLAGS}

trackerGUI:
	g++ ${GUI_FLAGS} ${MYGUI_FILES} ${IMGUI_FILES} -o Build/trackerGUI ${GUI_SUFFIX_FLAGS}

clean:
	rm -f Build/trackerCLI Build/trackerGUI
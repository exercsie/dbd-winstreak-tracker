TRACKER_FILES = \
src/Main.cpp \
src/Tracker.cpp

tracker Tracker:
	g++ ${TRACKER_FILES} -o Build/tracker -std=c++23

clean:
	rm tracker
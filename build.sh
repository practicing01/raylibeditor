set -e    # if error, stop the build
cc -g -std=c99 -c -I /opt/raylib/src raylibeditor.c -o ./obj/raylibeditor.o
cc -g -std=c99 -c -I /opt/raylib/src splash.c -o ./obj/splash.o
cc -g -std=c99 -c -I /opt/raylib/src menu.c -o ./obj/menu.o
cc -g -std=c99 -c -I /opt/raylib/src gameplay.c -o ./obj/gameplay.o
cc -g -std=c99 -c -I /opt/raylib/src drawgui.c -o ./obj/drawgui.o
cc -g -std=c99 -c -I /opt/raylib/src drawnodes.c -o ./obj/drawnodes.o
cc -o raylibeditor  obj/raylibeditor.o obj/splash.o obj/menu.o obj/gameplay.o obj/drawgui.o obj/drawnodes.o -s -Wall -std=c99 -I/opt/raylib/src -L/opt/raylib/release/libs/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./raylibeditor

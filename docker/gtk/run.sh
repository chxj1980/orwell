xhost local:root && sudo docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v ${PWD}../../../:/home/dev/orwell -e DISPLAY=unix$DISPLAY orwell_gtk_builder && xhost -

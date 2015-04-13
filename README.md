# textures

This repository a collection of files:

1. A (C) application that converts anything to a wave file
2. A script to call this application every time a file was changed
3. A blank image to work with and a GIMP image to play with

All this applications together are used to play textures. Normally I also use
GIMP and VLC / aplay.

TO COMPILE
==========

`cd src`
`make`


TO INSTALL
==========

`cp src/towav ~/bin`
`cp script/update.sh ~/bin`

TO PERFORM
==========

The score is like:
1. Open img/Textures.bmp in GIMP and edit it.
2. Export the BMP in GIMP using `Ctrl + E`
3. Use the update.sh script to monitor changes in Texture.bmp
4. Update.sh will use towav to convert Textures.bmp to Textures.bmp.wav
5. Use VLC in loop mode to keep Textures.bmp.wav playing forever. Yes, it can glitch...

Some instructions to perform:
* Open a first terminal and run `gimp Textures.bmp`
* Open a second terminal and run `./update.sh Textures.bmp`
* Open a third terminal and run `vlc --loop Textures.bmp.wav`or 
   `while [ true ] ; do aplay -q Textures.bmp.wav ; done`


No, have fun!

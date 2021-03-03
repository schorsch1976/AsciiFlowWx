Asciiflow Wx Editor
===================

Purpose:
========
Draw and edit any ascii art diagram.
It is mostly used for programming issues to document structure in an header or source as a comment.

Why a offline Ascii Flow Editor?
================================

As i got annoyed that [AsciiFlow Infinity](http://asciiflow.com/) or [asciiflow stable](http://stable.ascii-flow.appspot.com/#Draw) is just available when there is internet, i wrote my own QT based application.

[AsciiFlow Infinity](http://asciiflow.com/) now wants me to log in to google. Why should i log in to google to draw some ascii charts?

Features:
=========
- Export and Import ascii to/from the clipboard
- Export and Import ascii to/from a file
- Move parts of the ascii art
- draw rectangles
- resize rectangles
- draw class diagramms
- draw arrows
- draw lines
- draw freehand
- add text to any position
- undo and redo any change
- practicly unlimited space. The drawing area grows when you move out of it.

Licence
=======
GPL-3.0

Used Libraries:
===============
QT-5

Building:
=========

Installing requirements:
========================
- apt install libwxgtk3.0-gtk3-dev cmake g++

Building:
=========
- git clone https://github.com/schorsch1976/AsciiFlowWx.git
- cd AsciiFlowWx
- mkdir build
- cd build
- cmake ..
- make

Just start it by ./src/AsciiFlow

Differences compared to [AsciiFlowQt](https://github.com/schorsch1976/AsciiFlowQT.git)
====================================
- Text editing understands Backspace
- Under GTK3, the button can't remove a bitmap.

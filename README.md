<img width="1920" height="1080" alt="New Project" src="https://github.com/user-attachments/assets/ad5a4fef-d37f-4a9a-8baf-9c98dc3035f6" />

# This is a Work-In-Progress solo "ft_vox".
Despite its name, it doesn't follow 42's subject as I'm doing this solo, has ImGUI in it and player mechanics. <br>
See this more like a personal Minecraft clone in OpenGL than anything else.<br>
It also has the Nether and The End dimensions! <br>
But hey, if you're working on ft_vox and you're here to get help, you can check out my code and you might find what you're looking for! <br>
If you have any questions, need help with anything or just want to suggest things, **you can DM me on discord: eleowo**
# How to install [LINUX ONLY!]
First, make sure those libraries are installed on your system:<br>
```
sudo apt update
sudo apt install \
    build-essential cmake git \
    libgl1-mesa-dev \
    libglew-dev \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev
```
Clone this repo then run ``make`` at the root. The other static libraries will be downloaded. <br>
*Note: it is recommended to use the parameter -j with the amount of threads you wish to use (i.e. ``make -j8`` for 8-cores single-threaded CPUs) to speed up compilation.* <br>
Run the program with ``./player`` (you can also do ``make run`` to make and... run). <br>
# Controls
- ``F1``: Hide/Show crosshair and block overlay
- ``F3``: Show/Hide ImGUI with lots of useful informations and options *(Coords, Render Distance, Fog, current block to be placed, ...)*
- ``F11``: Toggle Fullscreen
- ``L``: Show/Hide your mouse cursor *(useful to move your mouse without moving your camera)*
- ``WASD/Arrow keys`` *(yes, I'm an arrow keys player on Minecraft)*: Move around
- ``Left-Shift/Right-Shift``: Sprint
- ``Space-bar/Right-CTRL``: Jump
- ``Left-click``: Break block
- ``Right-click``: Place block *(Only upwards for now)*
- ``Middle-click``: Pick block to be placed
- ``Scroll``: Change block to be placed
# Commands
In the F3 ImGUI menu, you'll see a command line. Here you can try these commands: <br>
- ``tp x y z`` (i.e. ``tp 0 90 0``): Teleports you to coordinates.
- ``setblock x y z`` type (i.e. ``setblock 0 90 0 stone``): Change the type of a block. Can also be applied to air.
- ``cw overworld/nether/the_end`` (i.e. ``cw nether``): Change the world you're in.<br>
*It's recommended to check the Spectator gamemode in the ImGUI menu if you don't want to fall out of the map when switching worlds!*
# Libraries used
- OpenGL
- STB
- GLM
- ImGUI
- GLEW
- GLFW
# Credits
- Libany, without her I wouldn't even have been to 42 School and who knows what my life would be right now!
- mbatty & mbirou for their OpenGL help 🤓☝️
- [Draconic Evolution](https://github.com/Draconic-Inc/Draconic-Evolution) for its End Chaos Island I reproduced using its Java code!
- You, because no one ever reads a readme until the end.

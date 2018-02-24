# beast.c

Simple clone in C (C99 with SDL) of the 1984 ASCII game [Beast](https://en.wikipedia.org/w/index.php?title=Beast_(video_game)) that I had fond memories of playing as kid. The original DOS game was written in IBM-PC Macro Assembly (MASM), primarily by Dan Baker. In [an email](https://en.wikipedia.org/wiki/Talk:Beast_(video_game)/Dan_Baker_Letters), he recounted some of the history:

> Alan Brown wrote a routine that would make sound, Derrick [Shadel] wrote a disk I/O routine for saving the high scores, and Mark [Hamilton] wrote a random number generator for me. I distributed Beast as shareware for a few years, and made some money (not much, maybe $100). After WordPerfect came out with the Shell (which allowed multiple programs to be loaded at the same time, but the user could only interact with one at time), I fixed Beast to be "Shell Compatible". WordPerfect bought Beast from me for about $1000 (which my wife and I quickly spent on a video camera), and Beast started shipping with Shell (or Library, what ever they called it).

## SDL Setup for OSX

This game relies on the SDL library. Normally this will be bundled with the game, but if you want to compile & run it from scratch, you'll need to create an SDL library and put it somewhere the compiler can find it:

```
mkdir ~/SDL 
cd ~/SDL 
curl -O http://www.libsdl.org/release/SDL2-2.0.7.zip
tar -xvjpf SDL2-2.0.7.zip
cd SDL2-2.0.7
./configure
make
make install
```

## Compiling (gcc) & Running

```
git clone https://github.com/prust/beast.c.git
cd beast.c
make && ./beast
```

## Other Ports

### Javascript (browser, HTML5 Canvas)

* [Beast on Chrome Store (free)](https://chrome.google.com/webstore/detail/beast/fhjaglelmpnoipibchkcdjiejlkodmcp) - This is the most complete rewrite, apparently by the [original author](https://github.com/danbaker)
* [prust/beast.js](https://github.com/prust/beast.js)
* [robertprovine/beast](https://github.com/robertprovine/beast)

### Javascript (console, node.js)

* [dominikwilkowski/beast.js](https://github.com/dominikwilkowski/beast.js)

### Lua

* [prust/beast-lua](https://github.com/prust/beast-lua).

### Python

* [houbbit/beast](https://github.com/houbbit/beast)

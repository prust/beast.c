# beast.c

Remake of the 1984 ASCII game [Beast](https://en.wikipedia.org/w/index.php?title=Beast_(video_game)) that I had fond memories of playing as kid. Here's an overview of the game [from mobygames](http://www.mobygames.com/game/dos/beast_):

> The hows and whys are apparently unimportant. What matters is this: you, represented by a double-pointing pair of ASCII glyphs, are trapped in a devious labyrinth. Your goal, however, is not to escape, but rather to survive: in the maze with you are a number of Beasts, looking like slim sinister Hs. If they find you, they will eat away one of your lives.

> Your weapons? None. However, nearly all of the walls in this maze can be pushed, like Sokoban crates... only indefinitely stackable. If you are able to line up wall segments and wait until a Beast enters into a gap between them, you should be able to close the gap on top of him, pushing the walls shut and making Beastly pulp.

Here is a screenshot of the original game:

![original Beast game](https://raw.githubusercontent.com/prust/beast.c/master/beast-orig-screenshot.jpg)

Here's a screenshot of this beast.c remake:

![beast.c remake](https://raw.githubusercontent.com/prust/beast.c/master/beast.c-screenshot.png)

# History

The original DOS game was written in IBM-PC Macro Assembly (MASM), primarily by Dan Baker. In [an email](https://en.wikipedia.org/wiki/Talk:Beast_(video_game)/Dan_Baker_Letters), he recounts some of the history:

> Alan Brown wrote a routine that would make sound, Derrick [Shadel] wrote a disk I/O routine for saving the high scores, and Mark [Hamilton] wrote a random number generator for me. I distributed Beast as shareware for a few years, and made some money (not much, maybe $100). After WordPerfect came out with the Shell (which allowed multiple programs to be loaded at the same time, but the user could only interact with one at time), I fixed Beast to be "Shell Compatible". WordPerfect bought Beast from me for about $1000 (which my wife and I quickly spent on a video camera), and Beast started shipping with Shell (or Library, what ever they called it).

# Installation

To play Beast:
* Download [beast-win-0.1.1.zip](https://github.com/prust/beast.c/releases/download/v0.1.1/beast-win-0.1.1.zip) on a Windows computer
* Unzip it
* Double-click on `beast.exe`.

## Language & Libraries

This remake was written in C (C99) with the SDL library.

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

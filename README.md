Todo:
1. game adjustments
2. nevermind fisheye still needs some work
3. mini map
4. form for submition asdfasdfasdf
5. draw a flowchart(sike)
6. i need to learn the uses of headers or cooked

To add a button:
add it to InitButtons() in interface.c

if you want to set the coords for the button:
use fractions of SCREEN_WIDTH and SCREEN_HEIGHT
for example, to set a button in the bottom left of the screen:
it would be in the first quarter of SCREEN_WIDTH and the third quarter of SCREEN_HEIGHT
so it would be located at:
SCREEN_WIDTH/4, 3 * SCREEN_HEIGHT/4

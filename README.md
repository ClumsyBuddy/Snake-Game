![ZJDxvd5vlI](https://user-images.githubusercontent.com/35859863/122295436-2e90fe00-cec7-11eb-8418-b67e0c748730.gif)
# Snake-Game

My take on the snake game, I went for a really wierd approach on how the snake works. The screen is a grid of rectangles from the sfml library and I change color of the rectangles
based on the interactions in the game. The player has a head position and then has a vector of vector2's for the tail and I simply remove the oldest tail section and add
a new one between the tail and the head to give the illusion of the tail following.
The apples spawn randomly within the bounds and the player class controls everything other the the rectangles themselves.

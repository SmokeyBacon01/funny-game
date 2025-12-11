# funny-game
A test game made with raylib to get use to game development

Hello yes, this project works now.
Run the executable with any command line argument to open in window mode (to view console).

KNOWN ISSUES:

Zoom interacts weirdly with change in time.
  I think it has something to do with dt not working properly. Maybe adding an extra field, adjusted dt (for game elements) and true dt (like global counter)

TODO:

Implement slow-down after enemy kill

refactor hitboxes, or use an index system to link the enemy struct to its hitbox.

Add enemies

Add a menu, game over, and pause menu

Add a score counter or something

Implement enemy spawning.

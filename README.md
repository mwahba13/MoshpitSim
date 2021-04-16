# MoshpitSim
This is a term project for CPSC 565 (Emergent Computing) completed at the University of Calgary.


You can read my full project write-up here.


##Instructions for downloading source code
1. Download or Clone Repo
2. Open Unreal Engine Project File


##Instructions for downloading build
You can download a build from the "releases" section of this repo.

##Main Menu
Upon starting the simulation you will get to adjust some of the parameters of the simulation through the Main Menu.


###Settings
![MainMenu](Images/MainMenu.png)

* **Num of Agents:** How many agents you wish to start the simulation with.
* **Agent Neighborhood Radius:** How big each agent's neighborhood is.
* **Select Song:** From this dropdown you can select the song that will play during the simulation. Songs can be added (see section below)
* **Aggression:** Affects how likely an agent is to start thrashing.
* **Music Feel:** Affects how much the music intensity affects the agent's state.
* **Group Think:** Affects how likely an agent's state change is affected by its neighbors.



##In-Game Debug Menu
During the simulation you can press the 'J' key to bring up the debug menu.

##Adding your own songs
To add your own songs all you need to do is import a WAV file into the Unreal Engine project and add the song to the "Song List" found 
in the GameManager Blueprint. Your song will now show up in the main menu's drop down menu.

![songselect](Images/SongSelect.png)
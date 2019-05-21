# Smart Shoe Repo

![Discord Channel](https://avatars3.githubusercontent.com/u/1965106?s=16&v=4) [Help Channel](https://discord.gg/S9uT74G)

#### A repo for your smart shoe

### Installing

  Start by cloning the repo to your disk

  ```
  git clone https://github.com/Slothkiller1234/smartShoeRepo.git
  ```
  
  After the repo has been cloned, go to the location at which it was cloned.
  
    *Note, if your computer doesn't support 'git' you may ethier download a git client, or the .zip provided.
  
  After you have cloned the repo, move the folder provided to the arduino/libraries folder.
  
	*Note, I am using arduino 1.8.9

### Using the repo
	
   In order to use the repo, you must first include it in your arduino sketch.
	
	
	#include "smartShoe.h"
	
   After you have included the repo, you may look through smartShoe.cpp for the given functions. Remember you can always create your own.
   
   Some example functions however are:
	smart_shoe.say()
	smart_shoe_pin()
	etc...
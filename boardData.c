#include "boardData.h"

const char *boardSpaces[MAX_BOARD_SPACES] =
        {
                "none:0:GO",
                "brown:1:Burger Joint",
                "brown:1:Pizza House",
                "none:0:Chance",
                "ltblue:1:Candy Store",
                "ltblue:1:Ice Cream Parlor",
                "none:0:Jail",
                "pink:2:Museum",
                "pink:2:Library",
                "none:0:Chance",
                "orange:2:Skate Park",
                "orange:2:Swimming Pool",
                "none:0:Free Parking",
                "red:3:Video Game Arcade",
                "red:3:Movie Theater",
                "none:0:Chance",
                "yellow:3:Toy Store",
                "yellow:3:Pet Store",
                "none:0:Go to Jail",
                "green:4:Bowling Alley",
                "green:4:The Zoo",
                "none:0:Chance",
                "blue:5:Park Place",
                "blue:5:Boardwalk"
        };

const char *chanceCards[MAX_CARDS] =
        {
                "x::Get out of Jail FREE",
                "m:free:Move to next unowned property, Get it for free",
                "m:free:Move to next unowned property, Get it for free",
                "m:free:Move to next unowned property, Get it for free",
                "m:free:Move to next unowned property, Get it for free",
                "+:5:Move forward five spaces",
                "+:1:Move forward one space",
                "g:0:Advance to GO, collect $2",
                "$:-2:You ate too many sweets! Pay $2",
                "$:1:It's your birthday! Everyone pays you $1",
                "$:2:You did all your homework! Collect $2",
                "g:23:Advance to Boardwalk, Get it for free or pay rent",
                "g:10:Advance to Skate Park, Get it for free or pay rent",
                "m:red:Move to red space, Get it for free or pay rent",
                "m:ltblue:Move to light blue space, Get it for free or pay rent",
                "m:orange:Move to orange space, Get it for free or pay rent",
                "m:green:Move to green space, Get it for free or pay rent",
                "m:pink:Move to pink space, Get it for free or pay rent",
                "m:blue:Move to dark blue space, Get it for free or pay rent",
                "m:yellow:Move to yellow space, Get it for free or pay rent"
        };

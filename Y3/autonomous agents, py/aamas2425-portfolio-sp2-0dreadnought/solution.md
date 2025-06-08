# How to run

To run Release 1.0, Release 2.0 or Release 2.1 you need to:
    - Install required libraries by running "pip install -r requirements.txt"
    - Run scripts runNormal.py, runExtended.py or runNovel.py respectively

# Legend for Visualization

## Normal

Brown squares are ground paths, Blue squares represent river and White square represents the base.

Ripe bushes are shown as red squares, they will turn green, when are picked.

Scout agents are gray squares, they will light orange while theyre over ripe bush
and will light pink when they have to return to base.

Picker agents are yellow sqares, that will turn lime when they have to return to base

## Extended

Bushes will now light red again, once they are ripe

Pickers are still yellow squares, that is while they don't have a target location.
When they get one, they will turn purple and after picking up the berry
from the target location they will turn yellow again. 
Once they have to return to base, they will light lime.

Rest is unchanged

## Novel

Pickers will now be black squares. Once they recieve target location they will turn purple.
They will turn black, once they have picked up the berry.

Maintainers are yellow squares that will turn pink while theyre busy and will turn lime
to show that they have to return to base

Rest is unchanged

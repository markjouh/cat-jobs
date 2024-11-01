# Engine
This is where the battle simulations happen. I'm just keeping track of some ideas here.

## Unit Instances
We can observe that when there's multiple of the same cat on the field, most of their stats are shared and immutable - all the info about their attack, abilities, etc.

The only thing that's unique between different instances of a unit are its position, health, and the state of the animation it's on.
Everything else can be handled by storing a small piece of data linking it to some object that handles all the instance-independent data, which we'll call a unit class.

## Unit Classes
In a battle, there are only 10 distinct cat classes, and around the same order of magnitude for enemies. Modifiers like combos or magnifications don't change this.

Since the properties of these classes don't change, we only need to prepare them once before the start of the battle.
Further, since there are also fairly few cat-enemy pairs, we can precompute a lot of things we'd normally need to calculate every time they interact.
Things like attack/defense multipliers, chance of ability proc, and things like that can be encoded into small vectors.

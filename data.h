#ifndef DATA_H
#define DATA_H

/*
Sources:
https://github.com/battlecatsultimate/BCU_java_util_common/blob/1ed5b8d261cd3725b2d69eb4cfb691c9fa31c737/util/Data.java

Notes:
BCU lists the default knockback time as 11, which conflicts with the wiki's 12.
It's not clear to me whether we're supposed to count the activation frame, which
is probably the reason for this difference.
*/

// Knockbacks from going under an HP cutoff
const int KB_DIST = 165, KB_DURATION = 12;
// Knockbacks from ability procs and sniper the cat (small)
const int PROC_KB_DIST = 345, PROC_KB_DURATION = 12;
// Knockbacks from boss shockwaves (large)
const int BOSS_KB_DIST = 705, BOSS_KB_DURATION = 47;

#endif
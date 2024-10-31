# Data
This stuff prepares cat and enemy data so that the engine only needs to deal with a few neat, constexpr headers.  
The data goes into these jsons as an intermediate step because:
- It's nice as a sanity check, since the generated code won't be geared towards readability
- I don't want to have to mess with the parser when changing the scheme I'm using to store unit data
- Getting all this data in a nice, computer friendly format was a big pain point when starting this project, so maybe sharing it here in a project & language agnostic format could be helpful for other future BC projects(?)

Unlike the engine, where I generally try my best to implement everything in the most beautiful and efficient way I can, this stuff is more... I just need it to do the job. Especially for the parser, which I really just wanted to get out of the way.

Sources:
- https://battlecats-db.com/unit/status_r_all.html
- https://battlecats-db.com/enemy/status_atr_all.html
- https://github.com/battlecatsultimate/bcu-resources/tree/master/resources/lang/en

Known Issues:
- Meditation Cat's immunity to boss SW isn't listed on the battlecats-db table
- Missing metal killer

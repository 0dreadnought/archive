# Activity 2

## Moving agents

In this activity you need to:

1. Create a new subfolder of `src` called `moving-agents` 
2. In that folder create the required files to build a model that:
   * Defines two 2 type of agents, `Horizontals` that are able to move from left to right or right to left in a 2D space and `Verticals` able to move up and down in the space. Try to use a different color for every type of agent.
   * The model should create a random number of `Horizontals` and `Verticals` and show them moving along the 2D space in the MESA web interface.
   * Define a rule to tackle potential collisions between agents, for example, `Vertical` agents always have priority over `Horizontal` ones
   * Suggest and implement a different rule for handling collisions. 

3. Check the MESA docs, especially the `move_agent` function. 
4. Analyse the different outputs of the model and the behaviour of the key methods e.g. step.
5. commit and push the resulting source code.
6. Still time? check other examples available in the MESA docs.

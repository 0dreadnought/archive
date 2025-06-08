[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/DDPOF3-W)
# AA-MAS - Worksheet Week 4 

**Date**: 24/Oct/24
 
**Topic**: Multiagent System Analysis and Agent Interactions

**Objective**: 

* To practice the analysis of agent models at system level.
* To use basic game theory concepts to study interactions among agents at system level.

## Overview

We have seen how to implement simple agent architectures and decision models. Now we are going to focus in obtaining data from the simulation to analyse the behaviour of the agents at system level. 

## **ACTIVITIES**

### **Activity 1: Analysis of the Warehouse Multiagent System**

For this activity you need to read, understand the code and run [this notebook](t1-warehouse-analysis.ipynb).

### **Activity 2: Further Warehouse MAS analysis**

Create a Jupyter notebook that enables you to answer the following questions:

* Q1: How many robots remain FREE and BUSY at every time step?
* Q2: What is the distribution of occupancy of the robots during the simulation? i.e. How many robots remain busy for how long?
* Q3: Try changing the number of robots and seed and see how the distribution changes.

*Solution Ideas (Feel free to try your own approach too!)*

* Q1
  * We need to add a new data collector field in `warehouse\model.py` at the end of the `__init__` method, with the robots that are FREE and BUSY. 
* Q2
  * We need to get a `dataframe` with the **type** of agents. We have `PickerRobot` and `Box`. We need to add the **type** of agent in the *data collector* configuration see `warehouse\model.py` at the end of the `__init__` method.

  * Once we have the **type** we can filter the resulting *dataframe* to create a *dataframe* with robots only. 

  * We can get the sum of steps when each robot has been busy. As our state `FREE = 1` and `BUSY=0` if we sum all the steps with state `FREE` we get all the steps the robots were free, so we just need to inverse that value to get times they were busy. Alternatively you can try to change FREE to 0 and BUSY to 1 in the `src\warehouse\agents.py` to see if you can avoid inverting the data after collected (Not tested).

  * Once we have the `not_free` values for each robot, we can sum these values and that will give us the No. of steps the agents were busy.


### **Activity 3: Using Game Theory to analyse agent interactions**

* In this activity you are required to read, understand and explore (by changing parameters) the `mesa`'s Demographic Prisoner Dilemma available [here](t3-prisoners-dilemma-analysis.ipynb).


### **Activity 4: Virtual Robot Battles (VRB)**

[BBC Robot Wars](https://youtu.be/abrqn4nPw9k?si=ENXNnUumnSgS0FeJ&t=49) was a popular show where engineers built robots to compete in tenacious battles. In this activity you will build your own virtual arena: a VRB Game, where virtual robots (vRobots) will compete. 

The expected behaviour of the VRB Game is as follows:

  * There are 100 vRobots that will compete
  * In these battles there is no need for contact, at start of each time step, vRobots are paired randomly to fight.
  * Instead of the furious battles the robots fought in *Robot Wars*, the vRobots will play a friendly Rock, Paper, Scissors game to determine who wins. You need to keep track of the **outcome** at every time step.
  * By default, vRobots will play a **pure strategy**.  
  * Assume VRB is a **perfect information** game.
  * In VRB, a *follower* is a vRobot that switches its strategy to the most successful action.
  * At every time step, every vRobot has a given probability of becoming a *follower*

---

1. Identify the key elements of the VRB Game 
2. Plot the behaviour of the **outcomes** during 100 time steps.
3. Compare the behaviour of the **outcomes** with different probabilities of becoming a *follower*. What can you observe?

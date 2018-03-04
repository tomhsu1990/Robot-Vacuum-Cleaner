# Robot Vacuum Cleaner

Suppose you have a robot vacuum cleaner. After placing it on the floor, could it sweep the entire floor without map information? In other words, just by bumping the wall, could the robot accomplish the task?

Here is the simulation video: [Robot Vacuum Cleaner Simulation](https://youtu.be/AFBVZCKAXzI)

## Idea
The idea is simple. Just view the problem as solving a maze problem. We have to maintain going along with the wall and that left hand touches the wall. After sweeping a position, treat the position as a kind of virtual wall for termination condition.

## Algorithm
1. Turn the direction to head to North.

2. Move forward until touch a wall.

    a. Assume robot's back is an empty position and record it as a possible empty position pos. For instance, if the robot's position is (x, y) and direction is North, the recorded position is (x+1, y).
    
    b. Turn right.
    
    c. Move forward. If the robot does not touch the wall, go to step (d). Otherwise, repeat step (b) and (c). Once, it makes turns 4 times, go to step (d).
    
    d. Turn left.
    
    e. Move forward. If the robot touches a wall, go to step 3. Otherwise, repeat step (d) and (e).
    
3. Sort all possible empty positions pos based on current position.

4. Pick a valid position from pos where it is not cleaned, not out of boundary and not a wall.

5. If there is no such valid position, in other words, the floor is fully sweeped.

6. If we can find a valid position, do a bfs to move current position to the valid position pos. The movements are on the recorded cleaned positions. Then, go to the first step.

* When it is a valid Move, we will record the position as a cleaned.

## Discussion
The behavior would look like circling the environment and the circle becomes smaller and smaller. However, it might produces several isolated circles. This way, we perform step 3-6 to visit each possible isolated circles.

There is a trivial way to accomplish this task by dfs with backtracking. However, it might use more steps and turns!

## How to run: (MacOS)
g++ -std=c++11 Cleaner.cpp

./a.out ./map/map.txt

If you want to see a simple animation,

./a.out ./map/map.txt 1

I haven't tested in Linux and Windows, but I guess the code will work fine under Linux.

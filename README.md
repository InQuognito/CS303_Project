# CS303 Project
**Project:** 1A (Infix Calculator)

**Team Members:** Patrick Cotter

### Assumptions // Known Issues
- The infix operation of nested unary operators (ex. ++++2) does not work. This was the one part of the project that stumped me.

### UML Diagram
I wasn't too sure what to do for this one, since there aren't any class relationships to speak of. I decided to go with a workflow instead, please let me know if this is incorrect.
![image](https://github.com/InQuognito/CS303_Project/assets/44120299/db5b6bad-b933-4b87-881b-3baf5f77805b)

### Efficiency
The algorithm has an overall time complexity O(n), where n is the length of the input. Because the program iterates through each character to parse it, it will need to execute at least this many times. This time complexity is consistent through all significant functions.

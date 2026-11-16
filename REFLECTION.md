1. For every function you wrote (TODO 1 to 19), explain in your own words what it does and
   give its time complexity in terms of n, the number of scenes. One short paragraph each.
TODOS 
   1. O(1)
      Constructs the scene object, specifically it initializes the choices array with the current number of indexes for that scene. 
   2. O(n)
      Destructs and free's the scene and everything it owns. We are O(n) because the delete method is effectively recursive as deleting a node deletes it's children who's delete method deletes it's children and so on. 
   3. O(1)
       We know it's an ending if there's no choices out of it so we just compare to numChoices. 
   4. O(1)
       Makes sure the index given is valid for this node's number of choices, then return choice if it is or nullptr if it isn't.
   5. O(1)
       Set a scene's choices slot at the given index to the new scene next. 
   6. O(n)
        We first check if our current scene = nullptr and return 0 because there's no scene to count here. Then since we know were on a valid node, we return 1 + the recursive call on each of the 3 children. Since getChoice returns nullptr for invalid indexes, they are caught by our base case so we don't have to check if it's a valid choice index before calling the recursion. O(n) since we traverse the entire tree once. 
   7. O(n)
        Very, very similar functionality to countScenes, however instead of always returning 1 + recursive calls on the children, we return 1 if our current scene is an ending and ONLY return the recursive calls for non endings since they shouldn't add to the count. O(n) since we traverse the entire tree once.
   8. O(n)
      We first check if our current scene = nullptr and return -1 so that empty stories have a height of -1 and to ensure when were outside the tree, we don't count height. Then we use a temp variable to compare and save the largest height of all the children by recursively calling height on each of the children. Finally, we return our maximum height found + 1 to account for the height of the current node as well. O(n) because we iterate through ALL the scenes that are descendants of the current scene. 
   9. O(n)
        This is nearly identical to TODO 2, its effectivity a recursive call of delete on each scene, moving down through the tree deleting everything on its way. We have to set root_ to nullptr after to ensure our pointer isn't dangling and pointing to freed memory. This leaves the tree "empty" as the instructions say to
   10. O(N)
      Load reads the story file and creates the scenes with readScene. it is O(N) because it creates every scne once. Load handles bad input and file read errors as well as sets starting hp. 
   11. O(1)
        readScene grabs each of the fields form the text file and creates a new moment and scene with them assuming they are valid. O(1) because it only makes one scene at a time.
   12. O(1)
        asks for player input. O(1) because it's time is not dependent on the number of scnes.
   13. O(N)
        walks the single path chosen by the player. Printing all the values and scene info as its updated. asks for player input and handles end conditions. O(N) becuase the player coudl pick the longest possible path. 
   14. O(N)
        makes 3 full traversals of the tree, printing our helper methods for countScenes. CountEndings and height. 3n is still o(n)
   15. O(N)
        O(N) because it makes one call to printEndings which is O(N). 
   16. O(N)
        O(n) same time complexity as countScenes, but it only increments if its an ending not all scnes. 
   17. O(N)
        O(N) is worst case, the bestloot path could be the longest path as well so all scenes have to get checked. We just compare the highest loot of our next posible choice and chose that one. tie breakers go to the lowest numbered chocie.
   18. O(N)
        O(N) because we have to check all the nodes to find the best path. BestLoot iteates through the tree getting back loot from possilbe children choiced and then builds the correct path and total loot on the winddown stage. 
   19. O(N)
        calls bestLoot once on root to use the entire tree as bestLoot's subtree for searching. O(N) beacause bestLoot checks all the scnes once. 


2. Why does each scene allocate exactly numChoices slots instead of declaring Scene<T>*
   choices[3]? What does an ending hold in choices_, and why?
We allocate only the needed number of slots to avoid allocating memory we don't need. If we hard coded choices[3] then an ending will allocate an array of size 3 even though it isn't used or needed at all. An ending holds nullptr in choices_ because we don't need to allocate any memory for an empty array so we can leave it as nullptr and could just compare it to nullptr when we want to detect an ending. I chose to use numChoices instead but either would work

3. Walk through your Scene destructor. What gets deleted, and in what order? What would
   go wrong if choices_ were deleted first?
First we iterate though the array, freeing all the choices (children) of the current scene. This effectively causes recursion as their delete methods also call it on their children and so on. We then free the array that stores the choices after we know it's contents are freed. delete[] frees the array and pointers but does not free the objects stored at the pointers which is why we iterate though them first. We then delete data_ to clean up our last owned memory for the scene. If we deleted choices_ first, then we wouldn't be able to free the objects at the end of the pointers, leading to a memory leak. If we tried to we would get an error as we try to free the destination of a freed pointer.

4. Show the path greedy took and the path optimal took on your story, with the loot for each.
   Explain why greedy lost. If greedy did not lose, change your story until it does, and explain
   what you changed.
   Greedy advisor
     Path: 1-1
     Loot: 5
     Result: died
   Optimal advisor
     Path: 2-2-1-1-2
     Loot: 12
     Result: reached an ending
Greedy snoozes gaining 2 loot over the wake-up of 0 loot. greedy then chooses to turn off the alarm for 3 loot over snooze again of 1 loot. This path kills you and since greedy dosent care about hp or look more than one step ahead, it blindly walked down a bad path. I increased the loot to snoozing the alarm since more sleep is valuable, origianlly it was 0 loot and greedy only picked it because of the ordeirng of choices. 

5. bestLoot tries every path, yet it runs in O(n) time. Explain why it is not exponential, using
   what you know about paths and leaves in a tree.
Since we iterate through the tree child by child with recursion, we only check each node once, combining values from later calls to build out path up the tree. We don't have to physcially walk each possible path every time because the iteration effectively lets each unique path share the calculation of each node. 

6. Which List methods does your game use, and why? What does one search cost as the bag
   grows?
I used search to detrech exisitng items in the inventory. I used addFront to add new items to the inventory. I used size in the print to say how many items are in the inventory. I used print to print the contents of the inventory. Search is linear growth, reliant on the number of items, if n is num items were O(n) is n is scnes were o(1)


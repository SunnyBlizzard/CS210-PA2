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
   10. O()
   11. O()
   12. O()
   13. O()
   14. O()
   15. O()
   16. O()
   17. O()
   18. O()
   19. O()


2. Why does each scene allocate exactly numChoices slots instead of declaring Scene<T>*
   choices[3]? What does an ending hold in choices_, and why?
We allocate only the needed number of slots to avoid allocating memory we don't need. If we hard coded choices[3] then an ending will allocate an array of size 3 even though it isn't used or needed at all. An ending holds nullptr in choices_ because we don't need to allocate any memory for an empty array so we can leave it as nullptr and could just compare it to nullptr when we want to detect an ending. I chose to use numChoices instead but either would work

3. Walk through your Scene destructor. What gets deleted, and in what order? What would
   go wrong if choices_ were deleted first?
First we iterate though the array, freeing all the choices (children) of the current scene. This effectively causes recursion as their delete methods also call it on their children and so on. We then free the array that stores the choices after we know it's contents are freed. delete[] frees the array and pointers but does not free the objects stored at the pointers which is why we iterate though them first. We then delete data_ to clean up our last owned memory for the scene. If we deleted choices_ first, then we wouldn't be able to free the objects at the end of the pointers, leading to a memory leak. If we tried to we would get an error as we try to free the destination of a freed pointer.

4. Show the path greedy took and the path optimal took on your story, with the loot for each.
   Explain why greedy lost. If greedy did not lose, change your story until it does, and explain
   what you changed.


5. bestLoot tries every path, yet it runs in O(n) time. Explain why it is not exponential, using
   what you know about paths and leaves in a tree.


6. Which List methods does your game use, and why? What does one search cost as the bag
   grows?



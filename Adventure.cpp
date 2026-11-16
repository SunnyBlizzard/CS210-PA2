#include "Adventure.h"
#include <iostream>

// ===================== GIVEN HELPERS (do not change) =====================

// Reads the next line that is not blank.
// Spaces at the start of the line are removed.
// Returns false when the file has no more lines.
bool Adventure::nextLine(std::ifstream& in, std::string& line) {
    std::string raw;
    while (std::getline(in, raw)) {
        int start = 0;
        while (start < (int)raw.size() && (raw[start] == ' ' || raw[start] == '\t')) {
            start++;
        }
        line = raw.substr(start);
        // Files saved on Windows can end each line with an extra '\r'.
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
        if (!line.empty()) {
            return true;
        }
    }
    return false;
}

// Reads the next line and checks that it starts with keyword.
// Example: for the line "HP -2" and keyword "HP", value becomes "-2".
// Prints an error and returns false if the line is missing or wrong.
bool Adventure::readField(std::ifstream& in, const std::string& keyword, std::string& value) {
    std::string line;
    if (!nextLine(in, line)) {
        std::cout << "Error: expected a " << keyword << " line but the file ended." << std::endl;
        return false;
    }
    std::string prefix = keyword + " ";
    if (line.substr(0, prefix.size()) != prefix) {
        std::cout << "Error: expected a " << keyword << " line but found: " << line << std::endl;
        return false;
    }
    value = line.substr(prefix.size());
    return true;
}

// Adds one choice number to a path.
// extendPath("", 2) gives "2". extendPath("2-1", 3) gives "2-1-3".
std::string Adventure::extendPath(const std::string& path, int choiceNumber) const {
    if (path.empty()) {
        return std::to_string(choiceNumber);
    }
    return path + "-" + std::to_string(choiceNumber);
}

// ===================== LOADING =====================

// TODO 10: open the story file and build the tree.                                -----------DONE-----------
//   - If the file will not open, print exactly:
//       Error: could not open file <fileName>
//     and return false.
//   - Read the START_HP line and save the number in startHp_.
//     (std::stoi turns "10" into 10.)
//   - Build the whole story with readScene. The first scene's label is "START".
//   - If anything failed, return false. Otherwise give the story to tree_.       -----------DONE-----------
bool Adventure::load(const std::string& fileName) {
    std::ifstream file_in(fileName);
    if (!file_in.is_open()) { //check file is open
        std::cout << "Error: could not open file " << fileName << std::endl;
        return false;
    }
    std::string value;
    if (readField(file_in, "START_HP", value)) { //make sure we got what we wanted
        startHp_ = std::stoi(value);
    }
    else {
        return false;
    }
    Scene<Moment>* tempPtr = readScene(file_in, "START"); //dont want to run readScene twice so we store it temporarily for our comparison
    if (tempPtr != nullptr) { //make sure we got what we wanted
        tree_.setRoot(tempPtr);
    }
    else {
        return false;
    }
    return true;
}

// TODO 11: read ONE scene and everything below it. Return it.                  -----------DONE-----------
//   - Read SCENE, HP, LOOT, ITEM, and CHOICES with readField, in that order.
//   - If CHOICES is below 0 or above Scene<Moment>::MAX_CHOICES, print exactly:
//       Error: scene "<scene text>" has <n> choices. The limit is 3.
//     and return nullptr.
//   - Each choice is a LABEL line followed by a whole scene.
//   - If anything goes wrong at any depth, return nullptr
//     and make sure nothing you already built is leaked.                       -----------DONE-----------
Scene<Moment>* Adventure::readScene(std::ifstream& in, const std::string& label) {
    //Scene Text
    std::string scene_value;
    if (!readField(in, "SCENE", scene_value)) { //make sure we got what we wanted
        return nullptr;
    }

    //HP + or -
    std::string temp_value;
    if (!readField(in, "HP", temp_value)) { //make sure we got what we wanted
        return nullptr;
    }
    int scene_hp = std::stoi(temp_value);

    //LOOT
    if (!readField(in, "LOOT", temp_value)) { //make sure we got what we wanted
        return nullptr;
    }
    int scene_loot = std::stoi(temp_value);

    //Items
    std::string scene_item;
    if (!readField(in, "ITEM", scene_item)) { //make sure we got what we wanted
        return nullptr;
    }

    //Number of Choices
    if (!readField(in, "CHOICES", temp_value)) { //make sure we got what we wanted
        return nullptr;
    }
    int scene_choices = std::stoi(temp_value);
    if (scene_choices < 0 || scene_choices > Scene<Moment>::MAX_CHOICES) {
        std::cout << "Error: scene \"" << scene_value << "\" has " << scene_choices << " choices. The limit is "
                        << Scene<Moment>::MAX_CHOICES << "." << std::endl;
        return nullptr;
    }

    Moment* scene_moment = new Moment(scene_value, label, scene_hp, scene_loot, scene_item);
    Scene<Moment>* new_scene = new Scene<Moment>(scene_moment, scene_choices);

    std::string temp_label;
    for (int i = 0; i < scene_choices; i++) {
        if (!readField(in, "LABEL", temp_label)) { //make sure we got what we wanted
            delete new_scene; //also deletes the moment since it owns the moment
            return nullptr;
        }
        else {
            Scene<Moment>* new_child = readScene(in, temp_label);
            if (new_child != nullptr) {
                new_scene->setChoice(i, new_child);
            }
            else {
                delete new_scene; //also deletes the moment since it owns the moment
                return nullptr;
            }
        }
    }
    return new_scene;
}

// ===================== PLAYING =====================

// TODO 12: ask the player to pick a choice from 1 to numChoices.              -----------DONE-----------
//   - Print "Your choice: " (no newline) and read a whole line.
//   - If the line is not a valid number in range, print exactly:
//       Please type a number from 1 to <numChoices>.
//     and ask again.
//   - If the input ends (getline fails), return 0.                           -----------DONE-----------
int Adventure::askChoice(int numChoices) {
    int choice = 1;
    while (true){
        std::cout << "Your choice: ";
        //get input
        std::string line;
        if (!std::getline(std::cin, line)) { //input fail
            return 0;
        }
        if (line.empty() || line.length() > 1) { //empty or more than one char
            std::cout << "Please type a number from 1 to " <<numChoices << "." << std::endl;
            continue;
        }
        if (line[0] >= '0' && line[0] <= '9') { //digit check
            choice = std::stoi(line);
        }
        else {
            std::cout << "Please type a number from 1 to " <<numChoices << "." << std::endl;
            continue;
        }

        if (choice <= 0 || choice > numChoices) { //digit in the correct range
            std::cout << "Please type a number from 1 to " <<numChoices << "." << std::endl;
            continue;
        }
        else {
            break;
        }
    }
    return choice;
}

// TODO 13: play the story once from the start.                                   -----------DONE-----------
// Every time the player ENTERS a scene, in this order:
//   1. Add its HP change to hp and its loot to loot.
//   2. Print a line of 40 dashes, then the scene text.
//   3. If the scene has an item:
//        if the bag already has it, print "You already have: <item>"
//        otherwise add it to the bag and print "You picked up: <item>"
//   4. Print "HP: <hp>  Loot: <loot>" (two spaces before Loot).
//   5. If hp is 0 or less, print "Your HP hit 0. GAME OVER." and stop.
//      If the scene is an ending, print "THE END." and stop.
//   6. Print each choice as "[1] <label>" and use askChoice.
//      If askChoice returns 0, leave play right away.
// When the game stops, print:
//   Final HP: <hp>  Final loot: <loot>
//   Inventory (<count> items):
// and then print the bag.
// The bag is a List<Item>. Start with HP = startHp_ and loot = 0.                -----------DONE-----------
void Adventure::play() {
    int hp = startHp_;
    int loot = 0;
    List<Item> inventory;
    Scene<Moment>* current_scene = tree_.getRoot();
    Moment* current_moment;

    while (current_scene != nullptr) {
        current_moment = current_scene->getData();
        hp += current_moment->getHpChange();
        loot += current_moment->getLoot();
        std::cout << "----------------------------------------" << std::endl;
        std::cout << current_moment->getText() << std::endl;

        if (current_moment->hasItem()) {
            if (inventory.search(current_moment->getItem())) {
                std::cout << "You already have: " << current_moment->getItem() << std::endl;
            }
            else {
                Item* new_item = new Item(current_moment->getItem());
                inventory.addFront(new_item);
                std::cout << "You picked up: " << current_moment->getItem() << std::endl;
            }
        }
        std::cout << "HP: " << hp << "  Loot: " << loot << std::endl;
        if (hp <= 0) {
            std::cout << "Your HP hit 0. GAME OVER." << std::endl;
            break;
        }
        if (current_scene->isEnding()) {
            std::cout << "THE END." << std::endl;
            break;
        }
        else {
            for (int i = 0; i < current_scene->getNumChoices(); i++) {
                std::cout << "[" << (i+1) << "] " << current_scene->getChoice(i)->getData()->getLabel() << std::endl; //plus 1 because players choices are 1-3 but the array is 0 indexed
            }
            int choice = askChoice(current_scene->getNumChoices());
            if (choice == 0) {
                return;
            }
            else {
                current_scene = current_scene->getChoice(choice-1); //minus 1 because players choices are 1-3 but the array is 0 indexed
            }
        }
    }
    std::cout << "Final HP: " << hp << "  Final loot: " << loot << std::endl;
    std::cout << "Inventory (" << inventory.size() << " items):" << std::endl;
    inventory.print();
}

// ===================== ANALYZING =====================

// TODO 14: print exactly three lines:                                         -----------DONE-----------
//   Scenes: <count>
//   Endings: <count>
//   Longest playthrough: <height> choices                                     -----------DONE-----------
void Adventure::printStats() const {
    std::cout << "Scenes: " << tree_.countScenes() << std::endl;
    std::cout << "Endings: " << tree_.countEndings() << std::endl;
    std::cout << "Longest playthrough: " << tree_.height() << " choices" << std::endl;
}

// TODO 15: print every ending in the story, one per line.                        -----------DONE-----------
void Adventure::printSpoilers() const {
    printEndings(tree_.getRoot(), "");
}

// TODO 16: print every ending below scene.                                     -----------DONE-----------
// path holds the choice numbers taken to reach scene, like "2-1".
// Each ending prints as:
//   Path <path>: <scene text>
// If the very first scene is an ending, print "Path (start): <scene text>".
// Endings print in order: choice 1 before choice 2 before choice 3.
void Adventure::printEndings(Scene<Moment>* scene, const std::string& path) const {
    if (scene == nullptr) { //empty or off trere
        return;
    }
    if (scene->isEnding()) { //target
        if (path.length() == 0) { //new path
            std::cout << "Path (start): " << scene->getData()->getText() << std::endl;
        }
        else {
            std::cout << "Path " << path << ": " << scene->getData()->getText() << std::endl;
        }
    }
    for (int i = 0; i < scene->getNumChoices(); i++) { // this will print in the correct order
        printEndings(scene->getChoice(i), extendPath(path, i + 1)); //+1 because choices are 0 indexed and player input is not
    }
}

// TODO 17: the greedy advisor.
// Start at the first scene with startHp_ and 0 loot.
// Enter scenes the same way play does (add HP and loot).
// Stop when hp is 0 or less, or you reach an ending.
// Otherwise always move to the choice with the MOST loot right now.
// On a tie, pick the lower choice number.
// Print:
//   Greedy advisor
//     Path: <path>
//     Loot: <loot>
//     Result: died               (or "Result: reached an ending")
void Adventure::greedyRun() const {
    int hp = startHp_;
    int loot = 0;
    Scene<Moment>* current_scene = tree_.getRoot();
    Moment* current_moment;
    std::string path;
    std::string end_condition;

    while (current_scene != nullptr) {
        current_moment = current_scene->getData();
        hp += current_moment->getHpChange();
        loot += current_moment->getLoot();

        if (hp <= 0) { //check for death
            end_condition = "died";
            break;
        }
        if (current_scene->isEnding()) { //check for ending
            end_condition = "reached an ending";
            break;
        }
        int max_loot = -1;
        int max_choice = -1;
        for (int i = 0; i < current_scene->getNumChoices(); i++) { //find best next choice and save it
            if (current_scene->getChoice(i)->getData()->getLoot() > max_loot) {
                max_loot = current_scene->getChoice(i)->getData()->getLoot();
                max_choice = i;
            }
        }
        path = extendPath(path, max_choice + 1); //+1 to because player choices are 1 indexed
        current_scene = current_scene->getChoice(max_choice); //no +1 cause its not player formatted
    }
    std::cout << "Greedy advisor" << std::endl;
    std::cout << "  Path: " << path << std::endl;
    std::cout << "  Loot: " << loot << std::endl;
    std::cout << "  Result: " << end_condition << std::endl;
}

// TODO 18: the best total loot you can collect from scene down to an ending,
// arriving at scene with hp health and having taken path to get here.
//   - Return -1 if every way down from here kills you.
//   - Put the path of the best run into bestPath.
//   - Must be recursive and look at every choice.
int Adventure::bestLoot(Scene<Moment>* scene, int hp, const std::string& path, std::string& bestPath) const {
    hp += scene->getData()->getHpChange();
    if (hp <= 0) { //check death
        return -1; //-1 ensures its never accepted as best
    }
    if (scene->isEnding()) { //reached end, current best path is the best
        bestPath = path;
        return scene->getData()->getLoot(); //return current loot
    }

    int best_loot = -1; //again ensures its overridden by any amount of loot that's found
    for (int i = 0; i < scene->getNumChoices(); i++) { //check all choices
        std::string temp_path;
        int temp_loot = bestLoot(scene->getChoice(i), hp, extendPath(path, i + 1), temp_path);//check childs bestLoot, +1 because of player choice 1 indexing
        if (temp_loot > best_loot) { //better loot path found
            best_loot = temp_loot;
            bestPath = temp_path;
        }
    }
    if (best_loot != -1) { //we found more loot
        return scene->getData()->getLoot() + best_loot; //add our current scene's loot to the count
    }
    return -1;
}

// TODO 19: the optimal advisor. Use bestLoot from the first scene.
// Print:
//   Optimal advisor
//     Path: <path>
//     Loot: <loot>
//     Result: reached an ending
// If no path survives, print only:
//   Optimal advisor
//     Result: no path survives this story
void Adventure::optimalRun() const {
    int hp = startHp_;
    int loot = 0;
    Scene<Moment>* scene = tree_.getRoot();
    std::string path;
    std::string best_path;

    loot = bestLoot(scene, hp, path, best_path);

    std::cout << "Optimal advisor" << std::endl;
    if (loot < 0) {
        std::cout << "  Result: no path survives this story" << std::endl;
    }
    else {
        std::cout << "  Path: " << best_path << std::endl;
        std::cout << "  Loot: " << loot << std::endl;
        std::cout << "  Result: reached an ending" << std::endl;
    }
}

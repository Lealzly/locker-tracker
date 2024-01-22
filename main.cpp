/*
Program Name: Locker Tracker
Programmer Name: Osa George-Mannella
Programming Language: C++
Date Project started: Jan 22nd 20 2024
Date Last edited: Jan 22nd 20 2024
Program Description: The Locker Tracker tracks the location and owner of each locker within TAS.

Data Dictionary:
command_list: A list of objects that represent the commands which the user can run.
command_line: Represents the text entered by the user and is then parsed and passed into the individual handlers.
found: Represents whether or not the entered command line matches an existing command.
selected_function: Represents the ActionCall function of the command that the user entered.
command: Represents the command when iterating through the command_list
*/
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "colorText.cpp"
#include "utilityFunctions.cpp"
#include "handlers.cpp"
#include "assets.cpp"

int main()
{
    std::cout << "Type \"help\" for a list of commands" << std::endl; // Outputs the help command for new users
    command_list = {{"help", "Displays all of the commands", "help", help}, {"list", "Lists all the lockers which follow the inputted parameters as shown below", "list --range=(int-int) --floor=(int) --hallway=(string)", list}, {"info", "Lists all the info on the locker owner, requires user to input locker number", "info (int)", info}, {"clear", "Clears the console", "clear", clear}}; // Defines all of the commands that the user can call with their name, description, usage and ActionCall function that will handle the command.
    while (true) // Defines a true look to ask the user unlimited times
    {
        std::string command_line; // Defines the command_line that will store the text entered by the user.
        std::cout << "> "; // Outputs line prefix
        std::getline(std::cin, command_line); // Uses getline to get the input from the user to ensure the entire line is captured.
        if (command_line == "exit") // If the command_line is exit just terminate the program
            return 0;
        bool found = false; // Set found to false as a flag so it can later be set to true if a command matching the input is found.
        FunctionType selected_function; // Defines the selected function that will store the handler function.
        for (const Command command : command_list) // Iterates through the command_list
        {
            if (command.name == command_line.substr(0, command.name.length())) // Checks if the command name is equal to the first x letters of the comnmand line x being the length of the command.
            {
                found = true; // Sets found to true to prevent later error
                selected_function = command.ActionCall; // Sets the selected_function to the actioncall of the command.
                break; // Breaks from the for-loop. This does not terminate the upper while loop.
            }
        }
        if (!found) // If no command was found ouput an error.
        {
            PrintColor(RED, "Command Not Found");
        }
        else // Otherwise run the function that was found.
        {
            selected_function(command_line);
        }
    }
}

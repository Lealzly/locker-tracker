#include <iostream>
#include <vector>
#include "json.hpp"
#include <string>
#include <sstream>
#include <functional>
#include <fstream>
#include <regex>
#include "assets.cpp"
#define OPTIONAL_PREFIX "--"
#ifdef __linux__
#define CLEAR "clear"
#elif _WIN32
#define CLEAR "cls"
#elif __APPLE__
#define CLEAR "clear"
#else
#define CLEAR "clear"
#endif

using json = nlohmann::json;
using FunctionType = std::function<void(std::string)>;
std::ifstream f("students.json");

class Command
{
public:
    std::string name;
    std::string description;
    std::string usage;
    FunctionType ActionCall;

    Command(std::string newName, std::string newDescription, std::string newUsage, FunctionType func)
        : name(newName), description(newDescription), usage(newUsage), ActionCall(func) {}
};

std::vector<Command> command_list;

void help(std::string helpArgs)
{
    for (const Command cmd : command_list)
    {
        std::cout << cmd.name << "\t" << GREEN << cmd.description << std::endl
                  << BLUE << "\tUsage: " << cmd.usage << RESET << std::endl;
    }
}

void printOwner(json owner_json)
{
    PrintColor(BLUE, "Owner:");
    if (owner_json.is_null())
    {
        PrintColor(RED, std::string(SOFTTAB) + "This Locker has no Owner");
        return;
    }
    std::string student_name = owner_json["name"].get<std::string>();
    int student_grade = owner_json["grade"].get<int>();
    std::cout << SOFTTAB << BLUE << "Name: " << RESET << student_name << std::endl
              << SOFTTAB << BLUE << "Grade: " << RESET << student_grade << "%" << std::endl;
}

void info(std::string infoArgs)
{
    std::vector<std::string> tokens = stringSplit(infoArgs, ' ');
    if (tokens.size() != 2)
    {
        Error("Incorrect number of arguments", false);
        return;
    }
    int lockerNumber = stringToInt(tokens[1]);
    json locker_value;
    json data = json::parse(f);
    for (auto &el : data.items())
    {
        json value = el.value();
        if (value["lockerNumber"].get<int>() == lockerNumber)
        {
            locker_value = value;
        }
    }
    std::cout << BLUE << "Number #: " << RESET << locker_value["lockerNumber"].get<int>() << std::endl
              << BLUE << "Hallway: " << RESET << locker_value["Hallway"].get<std::string>() << std::endl;
    printOwner(locker_value["Owner"]);
}

void clear(std::string clearArgs)
{
    std::system(CLEAR);
    std::cout << GREEN << "Console Cleared" << RESET << std::endl;
}

void list(std::string listArgs)
{
    std::regex range_regex("(?<=--range(\\s|=))([0-9]+-[0-9]+)");
    std::regex floor_regex("(?<=--floor(\\s|=))([0-9])");
    std::regex hallway_regex("(?<=--hallway(\\s|=))(\\w+)");
    json data = json::parse(f);
    std::smatch match;

    if (std::regex_search(listArgs, match, range_regex))
    {
        std::vector<std::string> tokens = splitString(match.str(), "-");
        if (tokens.size() != 2)
        {
            Error("Incorrect number of arguments", false);
            return;
        }
        int bottomRange = stringToInt(tokens[0]);
        int topRange = stringToInt(tokens[1]);
        int index = -1;
        for (auto &el : data)
        {
            index++;
            int locker_number = el["lockerNumber"].get<int>();
            if (locker_number > topRange || locker_number < bottomRange)
            {
                data.erase(index);
            }
        }
    }
    if (std::regex_search(listArgs, match, floor_regex))
    {
        int floor = stringToInt(match.str());
        int index = -1;
        for (auto &el : data)
        {
            index++;
            if (el["Floor"].get<int>() != floor)
            {
                data.erase(index);
            }
        }
    }
    if (std::regex_search(listArgs, match, hallway_regex))
    {
        std::string hallway_string(match.str());
        int index = -1;
        for (auto &el : data)
        {
            index++;
            if (el["Hallway"].get<std::string>() != hallway_string)
            {
                data.erase(index);
            }
        }
    }
}
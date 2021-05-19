#include "parsexec.hpp"

static void ClearArgs()
{
    for(int i=0; i<26; i++)
    {
        gArgs[i] = nullptr;
    }
}

static bool CompareWithTag(const std::string& src, const std::string& tag)
{
    std::string::const_iterator src_it = src.begin();
    std::string::const_iterator tag_it = tag.begin();

    while(tag_it != tag.end())
    {
        while(*src_it==' ' && src_it!=src.end()) src_it++;
        while(*tag_it==' ' && tag_it!=tag.end()) tag_it++;

        bool charactersMatch = CompareCharInsensitive(*src_it, *tag_it);
        bool sourceEnded = (src_it == src.end());
        if( !charactersMatch || sourceEnded)
        {
            return false;
        }
        src_it++;
        tag_it++;
    }
    return true;
}

static bool MatchObjectTag(const std::string& src, const Object& obj,
                        int& minTagLength)
{
    bool result = false;
    for(std::string tag : obj.tags)
    {
        int tagLength = tag.length();
        if( tagLength <= minTagLength
            || !CompareWithTag(src, tag)) continue;
        minTagLength = tagLength;
        result = true;
    }
    return result;
}

static Object *FindByTagRecursion(Object* head, const std::string& input,
                        int &minTagLength, bool deepSearch)
{
    Object *match = nullptr;
    for(Object* o=head; o!=nullptr; o=o->next)
    {
        if(o != gpPlayer && MatchObjectTag(input, *o, minTagLength))
        {
            match = o;
        }

        if(deepSearch /*&& !ObjectHasProperty(*head, ObjectProperty::OBJECT_PROPERTY_HIDDEN_EQUIPMENT)*/)
        {
            Object* possibleMatch = FindByTagRecursion(head->inventoryHead, input, minTagLength, true);
            if(possibleMatch != nullptr) match = possibleMatch;
        }
    }
    return match;
}

static Object *FindByTag(const std::string& input,
                        int& minTagLength,
                        Distance minDistance, Distance maxDistance)
{
    Object* match = nullptr;
    if(IsInRange(DISTANCE_SELF, minDistance, maxDistance))
    {
        if(MatchObjectTag(input, *gpPlayer, minTagLength))
        {
            match = gpPlayer;
        }
    }

    if(IsInRange(DISTANCE_LOCATION, minDistance, maxDistance))
    {
        Object* parent = gpPlayer->parent;
        if(MatchObjectTag(input, *parent, minTagLength))
        {
            match = parent;
        }
    }

    if(IsInRange(DISTANCE_INVENTORY, minDistance, maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_INVENTORY_CONTAINED, minDistance, maxDistance);
        Object* possibleMatch = FindByTagRecursion(gpPlayer->inventoryHead, input, minTagLength, doDeepSearch);
        if(possibleMatch != nullptr) match = possibleMatch;
    }

    if(IsInRange(DISTANCE_NEAR, minDistance, maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_NEAR_CONTAINED, minDistance, maxDistance);
        Object *possibleMatch = FindByTagRecursion(gpPlayer->parent->inventoryHead, input, minTagLength, doDeepSearch);
        if(possibleMatch != nullptr) match = possibleMatch;
    }

    return match;
}

static bool MatchCommand(const std::string& input, const Command& cmd)
{
    ClearArgs();
    Object *match = nullptr;

    std::string::const_iterator pattern_it = cmd.pattern.begin();
    std::string::const_iterator input_it = input.begin();

    while(pattern_it != cmd.pattern.end())
    {
        while(*pattern_it==' ' && pattern_it!=cmd.pattern.end()) pattern_it++;
        while(*input_it==' ' && input_it!=input.end()) input_it++;

        if(IsUpper(*pattern_it))
        {
            int index = std::distance(input.begin(), input_it);
            int minTagLength = 0;
            match = FindByTag(input.substr(index, input.length()-index), minTagLength, cmd.minDistance, cmd.maxDistance);

            if(match != nullptr)
            {
                // ASCII:  A = 65, A -> 1st array element
                int index = (*pattern_it)-65;
                if(gArgs[index] == nullptr)
                {
                    gArgs[index] = match;
                }
                match = nullptr;

                // skip the name of matching object in the input string
                // to continue the comparison
                input_it += (minTagLength-1);
            }
            else
            {
                return false;
            }
        }
        else
        {
            bool charactersMatch = CompareCharInsensitive(*input_it, *pattern_it);
            bool sourceEnded = (input_it == input.end());
            if( !charactersMatch || sourceEnded) return false;
        }

        pattern_it++;
        input_it++;
    }

    return true;
}

extern std::string GetInput()
{
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

extern bool ParseInput(const std::string& input)
{
    std::vector<Command> commands = GetCommands();
    Command* cmd = nullptr;
    for(Command command : commands)
    {
        if(MatchCommand(input, command))
        {
            cmd = &command;
            break;
        }
    }

    if(cmd != nullptr)
    {
        if(cmd->function != nullptr)
        {
            return cmd->function(gArgs);
        }
        return true;
    }
    else
    {
        std::cout << "I don't know what you're trying to do." << std::endl;
    }
    return true;
}

#include <iostream>
#include <string>
#include <vector>


#define LOCAL_DEBUG 1
using namespace std;

using Commands = vector<string>;

class Observer
{
public:
    virtual void dump(Commands &cmd) = 0;
};

class Dumper
{
    Commands cmds;
    vector<Observer *> subs;
public:
    void subscribe(Observer *ob)
    {
        subs.push_back(ob);
    }

    void notify()
    {
        for (auto s : subs)
        {
            s->dump(cmds);
        }
    }

    void dump_commands(Commands cmd)
    {
        cmds = cmd;
        notify();
    }
};

class ConsoleDumper : public Observer
{
public:
    ConsoleDumper(Dumper *dmp)
    {
        dmp->subscribe(this);
    }

    void dump(Commands &cmd)
    {
        bool is_first = true;
        cout << "bulk: ";
        for(auto s : cmd)
        {
            if(is_first)
            {
                is_first = false;
            }
            else
            {
                cout << ", ";
            }
            cout << s;
        }
        cout << endl;
    }
};

class FileDumper : public Observer
{
public:
    FileDumper(Dumper *dmp)
    {
        dmp->subscribe(this);
    }

    void dump(Commands &cmd)
    {
        cout << "dump from FileDumper" << endl;
    }
};

class Processor
{
    Commands cmds;
    void addLine(string line);
};

int main(int argc, char *argv[])
{
#ifdef LOCAL_DEBUG
    freopen("cmd_input.txt", "rt", stdin);
#endif

    int commandsCount = 0;
    if ((argc > 1) &&
       (!strncmp(argv[1], "-v", 2) || !strncmp(argv[1], "--version", 9)))
    {
        cout << "version " /*<< version()*/ << endl;
        return 0;
    }
    else if (argc > 1) {
        commandsCount = atoi(argv[1]);
    }
    else
    {
        cout << "bad input" << endl;
        return 0;
    }

    Dumper dumper;

    ConsoleDumper conDumper(&dumper);
    FileDumper fileDumper(&dumper);

    vector<string> cmds;
    string line;
    int curCounter = 0;
    bool blockFound = false;
    int nestedBlocksCount = 0;
    while(getline(cin, line))
    {
        if((line != "{") && !blockFound)
        {
            ++curCounter;
            cmds.push_back(line);

            if(curCounter == commandsCount)
            {
                dumper.dump_commands(cmds);
                curCounter = 0;
                cmds.clear();
            }
        }
        else
        {
            if(!blockFound)
            {
                blockFound = true;
                if(cmds.size())
                {
                    dumper.dump_commands(cmds);
                    cmds.clear();
                }
                continue;
            }

            if(line == "{")
            {
                ++nestedBlocksCount;
            }
            else if(line == "}")
            {
                if (nestedBlocksCount > 0)
                {
                    --nestedBlocksCount;
                }
                else
                {
                    dumper.dump_commands(cmds);
                    cmds.clear();
                    blockFound = false;
                }
            }
            else
            {
                cmds.push_back(line);
            }
        }
    }

    if(curCounter)
    {
        dumper.dump_commands(cmds);
    }
    return 0;
}

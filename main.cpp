#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

//#define LOCAL_DEBUG 1
using namespace std;

class Commands
{
public:
    vector<string> cmds;
    time_t timestamp;
    int cmdCounter;

    void push_back(string str)
    {
        if(!cmds.size())
        {
            timestamp = time(nullptr);
        }
        cmds.push_back(str);
        ++cmdCounter;
    }

    void push_back_block(string str)
    {
        if(!cmds.size())
        {
            timestamp = time(nullptr);
        }
        cmds.push_back(str);
    }

    void clear()
    {
        cmds.clear();
        cmdCounter = 0;
    }
};

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

    void dump_commands(Commands &cmd)
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
        for(auto s : cmd.cmds)
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
        string filename = "bulk" + to_string(cmd.timestamp) + ".log";
        ofstream of(filename);

        bool is_first = true;
        of << "bulk: ";
        for(auto s : cmd.cmds)
        {
            if(is_first)
            {
                is_first = false;
            }
            else
            {
                of << ", ";
            }
            of << s;
        }
        of << endl;
        of.close();
    }
};



int main(int argc, char *argv[])
{
    try
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

        Commands cmds;
        string line;
        int curCounter = 0;
        bool blockFound = false;
        int nestedBlocksCount = 0;

        while(getline(cin, line))
        {
            if((line != "{") && !blockFound)
            {
                cmds.push_back(line);

                if(cmds.cmdCounter == commandsCount)
                {
                    dumper.dump_commands(cmds);
                    cmds.clear();
                }
            }
            else
            {
                if(!blockFound)
                {
                    blockFound = true;
                    if(cmds.cmdCounter)
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
                    cmds.push_back_block(line);
                }
            }
        }

        if(cmds.cmdCounter)
        {
            dumper.dump_commands(cmds);
        }
    }
    catch(exception &e)
    {
        cout << e.what();
    }
    return 0;
}

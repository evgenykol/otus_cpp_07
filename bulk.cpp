#include "bulk.h"

using namespace std;
using namespace bulk;

void Commands::push_back(string str)
{
    if(!cmds.size())
    {
        timestamp = time(nullptr);
        cmdCounter = 0;
    }
    cmds.push_back(str);
    ++cmdCounter;
}

void Commands::push_back_block(string str)
{
    if(!cmds.size())
    {
        timestamp = time(nullptr);
    }
    cmds.push_back(str);
}

void Commands::clear()
{
    cmds.clear();
    cmdCounter = 0;
}

void Dumper::subscribe(Observer *ob)
{
    subs.push_back(ob);
}

void Dumper::notify()
{
    for (auto s : subs)
    {
        s->dump(cmds);
    }
}

void Dumper::dump_commands(Commands &cmd)
{
    cmds = cmd;
    notify();
}

ConsoleDumper::ConsoleDumper(Dumper *dmp)
{
    dmp->subscribe(this);
}

void ConsoleDumper::dump(Commands &cmd)
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

FileDumper::FileDumper(Dumper *dmp)
{
    dmp->subscribe(this);
}

void FileDumper::dump(Commands &cmd)
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

BulkContext::BulkContext(size_t bulk_size_)
{
    //cout << "ctor BulkContext" << endl;
    bulk_size = bulk_size_;
    dumper = new Dumper();
    conDumper = new ConsoleDumper(dumper);
    fileDumper = new FileDumper(dumper);
}

BulkContext::~BulkContext()
{
    //cout << "dtor BulkContext" << endl;
    delete dumper;
    delete conDumper;
    delete fileDumper;
}

void BulkContext::add_line(string &cmd)
{
    if((cmd != "{") && !blockFound)
    {
        cmds.push_back(cmd);

        if(cmds.cmdCounter == bulk_size)
        {
            dumper->dump_commands(cmds);
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
                dumper->dump_commands(cmds);
                cmds.clear();
            }
            return;
        }

        if(cmd == "{")
        {
            ++nestedBlocksCount;
        }
        else if(cmd == "}")
        {
            if (nestedBlocksCount > 0)
            {
                --nestedBlocksCount;
            }
            else
            {
                dumper->dump_commands(cmds);
                cmds.clear();
                blockFound = false;
            }
        }
        else
        {
            cmds.push_back_block(cmd);
        }
    }
}

void BulkContext::end_input()
{
    if(cmds.cmdCounter)
    {
        dumper->dump_commands(cmds);
    }
}



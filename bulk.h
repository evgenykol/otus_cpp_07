#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "string.h"

using namespace std;

namespace bulk{

class Commands
{
public:
    vector<string> cmds;
    time_t timestamp;
    int cmdCounter;

    Commands();
    ~Commands(){}

    void push_back(string str);
    void push_back_block(string str);
    void clear();
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
    void subscribe(Observer *ob);
    void notify();
    void dump_commands(Commands &cmd);
};

class ConsoleDumper : public Observer
{
public:
    ConsoleDumper(Dumper *dmp);
    void dump(Commands &cmd);
};

class FileDumper : public Observer
{
public:

    FileDumper(Dumper *dmp);
    void dump(Commands &cmd);
    string get_unique_number();
};


class BulkContext
{
    Dumper* dumper;
    ConsoleDumper* conDumper;
    FileDumper* fileDumper;

    size_t bulk_size;
    bool blockFound;
    int nestedBlocksCount;

public:
    Commands cmds;
    BulkContext(size_t bulk_size);
    ~BulkContext();

    void add_line(string &cmd);
    void end_input();
};

}

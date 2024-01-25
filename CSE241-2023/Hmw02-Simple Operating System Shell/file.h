#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>
using std::string;

namespace MemOS
{
    class file
    {
    public:
        virtual void ls(char flag) = 0;
        virtual string getName() const = 0;
        virtual file* cd(const string& fileName) const = 0;
        //if something wrong, it returns nullptr;
        virtual bool mkdir(const string& fileName) = 0;
        virtual bool rm(const string& fileName) = 0;
        virtual void cat(const string& fileName) = 0;
        virtual string getDate() const = 0;
        virtual void setDate(string Date) = 0; 
        virtual ~file() {};
    };
}//namespace MemOS

#endif //FILE_H
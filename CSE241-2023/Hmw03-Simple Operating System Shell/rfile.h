#ifndef RFILE_H
#define RFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "file.h"
using std::fstream;
using std::string;
using std::vector;

namespace MemOS
{
    class rfile : public file
    {
    public:
        rfile() {}
        rfile(const string& path);
        rfile(const string& path, const string& name);
        bool openFile(std::ios_base::openmode mode);
        void closeFile();
        char get();
        void put(char ch);
        
        void cat(const string& fileName) override;
        bool mkdir(const string& fileName) override;
        bool rm(const string& fileName) override;
        file* cd(const string& fileName) const override;
        string getName() const override;
        string getPath() const;
        void ls(char flag) override;
        string getDate() const override;
        void setDate(string Date) override;
        
        virtual ~rfile();
    private:
        fstream fileStream;
        string filePath;
        string fileName;
        string modifiedDate;
    };
}//namespace MemOS

#endif //RFILE_H
#ifndef SLFILE_H
#define SLFILE_H

#include <string>
#include "file.h"
using std::string;

namespace MemOS
{
    class slfile : public file
    {
    public:
        slfile(const string& fileName, file* fileLink);
        file* getLink() const;
        void setLink(file* newLink);
        
        file* cd(const string& fileName) const override;
        void ls(char flag) override;
        bool mkdir(const string& fileName) override;
        bool rm(const string& fileName) override;
        void cat(const string& fileName) override;
        string getName() const override;
        string getDate() const override;
        void setDate(string Date) override;
    private:
        string name;
        file* link;
    };
    
}

#endif //SLFILE_H
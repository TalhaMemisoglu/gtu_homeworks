#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <string>
#include "file.h"
#include "rfile.h"
using std::string;
using std::vector;

namespace MemOS
{
    class directory : public file
    {
    public:
        directory() {}
        //directory(const directory& object);
        directory(file* previousDir, const string& fileName);
        void addFile(file* newFile);
        bool deleteFile(const string& fileName);
        bool isThereDir(const string& fileName) const;
        file* operator[](int index);
        directory* getPrevDirLink() const;
        directory* getCurrDirLink() const;
        int getSize();
        file* getFileLink(const string& fileName) const;
        
        file* cd(const string& fileName) const override;
        void ls(char flag) override;
        bool mkdir(const string& fileName) override;
        bool rm(const string& fileName) override;
        void cat(const string& fileName) override;
        string getName() const override;
        string getDate() const override;
        void setDate(string Date) override;
        
        virtual ~directory();
    private:
        vector<file*> unders;
        string name;
        string modifiedDate;
    };
}//namespace MemOS

#endif //DIRECTORY_H
#include <iostream>
#include "file.h"
#include "slfile.h"
#include "rfile.h"
#include "directory.h"
using std::cout;

namespace MemOS
{
    slfile::slfile(const string& fileName, file* fileLink) 
                : link(fileLink), name(fileName)
    {/*Intentionally left blank.*/}

    string slfile::getName() const
    {
        return name;
    }

    string slfile::getDate() const
    {
        return link->getDate();
    }

    void slfile::setDate(string Date)
    {
        link->setDate(Date);
    }

    file* slfile::cd(const string& fileName) const
    {
        if(dynamic_cast<directory*>(link))
        {
            return link;
        }
        else
        {
            rfile temp;
            return temp.cd(fileName);
        }
    }

    file* slfile::getLink() const
    {
        return link;
    }

    void slfile::setLink(file* newLink)
    {
        link = newLink;
    }

    void slfile::ls(char flag)
    {
        link->ls(flag);
    }

    void slfile::cat(const string& fileName)
    {
        if(name == fileName)
        {
            link->cat(fileName);
        }
    }

    bool slfile::mkdir(const string& fileName)
    {
        return link->mkdir(fileName);
    }

    bool slfile::rm(const string& fileName)
    {
        return link->rm(fileName);
    }
}
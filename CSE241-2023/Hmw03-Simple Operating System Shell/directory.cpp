#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include "directory.h"
#include "slfile.h"
#include "rfile.h"
using std::cout;
using std::endl;

namespace 
{
    std::string getCurrentDateWithHourMinute() 
    {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm* currentDate = std::localtime(&currentTime);

        char buffer[80];
        std::strftime(buffer, 80, "%b %d %H:%M", currentDate); // Format: Mon DD HH:MM

        return std::string(buffer);
    }
}

namespace MemOS
{
    directory::directory(file* previousDir, const string& fileName) : name(fileName)
    {
        if(previousDir == nullptr)
        {
            unders.push_back(new slfile(".", this));
        }
        else
        {
            unders.push_back(new slfile(".", this));
            unders.push_back(new slfile("..", previousDir));
        }
    }

    void directory::addFile(file* newFile)
    {
        bool exist = false;
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == newFile->getName())
            {
                exist = true;
                cout << "This file with same name exist!" << endl;
            }
        }
        
        if(!exist)
        {
            unders.push_back(newFile);
        }
    }

    bool directory::deleteFile(const string& fileName)
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == fileName)
            {
                delete unders[i];
                unders.erase(unders.begin() + i);
                return 1;
            }
        }
        return 0;
    }

    bool directory::isThereDir(const string& fileName) const
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == fileName)
            {      
                if(dynamic_cast<directory*>(unders[i]))
                {
                    return true;
                }
                else if(dynamic_cast<slfile*>(unders[i]))
                {
                    slfile *temp = dynamic_cast<slfile*>(unders[i]);
                    if(dynamic_cast<directory*>(temp->getLink()))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    file* directory::operator[](int index)
    {
        if(index >= unders.size())
        {
            cout << "Illegal index" << endl;
            return nullptr;
        }
        return unders[index];
    }

    directory* directory::getPrevDirLink() const
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == "..")
            {
                slfile* prev = dynamic_cast<slfile*>(unders[i]);
                return dynamic_cast<directory*>(prev->getLink());
            }
        }
        cout << "There is no previous directory!" << endl;
        return nullptr;
    }

    directory* directory::getCurrDirLink() const
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == ".")
            {
                slfile* curr = dynamic_cast<slfile*>(unders[i]);
                return dynamic_cast<directory*>(curr->getLink());
            }
        }
        cout << "Something went wrong!" << endl;
        return nullptr;
    }

    int directory::getSize()
    {
        return unders.size();
    }

    file* directory::getFileLink(const string& fileName) const
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == fileName)
            {
                if(slfile* slf = dynamic_cast<slfile*>(unders[i]))
                {
                    return slf->getLink();
                }
                else
                {
                    return unders[i];
                }
            }
        }

        return nullptr;
    }

    file* directory::cd(const string& fileName) const
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(unders[i]->getName() == fileName)
            {
                if(dynamic_cast<directory*>(unders[i]))
                {
                    return unders[i];
                }
                else if(dynamic_cast<slfile*>(unders[i]))
                {
                    slfile* temp = dynamic_cast<slfile*>(unders[i]);
                    if(dynamic_cast<directory*>(temp->getLink()))
                    {
                        return temp->getLink();
                    }
                    else if(dynamic_cast<rfile*>(temp->getLink()))
                    {
                        cout << "You can't do this operation with this type!" << endl;
                        return nullptr;
                    }
                }
                else
                {
                    cout << "You can't do this operation with this type!" << endl;
                    return nullptr;
                }
            }
        }
        return nullptr;
    }

    void directory::ls(char flag)
    {
        for(int i=0; i<unders.size(); i++)
        {
            if(dynamic_cast<directory*>(unders[i]))
            {
                cout << "D " << unders[i]->getName() << " " << unders[i]->getDate() <<endl;
                //displayDate(unders[i]->getName());
                if(flag == 'R')
                    unders[i]->ls(flag);
            }
            else if(dynamic_cast<slfile*>(unders[i]))
            {
                slfile* temp;
                temp = dynamic_cast<slfile*>(unders[i]);
                if(dynamic_cast<directory*>(temp->getLink()))
                {
                    cout << "D " << unders[i]->getName() << " " << unders[i]->getDate() << endl;
                    if(temp->getName() != "." && temp->getName() != "..")
                    {
                        if(flag == 'R')
                            unders[i]->ls(flag);
                    }
                }
                else if(dynamic_cast<rfile*>(temp->getLink()))
                {
                    cout << "F " << unders[i]->getName() << " " << unders[i]->getDate() << endl;
                }
            }
            else if(dynamic_cast<rfile*>(unders[i]))
            {
                cout << "F " << unders[i]->getName() << " " << unders[i]->getDate() << endl;
            }
        }
    }

    bool directory::mkdir(const string& fileName)
    {
        if(!isThereDir(fileName))
        {
            addFile(new directory(this,fileName));
            return true;
        }
        else if(isThereDir(fileName))
        {
            cout << "The file already exist!";
            return true;
        }
        else
        {
            cout << "You cannot do operation with this type!\n";
            return false;
        }
    }

    bool directory::rm(const string& fileName)
    {
        deleteFile(fileName);
        return true;
    }

    void directory::cat(const string& fileName)
    {
        cout << "You cannot do operation with this type!\n";
    }

    string directory::getName() const
    {
        return name;
    }

    string directory::getDate() const
    {
        return modifiedDate;
    }

    void directory::setDate(string Date)
    {
        modifiedDate = Date;
    }

    directory::~directory()
    {
        for(int i=0; i<unders.size(); i++)
        {
            delete unders[i];
        }
        std::filesystem::remove(name + ".txt");
    }

}//namespace MemOS
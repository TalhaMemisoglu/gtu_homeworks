#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "file.h"
#include "rfile.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

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
    rfile::rfile(const string& path) :filePath(path), fileName("NoName.txt")
    {/*Intentionally left blank*/}
    
    rfile::rfile(const string& path, const string& name) : filePath(path), fileName(name)
    {/*Intentionally left blank*/}

    bool rfile::openFile(std::ios_base::openmode mode)
    //If file doesnt exist, file is created.
    {
        std::ifstream file(filePath);
        if (!file) {
            // File doesn't exist, create the file
            std::ofstream newFile(filePath);
            if (newFile) {
                newFile.close();
            } else {
                cout << "Error creating file!\n";
            }
        }
        else {
            file.close();
        }
        
        fileStream.open(filePath, mode);
        return fileStream.is_open();
    }

    void rfile::closeFile()
    {
        if(fileStream.is_open())
        {
            modifiedDate = getCurrentDateWithHourMinute();
            fileStream.close();
        }
    }

    char rfile::get()
    {
        if(fileStream.is_open() && fileStream.good())
        {
            char character;
            fileStream.get(character);
            return character;
        }
        return 0;
    }

    void rfile::put(char ch)
    {
        if(fileStream.is_open() && fileStream.good())
        {
            fileStream.put(ch);
        }
    }

    void rfile::cat(const string& fileName)
    {
        if(fileName == this->fileName)
        {
            if(!fileStream.is_open())
            {
                openFile(std::ios_base::in);
            }
            if(fileStream.is_open() && fileStream.good())
            {
                char ch;
                while(fileStream.get(ch))
                {
                    cout << ch;
                }
            }
        }
    }

    bool rfile::mkdir(const string& fileName)
    {
        cout << "You cannot do operation with this type!\n";
        return false;
    }

    bool rfile::rm(const string& fileName)
    {
        cout << "You cannot do operation with this type!\n";
        return false;
    }

    file* rfile::cd(const string& fileName) const
    {
        cout << "You can not do this operation with this type!" << endl;
        return nullptr;
    }

    string rfile::getName() const
    {
        return fileName;
    }

    string rfile::getPath() const
    {
        return filePath;
    }

    void rfile::ls(char flag) 
    {
        cout << "You can not do this operation with this type!" << endl;
    }

    string rfile::getDate() const
    {
        return modifiedDate;
    }

    void rfile::setDate(string Date)
    {
        modifiedDate = Date;
    }

    rfile::~rfile()
    {
        std::filesystem::remove(filePath);
    }

}//namespace MemOs
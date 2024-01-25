#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "OSManager.h"
#include "directory.h"
#include "slfile.h"
#include "rfile.h"
using std::string;
using std::cout;
using std::cin;
using std::getline;
using MemOS::file;
using MemOS::rfile;
using MemOS::directory;
using MemOS::slfile;
using std::ifstream;
using std::ofstream;
using std::endl;

namespace
{
    void getPath(directory* currentLink, string& filePath, bool isFirst = true)
    //get path which is seen in our OS. 
    {
        if(currentLink->isThereDir(".."))
        {
            getPath(currentLink->getPrevDirLink(), filePath, false);
        }
        if (!isFirst)
        {
            filePath += currentLink->getName() + "_";
        } else {  
            filePath += currentLink->getName();
        }
    }

    void displayPath(directory* currentLink, bool isFirst = true)
    //print path to terminal.
    {
        if(currentLink->isThereDir(".."))
        {
            displayPath(currentLink->getPrevDirLink(), false);
        }
        cout << currentLink->getName();
        if(!currentLink->isThereDir(".."))
        {
            cout << ":";
        }
        if (!isFirst)
        {
            cout << "\\";
        }
    }

    bool isLegal4FileName(const string& fileName)
    {
        if(fileName == "D" || fileName == "..")
        {
            cout << "You can't do operation with a file which is this name!" << endl;
            return false;
        }
        const std::string invalidChars = "/\\:*?\"<>|_";
        for (char c : invalidChars) {
            if (fileName.find(c) != std::string::npos) {
                cout << "It is not allowed to input characters: /\\:*?\"<>|_" << endl;
                return false;
            }
        }
        return true;
    }

    string getCurrentDateWithHourMinute() 
    {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm* currentDate = std::localtime(&currentTime);

        char buffer[80];
        std::strftime(buffer, 80, "%b %d %H:%M", currentDate); // Format: Mon DD HH:MM

        return string(buffer);
    }

    string getFileName(string path)
    {
        size_t found = path.find_last_of("\\");
        found = path.find_last_of("_");
        if (found != std::string::npos) 
        {
            return path.substr(found + 1);
        }
        return path;
    }

    string subtFirstName(string& path)
    //Given a string, it extracts and returns the first file name.
    {
        string locationName;
        size_t found = path.find(':');
        if(found != std::string::npos){
            path.erase(found, 1);
        }

        found = path.find('\\');
        if (found != std::string::npos)
        {
            locationName = path.substr(0, found);
            path.erase(0, found + 1); // +1 to remove the slash too
        } else {
            locationName = path;
            path = ""; // If no slash found, set original string to empty
        }

        return locationName;
    }

    file* getLocation(directory* root, string path)
    {
        string locationName;
        directory* currentLoc = dynamic_cast<directory*>(root);
       
        locationName = subtFirstName(path);
        
        while(path != "")
        {
            locationName = subtFirstName(path);
            if(dynamic_cast<directory*>(currentLoc->getFileLink(locationName)))
            {
                if(path == "")
                {
                    return currentLoc->getFileLink(locationName);
                }
                else
                {
                    currentLoc = dynamic_cast<directory*>(currentLoc->getFileLink(locationName));
                }
            }
            else if(dynamic_cast<rfile*>(currentLoc->getFileLink(locationName)) || dynamic_cast<slfile*>(currentLoc->getFileLink(locationName)))
            {
                if(path == "")
                {
                    return currentLoc->getFileLink(locationName);
                }
                else
                {
                    return nullptr;
                }
            }
            else
            {
                return nullptr;
            }
        }

        return nullptr;
    }

    bool copyFileFromOS(directory* currentLink, string fromPath)
    {
        string fromName = getFileName(fromPath);
        ifstream input;
        input.open(fromName);
        if(input.is_open())
        {
            string filePath;
            getPath(currentLink,filePath);
            filePath += "_" + fromName;

            rfile* newFile = new rfile(filePath, fromName);
            if(newFile->openFile(std::ios::out))
            {
                char ch;
                while(input.get(ch))
                {
                    newFile->put(ch);        
                }
            }
            currentLink->addFile(newFile);
            newFile->closeFile();
            input.close();
            return 1;
        }
        else
        {
            cout << "File doesn't exist." << endl;
        }
        return 0;
    }

    void copy(directory* root, directory* currentLink, string fileFrom, string fileTo)
    {
        file* fromLocation; 
        if(fromLocation = getLocation(dynamic_cast<directory*>(root),fileFrom))
        {
            if(fileTo == ".")
            {
                if(dynamic_cast<slfile*>(fromLocation))
                {
                    slfile* dirLocS = dynamic_cast<slfile*>(fromLocation);
                    fromLocation = dirLocS->getLink();
                }
                            
                if(dynamic_cast<directory*>(fromLocation))
                {
                    directory* dirLoc = dynamic_cast<directory*>(fromLocation);
                    currentLink->addFile(dirLoc);
                }
                else if(dynamic_cast<rfile*>(fromLocation))
                {
                    rfile* temp = dynamic_cast<rfile*>(fromLocation);
                    string filePath = temp->getPath();
                    copyFileFromOS(currentLink, filePath);
                }
            }
        }
        else
        {
            copyFileFromOS(currentLink, fileFrom);
        }
    }

    rfile* createNewFile(directory* currentLink, string fileName)
    {
        if(isLegal4FileName(fileName))
        {
            string filePath;
            getPath(currentLink,filePath);
            filePath += "_" + fileName;
            std::ifstream file(filePath);
            if(!file)
            {
                rfile* newRFile = new rfile(filePath,fileName);
                newRFile->openFile(std::ios::in);
                newRFile->closeFile();
                return newRFile;
            }
            else {
                cout << "File already exists.\n";
                file.close();
            }
        }
        return nullptr;
    }

    void save(directory* currentLink)
    {
        string fileName = currentLink->getName() + ".txt";
        std::ofstream file(fileName);
        if (!file) {
            std::cout << "Error creating file!\n";
        }

        for(int i=0; i<currentLink->getSize(); i++)
        {
            if(dynamic_cast<directory*>(currentLink->operator[](i)))
            {
                directory* dir = dynamic_cast<directory*>(currentLink->operator[](i));
                file << "D " << dir->getName() << ".txt " << getCurrentDateWithHourMinute() << endl;
                save(dynamic_cast<directory*>(currentLink->operator[](i)));
            }
            else if(dynamic_cast<rfile*>(currentLink->operator[](i)))
            {
                rfile* rf = dynamic_cast<rfile*>(currentLink->operator[](i));
                file << "F " << rf->getPath() << " " <<  rf->getDate() << endl;
            }
        }

        file.close();
    }

    void load(directory* currentLink ,string fileName)
    {
        fileName += ".txt";
        std::ifstream file(fileName);

        std::string line, date, month, day, hour;
        char type, space;
        while(file.get(type))
        {
            file.get(space);
            file >> line;
            file.get(space);
            file >> month >> day >> hour;
            file.get(space);
            date = month + " " + day + " " + hour;
            if(type == 'D')
            {
                size_t found = line.find('.');
                if (found != std::string::npos)
                {
                    line = line.substr(0, found);
                }
                directory* dir = new directory(currentLink, line);
                dir->setDate(date);
                currentLink->addFile(dir);
                load(dir, line);
            }
            else if(type == 'F')
            {
                rfile* rf = new rfile(line, getFileName(line));
                rf->setDate(date);
                currentLink->addFile(rf);
            }
        }

        file.close();
    }
}

namespace MemOS
{
    OSManager::OSManager()
    {
        root = new directory(nullptr, "D");
        crloc = root;
    }

    void OSManager::start()
    {
        load(dynamic_cast<directory*>(root), root->getName());
        string command;
        while(true)
        {
            directory* currentLink = dynamic_cast<directory*>(crloc);
            directory* rootLink = dynamic_cast<directory*>(root);
            displayPath(currentLink);
            cout << ">";
            cin >> command;
            if(command == "ls")
            {
                char space, line , flag;
                flag = ' ';
                cin.get(space);
                if(space != '\n')
                {
                    cin.get(line);
                    cin.get(flag);
                }
                crloc->ls(flag);
            }
            else if(command == "mkdir")
            {
                string fileName;
                cin >> fileName;
                if(isLegal4FileName(fileName))
                {
                    crloc->mkdir(fileName);
                }
                else
                {
                    cout << "You can not delete this file!\n";
                }
            }
            else if (command == "cp")
            //precondition:File must be in same directory with operating system.
            //precondition:input file's name.
            {
                string fileFrom;
                cin >> fileFrom;
                string fileTo;
                cin >> fileTo;
                copy(rootLink, currentLink, fileFrom, fileTo);
            }
            else if(command == "cat")
            {
                string fileName;
                cin >> fileName;
                file* fileptr;
                directory* currentLink = dynamic_cast<directory*>(crloc);
                if(fileptr = currentLink->getFileLink(fileName))
                {
                    fileptr->cat(fileName);
                    cout << endl;
                }
            }
            else if(command == "touch")
            {
                string fileName;
                cin >> fileName;
                rfile* newRFile = createNewFile(currentLink, fileName);
                currentLink->addFile(newRFile);
            }
            else if(command == "rm")
            {
                string fileName;
                cin >> fileName;
                if(isLegal4FileName(fileName))
                {
                    currentLink->rm(fileName);
                }
            }
            else if(command == "cd")
            {
                string filePath;
                cin >> filePath;
                if(crloc->cd(filePath))
                {
                    crloc = crloc->cd(filePath);
                }
                else
                {
                    file* loc;
                    if(loc = getLocation(rootLink,filePath))
                    {
                        crloc = loc;
                    }
                    else
                    {
                        cout << "There is no file that you typed!" << endl;
                    }
                }
            }
            else if(command == "link")
            {
                string target, linkName;
                cin >> target;
                cin >> linkName;
                file* link;
                if(link = currentLink->getFileLink(target))
                {
                    slfile* newlFile = new slfile(linkName, link);
                    currentLink->addFile(newlFile);
                } else if(link = getLocation(rootLink,target)){
                    slfile* newlFile = new slfile(linkName, link);
                    currentLink->addFile(newlFile);
                }
            }
            else if(command == "exit")
            {
                save(dynamic_cast<directory*>(crloc));
                break;
            }
        }
    }
}
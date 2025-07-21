#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

void displayMenu();

struct TabEntry
{
    string songName;
    string filePath;
};

class GuitarTabManager
{
    TabEntry tabArray[100];
    int currentTabCount;

public:
    GuitarTabManager() : currentTabCount(0){} //constructor to initialize each GuitarTabManager object's currentTabCount to 0 to avoid garbage data

    void addTab(const string& songName, const string& filePath);
    void viewTabList();
    void openTab(const string& songName);
    void printTabToConsole(const string& songName);
    void deleteTab(const string& songName);
    void loadTabsFromFile(const string& fileName);
    void saveTabsToFile(const string& fileName);

};

int main()
{
    GuitarTabManager tabManager;
    //tabManager.loadTabsFromFile();

    int userChoice;
    do
    {
        cout << "\n************************" << endl;
        cout << "Guitar Tab Manager Menu: " << endl;
        cout << "************************" << endl;

        displayMenu();

        cout << "\nEnter a choice from the menu: " << endl;
        cin >> userChoice;

        switch(userChoice){
            case 1: {
                cin.ignore();
                string songName, filePath;
                cout << "Enter the song name: ";
                getline(cin, songName);
                cout << "\nEnter the file Path";
                getline(cin, filePath);
                tabManager.addTab(songName, filePath);
                break;
            }

            case 2:
                tabManager.viewTabList();
                break;

            case 3: {
                cin.ignore();
                string songName;
                cout << "Enter the song name for which you want to view/edit the tab";
                getline(cin, songName);
                tabManager.openTab(songName);
                break;
            }

            case 4: {
                cin.ignore();
                string songName;
                cout << "Enter the name of the song that you want to print to the console: ";
                getline(cin, songName);
                tabManager.printTabToConsole(songName);
                break;
            }

            case 5: {
                cin.ignore();
                string songName;
                cout << "Enter the name of the song that you want to delete from the program: ";
                getline(cin, songName);
                tabManager.deleteTab(songName);
                break;
            }

            //case 6:





        }


    }while(userChoice != 7);

    return 0;
}


void GuitarTabManager::addTab(const string& songName, const string& filePath)
{
    if(currentTabCount >= 100){
        cout << "Error: Maximum number of tabs reached" << endl;
        return;
    }

    tabArray[currentTabCount].songName = songName;
    tabArray[currentTabCount].filePath = filePath;
    currentTabCount++;

    cout << "Tab for '" << songName << "' added successfully" << endl;

}

void GuitarTabManager::viewTabList()
{
    if(currentTabCount == 0){
        cout << "No tabs currently stored" << endl;
        return;
    }

    cout << "List of tabs:" << endl;
    for(int i{0}; i < currentTabCount; i++){
        cout << (i+1) << ". " << tabArray[i].songName << endl;
    }
}


void GuitarTabManager::openTab(const string& songName)
{
    string filePath;
    for(int i{0}; i < currentTabCount; i++){
        if(tabArray[i].songName == songName){
            filePath = tabArray[i].filePath;
            break;
        }
    }

    if(filePath.empty()){
        cerr << "Tab for '" << songName << "' not found" << endl;
        return;
    }

    ifstream file(filePath);
    if(!file.is_open()){
        cerr << "Error opening the tab file for '" << songName << "' " << endl;
        return;
    }
    file.close();

    string command = "start \"\" \"" + filePath + "\"";
    system(command.c_str());

}


void GuitarTabManager::printTabToConsole(const string& songName)
{
    string filePath;
    for(int i{0}; i < currentTabCount; i++){
        if(tabArray[i].songName == songName){
            filePath = tabArray[i].filePath;
            break;
        }
    }

    if(filePath.empty()){
        cerr << "Tab for '" << songName << "' not found" << endl;
        return;
    }

    ifstream file(filePath);
    if(!file.is_open()){
        cerr << "Error opening the tab file for '" << songName << "' " << endl;
        return;
    }

    cout << "Tab for '" << songName << "':" << endl;
    string line;
    while(getline(file, line)){
        cout << line << endl;
    }

    file.close();

}

void GuitarTabManager::deleteTab(const string& songName)
{
    int indexToDelete = -1;

    for(int i{0}; i < currentTabCount; i++){
        if(tabArray[i].songName == songName){
            indexToDelete = i;
            break;
        }
    }

    if(indexToDelete == -1){
        cerr << "Tab for '" << songName << "' not found" << endl;
        return;
    }

    for(int i{indexToDelete}; i < currentTabCount - 1; i++){
        tabArray[i] = tabArray[i+1];
    }

    currentTabCount--;

    cout << "Tab for '" << songName << "' deleted successfully" << endl;

}

void GuitarTabManager::loadTabsFromFile(const string& fileName)
{
    ifstream file(fileName);
    if(!file.is_open()){
        cerr << "Error opening the file '" << fileName << "'." << endl;
        return;
    }

    string line;
    int pos;
    while(getline(file, line)){
        if(currentTabCount >= 100){
            cerr << "Error: Maximum number of tabs reached, some tabs may not be loaded" << endl;
            break;
        }
        pos = line.find(',');
        if(pos == string::npos){
            cerr << "Error: invalid format in the file" << endl;
            continue;
        }

        string songName = line.substr(0,pos);
        string filePath = line.substr(pos + 2);

        if(filePath.empty()){
            cerr << "Error: File path is missing for '" << songName << "'" << endl;
            continue;
        }

        tabArray[currentTabCount].songName = songName;
        tabArray[currentTabCount].filePath = filePath;
        currentTabCount++;
    }
}

void GuitarTabManager::saveTabsToFile(const string& fileName)
{
    ofstream file(fileName);
    if(!file.is_open()){
        cerr << "Error opening the file '" << fileName << "' for writing." << endl;
        return;
    }

    for(int i{0}; i < currentTabCount; i++){
        file << tabArray[i].songName << ", " << tabArray[i].filePath << endl;
        if(file.fail()){
            cerr << "Error writing to the file '" << fileName << "'." << endl;
            file.close();
            return;
        }
    }

    cout << "Tabs saved successfully to '" << fileName << "'." << endl;

    file.close();
}

void displayMenu()
{
    cout << "1. Add tab to the program" << endl;
    cout << "2. View current list of tabs saved in program" << endl;
    cout << "3. Open a specific tab for viewing/editing" << endl;
    cout << "4. Print a tab to the console window for viewing" << endl;
    cout << "5. Delete a tab from the program" << endl;
    cout << "6. Save tab names and file paths to a text file" << endl;
    cout << "7. Exit the program" << endl;
}

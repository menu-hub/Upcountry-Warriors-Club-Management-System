#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

struct Player {
    string regNumber;
    string firstName;
    string lastName;
    string dob; 
    int runsScored;
    string teams[2]; 
};

struct Team {
    string teamName;
    string captain;
    int matchesPlayed;
};

Player players[100];  // Increased capacity
Team teams[100];
int noOfPlayers = 0;
int noOfTeams = 0;
const string PLAYER_FILENAME = "players.txt";
const string TEAM_FILENAME = "teams.txt";

// ANSI Color Codes for better interface
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";
const string UNDERLINE = "\033[4m";

// Function to clear screen (cross-platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to pause execution
void pauseScreen() {
    cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

// Function to display header
void displayHeader(string title) {
    clearScreen();
    cout << BOLD << BLUE << "+----------------------------------------------------------+\n";
    cout << "¦" << CYAN << "              UPCOUNTRY WARRIORS CRICKET CLUB              " << BLUE << "¦\n";
    cout << "¦" << YELLOW << "                     " << title << string(40 - title.length(), ' ') << BLUE << "¦\n";
    cout << "+----------------------------------------------------------+\n\n" << RESET;
}

// Function for loading animation
void loadingAnimation(string message) {
    cout << "\n" << CYAN << BOLD << message;
    for(int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        #ifdef _WIN32
            Sleep(300);
        #else
            usleep(300000);
        #endif
    }
    cout << RESET << "\n";
}

bool checkUser() {
    displayHeader("USER LOGIN");
    string userName, password;
    
    cout << BOLD << "?---------------------------------------------?\n";
    cout << "¦" << CYAN << "               Authentication               " << RESET << BOLD << "¦\n";
    cout << "?---------------------------------------------?\n\n" << RESET;
    
    cout << YELLOW << "? Username: " << RESET;
    cin >> userName;
    cout << YELLOW << "? Password: " << RESET;
    
    // Password masking
    char ch;
    password = "";
    ch = _getch();
    while(ch != 13) { // character 13 is enter
        password.push_back(ch);
        cout << '*';
        ch = _getch();
    }
    cout << "\n";
    
    loadingAnimation("Verifying credentials");
    
    if(userName == "warriorsclub" && password == "1234") {
        cout << GREEN << "? Login successful!" << RESET << "\n";
        #ifdef _WIN32
            Sleep(800);
        #else
            usleep(800000);
        #endif
        return true;
    } else {
        cout << RED << "? Invalid username or password!" << RESET << "\n";
        #ifdef _WIN32
            Sleep(1500);
        #else
            usleep(1500000);
        #endif
        return false;
    }
}

void loadPlayersFromFile() {
    ifstream inFile(PLAYER_FILENAME);
    if (!inFile) {
        return;
    }
    
    while (inFile >> players[noOfPlayers].regNumber 
                  >> players[noOfPlayers].firstName 
                  >> players[noOfPlayers].lastName 
                  >> players[noOfPlayers].dob 
                  >> players[noOfPlayers].runsScored 
                  >> players[noOfPlayers].teams[0] 
                  >> players[noOfPlayers].teams[1]) {
        noOfPlayers++;
    }
    inFile.close();
}

void savePlayersToFile() {
    ofstream outFile(PLAYER_FILENAME);
    for (int i = 0; i < noOfPlayers; i++) {
        outFile << players[i].regNumber << " "
                << players[i].firstName << " "
                << players[i].lastName << " "
                << players[i].dob << " "
                << players[i].runsScored << " "
                << players[i].teams[0] << " "
                << players[i].teams[1] << endl;
    }
    outFile.close();
}

void loadTeamsFromFile() {
    ifstream inFile(TEAM_FILENAME);
    if (!inFile) return;

    while (inFile >> teams[noOfTeams].teamName 
                  >> teams[noOfTeams].captain 
                  >> teams[noOfTeams].matchesPlayed) {
        noOfTeams++;
    }
    inFile.close();
}

void saveTeamsToFile() {
    ofstream outFile(TEAM_FILENAME);
    for (int i = 0; i < noOfTeams; i++) {
        outFile << teams[i].teamName << " "
                << teams[i].captain << " "
                << teams[i].matchesPlayed << endl;
    }
    outFile.close();
}

void addPlayer() {
    displayHeader("ADD NEW PLAYER");
    
    if (noOfPlayers >= 100) {
        cout << RED << "? Player database is full! Cannot add more players.\n" << RESET;
        pauseScreen();
        return;
    }

    Player newPlayer;
    
    cout << BOLD << "?---------------------------------------------?\n";
    cout << "¦" << CYAN << "            Player Information              " << RESET << BOLD << "¦\n";
    cout << "?---------------------------------------------?\n\n" << RESET;
    
    cout << YELLOW << "? Registration Number: " << RESET;
    cin >> newPlayer.regNumber;
    
    // Check if reg number already exists
    for(int i = 0; i < noOfPlayers; i++) {
        if(players[i].regNumber == newPlayer.regNumber) {
            cout << RED << "? Registration number already exists!\n" << RESET;
            pauseScreen();
            return;
        }
    }
    
    cout << YELLOW << "? First Name: " << RESET;
    cin >> newPlayer.firstName;
    cout << YELLOW << "? Last Name: " << RESET;
    cin >> newPlayer.lastName;
    cout << YELLOW << "? Date of Birth (DD/MM/YYYY): " << RESET;
    cin >> newPlayer.dob;
    cout << YELLOW << "? Runs Scored: " << RESET;
    cin >> newPlayer.runsScored;
    
    cout << "\n" << YELLOW << "? Enter Teams (Max 2, enter '-' for none):\n" << RESET;
    cout << "  Team 1: ";
    cin >> newPlayer.teams[0];
    cout << "  Team 2: ";
    cin >> newPlayer.teams[1];

    players[noOfPlayers] = newPlayer;
    noOfPlayers++;
    
    loadingAnimation("Saving player data");
    savePlayersToFile();
    
    cout << GREEN << "? Player added successfully!\n" << RESET;
    pauseScreen();
}

void viewPlayers() {
    displayHeader("PLAYER DATABASE");
    
    if(noOfPlayers == 0) {
        cout << YELLOW << "? No players found in the database.\n" << RESET;
        pauseScreen();
        return;
    }
    
    cout << BOLD << CYAN << "Total Players: " << noOfPlayers << "\n\n" << RESET;
    
    cout << BOLD << BLUE << "+-------------------------------------------------------------------------------------+\n";
    cout << "¦ " << CYAN << "No." << BLUE << " ¦ " << CYAN << "Reg Number" << BLUE << " ¦ " << CYAN << "Name" << BLUE << "             ¦ " << CYAN << "DOB" << BLUE << "        ¦ " << CYAN << "Runs" << BLUE << "       ¦ " << CYAN << "Teams" << BLUE << "          ¦\n";
    cout << "+-----+--------------+--------------------+------------+------------+-----------------¦\n" << RESET;
    
    for (int i = 0; i < noOfPlayers; i++) {
        string fullName = players[i].firstName + " " + players[i].lastName;
        string teamsStr = players[i].teams[0];
        if(players[i].teams[1] != "-" && players[i].teams[1] != "") {
            teamsStr += ", " + players[i].teams[1];
        }
        
        cout << "¦ " << setw(3) << i+1 << " ¦ " 
             << setw(12) << players[i].regNumber << " ¦ "
             << setw(18) << fullName.substr(0, 18) << " ¦ "
             << setw(10) << players[i].dob << " ¦ "
             << setw(10) << players[i].runsScored << " ¦ "
             << setw(15) << teamsStr.substr(0, 15) << " ¦\n";
    }
    cout << BOLD << BLUE << "+-------------------------------------------------------------------------------------+\n" << RESET;
    
    pauseScreen();
}

bool searchPlayerByRegNumber(string regNumber) {
    for (int i = 0; i < noOfPlayers; i++) {
        if (players[i].regNumber == regNumber) {
            displayHeader("PLAYER DETAILS");
            cout << BOLD << GREEN << "? Player Found!\n\n" << RESET;
            cout << BOLD << "?---------------------------------------------?\n";
            cout << "¦" << CYAN << "            Player Information              " << RESET << BOLD << "¦\n";
            cout << "?---------------------------------------------?\n\n" << RESET;
            
            cout << YELLOW << "? Registration Number: " << RESET << players[i].regNumber << "\n";
            cout << YELLOW << "? Name: " << RESET << players[i].firstName << " " << players[i].lastName << "\n";
            cout << YELLOW << "? Date of Birth: " << RESET << players[i].dob << "\n";
            cout << YELLOW << "? Runs Scored: " << RESET << players[i].runsScored << "\n";
            cout << YELLOW << "? Teams: " << RESET << players[i].teams[0];
            if(players[i].teams[1] != "-" && players[i].teams[1] != "") {
                cout << ", " << players[i].teams[1];
            }
            cout << "\n";
            
            return true;
        }
    }
    return false;
}

bool searchPlayerByName(string name) {
    bool found = false;
    displayHeader("SEARCH RESULTS");
    
    cout << BOLD << "?---------------------------------------------?\n";
    cout << "¦" << CYAN << "         Search Results for: " << setw(15) << left << name << RESET << BOLD << " ¦\n";
    cout << "?---------------------------------------------?\n\n" << RESET;
    
    for (int i = 0; i < noOfPlayers; i++) {
        if (players[i].firstName == name || players[i].lastName == name || 
            players[i].firstName + " " + players[i].lastName == name) {
            if(!found) {
                cout << GREEN << "? Matching players found:\n\n" << RESET;
                found = true;
            }
            
            cout << BOLD << CYAN << "Player " << i+1 << ":\n" << RESET;
            cout << "  " << YELLOW << "Reg No: " << RESET << players[i].regNumber << "\n";
            cout << "  " << YELLOW << "Name: " << RESET << players[i].firstName << " " << players[i].lastName << "\n";
            cout << "  " << YELLOW << "Runs: " << RESET << players[i].runsScored << "\n";
            cout << "  " << YELLOW << "Teams: " << RESET << players[i].teams[0];
            if(players[i].teams[1] != "-" && players[i].teams[1] != "") {
                cout << ", " << players[i].teams[1];
            }
            cout << "\n\n";
        }
    }
    return found;
}

void removePlayer() {
    displayHeader("REMOVE PLAYER");
    
    if(noOfPlayers == 0) {
        cout << YELLOW << "? No players to remove.\n" << RESET;
        pauseScreen();
        return;
    }
    
    string regNumber;
    cout << YELLOW << "? Enter Player Registration Number to remove: " << RESET;
    cin >> regNumber;

    for (int i = 0; i < noOfPlayers; i++) {
        if (players[i].regNumber == regNumber) {
            cout << "\n" << RED << BOLD << "? WARNING: About to delete player:\n" << RESET;
            cout << "Name: " << players[i].firstName << " " << players[i].lastName << "\n";
            cout << "Runs: " << players[i].runsScored << "\n\n";
            
            cout << YELLOW << "Are you sure? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            
            if(confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < noOfPlayers - 1; j++) {
                    players[j] = players[j + 1];
                }
                noOfPlayers--;
                
                loadingAnimation("Removing player");
                savePlayersToFile();
                
                cout << GREEN << "? Player removed successfully.\n" << RESET;
            } else {
                cout << YELLOW << "Operation cancelled.\n" << RESET;
            }
            pauseScreen();
            return;
        }
    }
    cout << RED << "? Player not found.\n" << RESET;
    pauseScreen();
}

void addTeam() {
    displayHeader("ADD NEW TEAM");
    
    if (noOfTeams >= 100) {
        cout << RED << "? Team database is full!\n" << RESET;
        pauseScreen();
        return;
    }

    cout << BOLD << "?---------------------------------------------?\n";
    cout << "¦" << CYAN << "              Team Information               " << RESET << BOLD << "¦\n";
    cout << "?---------------------------------------------?\n\n" << RESET;
    
    cout << YELLOW << "? Team Name: " << RESET;
    cin.ignore();
    getline(cin, teams[noOfTeams].teamName);
    
    cout << YELLOW << "? Captain Name: " << RESET;
    getline(cin, teams[noOfTeams].captain);
    
    cout << YELLOW << "? Matches Played: " << RESET;
    cin >> teams[noOfTeams].matchesPlayed;
    
    noOfTeams++;
    
    loadingAnimation("Saving team data");
    saveTeamsToFile();
    
    cout << GREEN << "? Team added successfully!\n" << RESET;
    pauseScreen();
}

void viewTeams() {
    displayHeader("TEAM DATABASE");
    
    if(noOfTeams == 0) {
        cout << YELLOW << "? No teams found in the database.\n" << RESET;
        pauseScreen();
        return;
    }
    
    cout << BOLD << CYAN << "Total Teams: " << noOfTeams << "\n\n" << RESET;
    
    cout << BOLD << BLUE << "+-----------------------------------------------------------------+\n";
    cout << "¦ " << CYAN << "No." << BLUE << " ¦ " << CYAN << "Team Name" << BLUE << "         ¦ " << CYAN << "Captain" << BLUE << "            ¦ " << CYAN << "Matches Played" << BLUE << " ¦\n";
    cout << "+-----+--------------------+--------------------+-----------------¦\n" << RESET;
    
    for (int i = 0; i < noOfTeams; i++) {
        cout << "¦ " << setw(3) << i+1 << " ¦ "
             << setw(18) << left << teams[i].teamName.substr(0, 18) << " ¦ "
             << setw(18) << left << teams[i].captain.substr(0, 18) << " ¦ "
             << setw(15) << teams[i].matchesPlayed << " ¦\n";
    }
    cout << BOLD << BLUE << "+-----------------------------------------------------------------+\n" << RESET;
    
    pauseScreen();
}

void removeTeam() {
    displayHeader("REMOVE TEAM");
    
    if(noOfTeams == 0) {
        cout << YELLOW << "? No teams to remove.\n" << RESET;
        pauseScreen();
        return;
    }
    
    string teamName;
    cout << YELLOW << "? Enter Team Name to remove: " << RESET;
    cin.ignore();
    getline(cin, teamName);

    for (int i = 0; i < noOfTeams; i++) {
        if (teams[i].teamName == teamName) {
            cout << "\n" << RED << BOLD << "? WARNING: About to delete team:\n" << RESET;
            cout << "Team: " << teams[i].teamName << "\n";
            cout << "Captain: " << teams[i].captain << "\n\n";
            
            cout << YELLOW << "Are you sure? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            
            if(confirm == 'y' || confirm == 'Y') {
                // Remove this team from all players
                for(int j = 0; j < noOfPlayers; j++) {
                    if(players[j].teams[0] == teamName) {
                        players[j].teams[0] = players[j].teams[1];
                        players[j].teams[1] = "-";
                    } else if(players[j].teams[1] == teamName) {
                        players[j].teams[1] = "-";
                    }
                }
                
                // Remove team from array
                for (int j = i; j < noOfTeams - 1; j++) {
                    teams[j] = teams[j + 1];
                }
                noOfTeams--;
                
                loadingAnimation("Removing team");
                saveTeamsToFile();
                savePlayersToFile();
                
                cout << GREEN << "? Team removed successfully.\n" << RESET;
            } else {
                cout << YELLOW << "Operation cancelled.\n" << RESET;
            }
            pauseScreen();
            return;
        }
    }
    cout << RED << "? Team not found.\n" << RESET;
    pauseScreen();
}

void displayStatistics() {
    displayHeader("SYSTEM STATISTICS");
    
    int totalRuns = 0;
    int maxRuns = 0;
    string bestPlayer;
    
    for(int i = 0; i < noOfPlayers; i++) {
        totalRuns += players[i].runsScored;
        if(players[i].runsScored > maxRuns) {
            maxRuns = players[i].runsScored;
            bestPlayer = players[i].firstName + " " + players[i].lastName;
        }
    }
    
    double avgRuns = noOfPlayers > 0 ? (double)totalRuns / noOfPlayers : 0;
    
    cout << BOLD << "?---------------------------------------------?\n";
    cout << "¦" << CYAN << "              Club Statistics               " << RESET << BOLD << "¦\n";
    cout << "?---------------------------------------------?\n\n" << RESET;
    
    cout << YELLOW << "? Total Players: " << RESET << noOfPlayers << "\n";
    cout << YELLOW << "? Total Teams: " << RESET << noOfTeams << "\n";
    cout << YELLOW << "? Total Runs Scored: " << RESET << totalRuns << "\n";
    cout << YELLOW << "? Average Runs per Player: " << RESET << fixed << setprecision(2) << avgRuns << "\n";
    if(noOfPlayers > 0) {
        cout << YELLOW << "? Top Scorer: " << RESET << bestPlayer << " (" << maxRuns << " runs)\n";
    }
    
    pauseScreen();
}

void displayMainMenu() {
    int choice;
    
    while (true) {
        displayHeader("MAIN MENU");
        
        cout << BOLD << "?---------------------------------------------?\n";
        cout << "¦" << CYAN << "            Management System               " << RESET << BOLD << "¦\n";
        cout << "?---------------------------------------------?\n\n" << RESET;
        
        cout << GREEN << BOLD << " PLAYER MANAGEMENT\n" << RESET;
        cout << "   1. " << CYAN << "? Add New Player\n" << RESET;
        cout << "   2. " << CYAN << "?? View All Players\n" << RESET;
        cout << "   3. " << CYAN << "?? Search Player by Registration Number\n" << RESET;
        cout << "   4. " << CYAN << "?? Search Player by Name\n" << RESET;
        cout << "   5. " << CYAN << "??? Remove Player\n" << RESET;
        
        cout << "\n" << GREEN << BOLD << " TEAM MANAGEMENT\n" << RESET;
        cout << "   6. " << CYAN << "? Add New Team\n" << RESET;
        cout << "   7. " << CYAN << "?? View All Teams\n" << RESET;
        cout << "   8. " << CYAN << "??? Remove Team\n" << RESET;
        
        cout << "\n" << GREEN << BOLD << " SYSTEM\n" << RESET;
        cout << "   9. " << CYAN << "?? View Statistics\n" << RESET;
        cout << "   0. " << RED << "?? Exit System\n" << RESET;
        
        cout << "\n" << BOLD << "?---------------------------------------------?\n";
        cout << "¦" << YELLOW << " Enter your choice (0-9): " << RESET;
        cin >> choice;
        cout << BOLD << "?---------------------------------------------?\n" << RESET;

        switch (choice) {
            case 1:
                addPlayer();
                break;
            case 2:
                viewPlayers();
                break;
            case 3: {
                displayHeader("SEARCH PLAYER BY REG NUMBER");
                string regNumber;
                cout << YELLOW << "? Enter Registration Number: " << RESET;
                cin >> regNumber;
                if (!searchPlayerByRegNumber(regNumber)) {
                    cout << RED << "? Player not found.\n" << RESET;
                }
                pauseScreen();
                break;
            }
            case 4: {
                displayHeader("SEARCH PLAYER BY NAME");
                string name;
                cout << YELLOW << "? Enter Player Name: " << RESET;
                cin >> name;
                if (!searchPlayerByName(name)) {
                    cout << RED << "? No players found with that name.\n" << RESET;
                }
                pauseScreen();
                break;
            }
            case 5:
                removePlayer();
                break;
            case 6:
                addTeam();
                break;
            case 7:
                viewTeams();
                break;
            case 8:
                removeTeam();
                break;
            case 9:
                displayStatistics();
                break;
            case 0:
                loadingAnimation("Saving all data");
                savePlayersToFile();
                saveTeamsToFile();
                cout << GREEN << "? All data saved successfully!\n" << RESET;
                cout << YELLOW << "\nThank you for using Upcountry Warriors Club System!\n" << RESET;
                #ifdef _WIN32
                    Sleep(1500);
                #else
                    usleep(1500000);
                #endif
                exit(0);
            default:
                cout << RED << "? Invalid choice! Please enter 0-9.\n" << RESET;
                pauseScreen();
        }
    }
}

void loginMenu() {
    int attempts = 0;
    
    while (attempts < 3) {
        displayHeader("WELCOME");
        
        cout << BOLD << "?---------------------------------------------?\n";
        cout << "¦" << CYAN << "          Upcountry Warriors Club           " << RESET << BOLD << "¦\n";
        cout << "¦" << YELLOW << "          Player Management System          " << RESET << BOLD << "¦\n";
        cout << "?---------------------------------------------?\n\n" << RESET;
        
        cout << BOLD << "?---------------------------------------------?\n";
        cout << "¦" << CYAN << "              Login Options                 " << RESET << BOLD << "¦\n";
        cout << "?---------------------------------------------?\n\n" << RESET;
        
        cout << "   1. " << GREEN << "?? Login to System\n" << RESET;
        cout << "   2. " << RED << "?? Exit Application\n" << RESET;
        
        cout << "\n" << YELLOW << "? Select option (1-2): " << RESET;
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (checkUser()) {
                displayMainMenu();
                return;
            } else {
                attempts++;
                cout << RED << "\n? Attempt " << attempts << " of 3 failed.\n" << RESET;
                if(attempts < 3) {
                    cout << YELLOW << "Try again in 2 seconds...\n" << RESET;
                    #ifdef _WIN32
                        Sleep(2000);
                    #else
                        usleep(2000000);
                    #endif
                }
            }
        } else if (choice == 2) {
            cout << YELLOW << "\nThank you for visiting Upcountry Warriors Club!\n" << RESET;
            #ifdef _WIN32
                Sleep(1000);
            #else
                usleep(1000000);
            #endif
            exit(0);
        } else {
            cout << RED << "? Invalid choice! Please select 1 or 2.\n" << RESET;
            pauseScreen();
        }
    }

    displayHeader("ACCESS DENIED");
    cout << RED << BOLD << "?---------------------------------------------?\n";
    cout << "¦" << "       Maximum Login Attempts Exceeded!       " << RESET << RED << BOLD << "¦\n";
    cout << "?---------------------------------------------?\n\n" << RESET;
    cout << "Please contact system administrator for assistance.\n";
    pauseScreen();
    exit(0);
}

int main() {
    // Load data at startup
    loadingAnimation("Loading player database");
    loadPlayersFromFile();
    
    loadingAnimation("Loading team database");
    loadTeamsFromFile();
    
    // Display welcome animation
    displayHeader("SYSTEM STARTUP");
    cout << GREEN << "? System initialized successfully!\n" << RESET;
    cout << YELLOW << "? Loaded " << noOfPlayers << " players and " << noOfTeams << " teams\n" << RESET;
    
    #ifdef _WIN32
        Sleep(1500);
    #else
        usleep(1500000);
    #endif
    
    // Start login process
    loginMenu();
    
    return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "Features.h"

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clearConsole() {
    system("cls");
}

void displayAsciiArt() {
    setConsoleColor(11);
    std::cout << R"(
Welcome to the Combo Tool!
==================================

)";
    setConsoleColor(7);
}

void displayMenu() {
    setConsoleColor(10);
    std::cout << "+==========================================+\n";
    std::cout << "|                Main Menu                 |\n";
    std::cout << "+==========================================+\n";
    setConsoleColor(7);


    std::cout << "\n";


    setConsoleColor(10); std::cout << "[1] "; setConsoleColor(7); std::cout << "Load Combo List\n";
    setConsoleColor(10); std::cout << "[2] "; setConsoleColor(7); std::cout << "Save Combo List\n";
    setConsoleColor(10); std::cout << "[3] "; setConsoleColor(7); std::cout << "Remove Duplicates\n";
    setConsoleColor(10); std::cout << "[4] "; setConsoleColor(7); std::cout << "Sort Lines Alphabetically\n";
    setConsoleColor(10); std::cout << "[5] "; setConsoleColor(7); std::cout << "Sort by Domain\n";
    setConsoleColor(10); std::cout << "[6] "; setConsoleColor(7); std::cout << "Merge Combo Lists\n";
    setConsoleColor(10); std::cout << "[7] "; setConsoleColor(7); std::cout << "Split Combo List\n";
    setConsoleColor(10); std::cout << "[8] "; setConsoleColor(7); std::cout << "Split Combo by Domain\n"; // New option
    setConsoleColor(10); std::cout << "[9] "; setConsoleColor(7); std::cout << "Filter by Domain or Keyword\n";
    setConsoleColor(10); std::cout << "[10] "; setConsoleColor(7); std::cout << "Shuffle Lines\n";
    setConsoleColor(10); std::cout << "[11] "; setConsoleColor(7); std::cout << "Extract Unique Domains or Usernames\n";
    setConsoleColor(10); std::cout << "[12] "; setConsoleColor(7); std::cout << "Remove Specific Domains or Keywords\n";
    setConsoleColor(10); std::cout << "[13] "; setConsoleColor(7); std::cout << "Count Lines by Domain\n";
    setConsoleColor(10); std::cout << "[14] "; setConsoleColor(7); std::cout << "Find and Replace Text\n";
    setConsoleColor(10); std::cout << "[15] "; setConsoleColor(7); std::cout << "Duplicate Each Line\n";
    setConsoleColor(10); std::cout << "[16] "; setConsoleColor(7); std::cout << "Remove Invalid Entries\n";
    setConsoleColor(10); std::cout << "[17] "; setConsoleColor(7); std::cout << "Convert Email:Password to User:Password\n";
    setConsoleColor(10); std::cout << "[18] "; setConsoleColor(7); std::cout << "Convert User:Password to Email:Password\n";
    setConsoleColor(10); std::cout << "[19] "; setConsoleColor(7); std::cout << "Exit\n";

    setConsoleColor(7);
    std::cout << "\nSelect an option: ";
}

int main() {
    std::vector<std::string> comboList;
    std::vector<std::string> filteredList;
    std::string loadedFileName;
    int loadedLineCount = 0;

    SetConsoleTitle(TEXT("Open Source Combo Editor"));

    clearConsole();
    displayAsciiArt();

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            if (Features::loadComboList(comboList, loadedFileName, loadedLineCount)) {
                clearConsole();
                displayAsciiArt();
                std::cout << "Loaded " << loadedLineCount << " lines from " << loadedFileName << ".\n\n";
            }
            break;

        case 2:
            if (Features::saveComboList(comboList)) {
                clearConsole();
                displayAsciiArt();
                std::cout << "Successfully saved " << comboList.size() << " lines.\n\n";
            }
            break;

        case 3:
            Features::removeDuplicates(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Removed duplicates. Total unique lines: " << comboList.size() << ".\n\n";
            break;

        case 4:
            Features::sortComboList(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Sorted lines alphabetically. Total lines: " << comboList.size() << ".\n\n";
            break;

        case 5:
            Features::sortByDomain(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Sorted by domain. Total lines: " << comboList.size() << ".\n\n";
            break;

        case 6:
            Features::mergeComboLists(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Merged combo lists successfully. Total unique lines: " << comboList.size() << ".\n\n";
            break;

        case 7:
            Features::splitComboList(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Combo list split completed successfully.\n\n";
            break;

        case 8:
            Features::splitComboByDomain(comboList);
            clearConsole();
            displayAsciiArt();
            break;

        case 9:
            Features::filterByDomainOrKeyword(comboList, filteredList);
            clearConsole();
            displayAsciiArt();
            break;

        case 10:
            Features::shuffleComboList(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Shuffle operation completed.\n\n";
            break;

        case 11: {
            char option;
            std::cout << "Do you want to extract (d)omains or (u)sernames? ";
            std::cin >> option;
            clearConsole();
            displayAsciiArt();
            if (option == 'd' || option == 'D') {
                Features::extractUniqueParts(comboList, true);
            }
            else if (option == 'u' || option == 'U') {
                Features::extractUniqueParts(comboList, false);
            }
            else {
                std::cout << "Invalid option. Returning to the main menu.\n\n";
            }
            break;
        }

        case 12:
            Features::removeSpecificLines(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Remove operation completed.\n\n";
            break;

        case 13:
            Features::countLinesByDomain(comboList);
            clearConsole();
            displayAsciiArt();
            break;

        case 14:
            Features::findAndReplaceText(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Find and Replace operation completed.\n\n";
            break;

        case 15:
            Features::duplicateEachLine(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Duplicate operation completed.\n\n";
            break;

        case 16:
            Features::removeInvalidEntries(comboList);
            clearConsole();
            displayAsciiArt();
            break;

        case 17:
            Features::convertEmailToUser(comboList);
            clearConsole();
            displayAsciiArt();
            std::cout << "Converted email:password to user:password for " << comboList.size() << " lines.\n\n";
            break;

        case 18: {
            std::string domain;
            std::cout << "Enter the domain to append (e.g., @example.com): ";
            std::cin >> domain;
            Features::convertUserToEmail(comboList, domain);
            clearConsole();
            displayAsciiArt();
            std::cout << "Converted user:password to email:password for " << comboList.size() << " lines.\n\n";
            break;
        }

        case 19:
            clearConsole();
            displayAsciiArt();
            std::cout << "Exiting program. Goodbye!\n";
            break;

        default:
            clearConsole();
            displayAsciiArt();
            std::cout << "Invalid choice. Please try again.\n\n";
        }
    } while (choice != 19);

    return 0;
}


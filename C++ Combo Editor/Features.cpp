#define NOMINMAX // Prevent windows.h from defining min/max macros
#include "Features.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <windows.h>
#include <random>
#include <unordered_map>

namespace Features {

    // Load a combo list from a file
    bool loadComboList(std::vector<std::string>& comboList, std::string& loadedFileName, int& loadedLineCount) {
        OPENFILENAMEA ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn)) {
            std::ifstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open file.\n";
                return false;
            }

            comboList.clear();
            std::string line;
            loadedLineCount = 0;
            while (std::getline(file, line)) {
                comboList.push_back(line);
                loadedLineCount++;
            }
            file.close();

            loadedFileName = fileName;
            return true;
        }
        else {
            std::cerr << "File selection canceled or an error occurred.\n";
            return false;
        }
    }

    // Save a combo list to a file
    bool saveComboList(const std::vector<std::string>& comboList) {
        OPENFILENAMEA ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "Text Files\0*.txt\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

        if (GetSaveFileNameA(&ofn)) {
            std::string fileNameStr(fileName);

            if (fileNameStr.substr(fileNameStr.find_last_of(".") + 1) != "txt") {
                fileNameStr += ".txt";
            }

            std::ofstream file(fileNameStr);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open file for writing.\n";
                return false;
            }

            for (const auto& line : comboList) {
                file << line << '\n';
            }
            file.close();

            return true;
        }
        else {
            std::cerr << "File saving canceled or an error occurred.\n";
            return false;
        }
    }

    // Remove duplicates from a combo list
    void removeDuplicates(std::vector<std::string>& comboList) {
        std::unordered_set<std::string> uniqueLines(comboList.begin(), comboList.end());
        comboList.assign(uniqueLines.begin(), uniqueLines.end());
    }

    // Sort a combo list alphabetically
    void sortComboList(std::vector<std::string>& comboList) {
        std::sort(comboList.begin(), comboList.end());
    }

    // Sorts the combo list by domain
    void sortByDomain(std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        // Sort based on domain
        std::sort(comboList.begin(), comboList.end(), [](const std::string& a, const std::string& b) {
            size_t atPosA = a.find('@');
            size_t atPosB = b.find('@');
            if (atPosA != std::string::npos && atPosB != std::string::npos) {
                return a.substr(atPosA + 1) < b.substr(atPosB + 1); // Compare domains
            }
            return a < b; // Fallback to lexicographical sorting
            });

        std::cout << "Sorted the combo list by domain successfully.\n";

        // Ask if the user wants to save the sorted list
        char saveChoice;
        std::cout << "Do you want to save the sorted list? (y/n): ";
        std::cin >> saveChoice;

        if (saveChoice == 'y' || saveChoice == 'Y') {
            if (saveComboList(comboList)) {
                std::cout << "Sorted list saved successfully.\n";
            }
            else {
                std::cerr << "Failed to save the sorted list.\n";
            }
        }
    }

    // Merge multiple combo lists
    void mergeComboLists(std::vector<std::string>& mainComboList) {
        std::vector<std::string> tempComboList;
        std::string loadedFileName;
        int loadedLineCount;

        std::cout << "Select a combo list to merge.\n";

        while (loadComboList(tempComboList, loadedFileName, loadedLineCount)) {
            std::cout << "Merged " << loadedLineCount << " lines from " << loadedFileName << ".\n";
            mainComboList.insert(mainComboList.end(), tempComboList.begin(), tempComboList.end());
            std::cout << "Do you want to add another file? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'n' || choice == 'N') {
                break;
            }
        }

        removeDuplicates(mainComboList);
        std::cout << "Duplicates removed. Total unique lines: " << mainComboList.size() << ".\n";
    }

    // Split a combo list into smaller files
    void splitComboList(const std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        int linesPerFile;
        std::cout << "Enter the number of lines per file: ";
        std::cin >> linesPerFile;

        if (linesPerFile <= 0) {
            std::cout << "Invalid number of lines per file. Please enter a positive number.\n";
            return;
        }

        int totalFiles = (comboList.size() + linesPerFile - 1) / linesPerFile; // Calculate total number of files
        std::cout << "Splitting into " << totalFiles << " files...\n";

        for (int i = 0; i < totalFiles; ++i) {
            std::string fileName = "split_" + std::to_string(i + 1) + ".txt";

            std::ofstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "Error: Could not create file " << fileName << ".\n";
                continue;
            }

            int startIndex = i * linesPerFile;
            int endIndex = std::min(static_cast<int>(comboList.size()), startIndex + linesPerFile);

            for (int j = startIndex; j < endIndex; ++j) {
                file << comboList[j] << '\n';
            }

            file.close();
            std::cout << "Created file: " << fileName << " with " << (endIndex - startIndex) << " lines.\n";
        }

        std::cout << "Combo list split completed successfully.\n";
    }

    // Splits the combo list by domain name.
    void splitComboByDomain(const std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        std::unordered_map<std::string, std::vector<std::string>> domainMap;

        // Group lines by domain
        for (const auto& line : comboList) {
            size_t atPos = line.find('@');
            size_t colonPos = line.find(':');
            if (atPos != std::string::npos && colonPos != std::string::npos && atPos < colonPos) {
                std::string domain = line.substr(atPos + 1, colonPos - atPos - 1);
                domainMap[domain].push_back(line);
            }
        }

        if (domainMap.empty()) {
            std::cout << "No valid domains found in the combo list.\n";
            return;
        }

        // Save each domain group to its own file
        for (const auto& [domain, lines] : domainMap) {
            std::string fileName = domain;
            std::replace(fileName.begin(), fileName.end(), '.', '_'); // Replace '.' with '_' in filenames
            fileName += ".txt";

            std::ofstream outFile(fileName);
            if (outFile.is_open()) {
                for (const auto& line : lines) {
                    outFile << line << '\n';
                }
                outFile.close();
                std::cout << "Saved " << lines.size() << " lines to \"" << fileName << "\".\n";
            }
            else {
                std::cerr << "Failed to save file for domain: " << domain << ".\n";
            }
        }
    }

    // Filters the combo list by domain name or keyword.
    void filterByDomainOrKeyword(const std::vector<std::string>& comboList, std::vector<std::string>& filteredList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        std::string keyword;
        std::cout << "Enter the domain (e.g., @gmail.com) or keyword to filter by: ";
        std::cin >> keyword;

        filteredList.clear();
        for (const auto& line : comboList) {
            if (line.find(keyword) != std::string::npos) {
                filteredList.push_back(line);
            }
        }

        if (filteredList.empty()) {
            std::cout << "No entries matched the keyword or domain \"" << keyword << "\".\n";
        }
        else {
            std::string fileName = "filtered_" + keyword + ".txt";

            // Replace special characters in filename with underscores
            for (auto& ch : fileName) {
                if (ch == ':' || ch == '/' || ch == '\\' || ch == '?' || ch == '*' || ch == '<' || ch == '>' || ch == '|') {
                    ch = '_';
                }
            }

            std::ofstream file(fileName);
            if (file.is_open()) {
                for (const auto& line : filteredList) {
                    file << line << '\n';
                }
                file.close();
                std::cout << "Filtered " << filteredList.size() << " entries matching \"" << keyword << "\".\n";
                std::cout << "Saved to \"" << fileName << "\" successfully.\n";
            }
            else {
                std::cerr << "Error: Could not save the filtered list to \"" << fileName << "\".\n";
            }
        }
    }

    // Shuffles lines in the combo list, for what reason? No idea.
    void shuffleComboList(std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        // Create a random engine based on a random device
        std::random_device rd;
        std::mt19937 generator(rd());

        // Shuffle the combo list
        std::shuffle(comboList.begin(), comboList.end(), generator);

        std::cout << "The combo list has been shuffled successfully.\n";

        // Ask if the user wants to save the shuffled list
        char saveChoice;
        std::cout << "Do you want to save the shuffled list to a new file? (y/n): ";
        std::cin >> saveChoice;

        if (saveChoice == 'y' || saveChoice == 'Y') {
            std::string fileName = "shuffled_combo_list.txt";
            std::ofstream file(fileName);
            if (file.is_open()) {
                for (const auto& line : comboList) {
                    file << line << '\n';
                }
                file.close();
                std::cout << "Shuffled list saved to \"" << fileName << "\" successfully.\n";
            }
            else {
                std::cerr << "Error: Could not save the shuffled list to \"" << fileName << "\".\n";
            }
        }
    }

    // Extract unique domain names or unique usernames from selected combolist
    void extractUniqueParts(const std::vector<std::string>& comboList, bool extractDomains) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        std::unordered_set<std::string> uniqueParts;
        for (const auto& line : comboList) {
            size_t delimiterPos = line.find(':');
            if (delimiterPos != std::string::npos) {
                size_t atPos = line.find('@');
                if (extractDomains && atPos != std::string::npos) {
                    // Extract domain part
                    uniqueParts.insert(line.substr(atPos + 1, delimiterPos - atPos - 1));
                }
                else if (!extractDomains) {
                    // Extract username part
                    if (atPos != std::string::npos) {
                        uniqueParts.insert(line.substr(0, atPos));
                    }
                    else {
                        uniqueParts.insert(line.substr(0, delimiterPos)); // For user:password
                    }
                }
            }
        }

        if (uniqueParts.empty()) {
            std::cout << "No unique entries found.\n";
        }
        else {
            std::string fileType = extractDomains ? "domains" : "usernames";
            std::string fileName = "unique_" + fileType + ".txt";

            std::ofstream file(fileName);
            if (file.is_open()) {
                for (const auto& part : uniqueParts) {
                    file << part << '\n';
                }
                file.close();
                std::cout << "Extracted " << uniqueParts.size() << " unique " << fileType << ".\n";
                std::cout << "Saved to \"" << fileName << "\" successfully.\n";
            }
            else {
                std::cerr << "Error: Could not save the unique " << fileType << ".\n";
            }
        }
    }

    // Removes specific lines from the combolist, for example: remove all lines containing @gmail.com
    void removeSpecificLines(std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        std::string keyword;
        std::cout << "Enter the domain (e.g., @gmail.com) or keyword to remove: ";
        std::cin >> keyword;

        // Count lines before removal
        size_t initialSize = comboList.size();

        // Use `std::remove_if` to filter out matching lines
        comboList.erase(
            std::remove_if(
                comboList.begin(),
                comboList.end(),
                [&keyword](const std::string& line) { return line.find(keyword) != std::string::npos; }
            ),
            comboList.end()
        );

        size_t removedCount = initialSize - comboList.size();

        if (removedCount == 0) {
            std::cout << "No lines matched the keyword \"" << keyword << "\".\n";
        }
        else {
            std::cout << "Removed " << removedCount << " lines containing \"" << keyword << "\".\n";

            // Ask if the user wants to save the updated list
            char saveChoice;
            std::cout << "Do you want to save the updated list? (y/n): ";
            std::cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y') {
                if (saveComboList(comboList)) {
                    std::cout << "Updated list saved successfully.\n";
                }
                else {
                    std::cerr << "Failed to save the updated list.\n";
                }
            }
        }
    }

    // Counts the different domain names in the combo and saves the amount to a file.
    void countLinesByDomain(const std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        std::unordered_map<std::string, int> domainCounts;

        // Count occurrences of each domain
        for (const auto& line : comboList) {
            size_t atPos = line.find('@');
            size_t colonPos = line.find(':');
            if (atPos != std::string::npos && colonPos != std::string::npos && atPos < colonPos) {
                std::string domain = line.substr(atPos + 1, colonPos - atPos - 1);
                domainCounts[domain]++;
            }
        }

        if (domainCounts.empty()) {
            std::cout << "No valid domains found in the combo list.\n";
        }
        else {
            std::string fileName = "domain_counts.txt";
            std::ofstream file(fileName);

            std::cout << "Domain Counts:\n";
            if (file.is_open()) {
                file << "Domain Counts:\n";
            }

            for (const auto& [domain, count] : domainCounts) {
                std::cout << domain << ": " << count << " lines\n";
                if (file.is_open()) {
                    file << domain << ": " << count << " lines\n";
                }
            }

            if (file.is_open()) {
                file.close();
                std::cout << "\nSaved domain counts to \"" << fileName << "\" successfully.\n";
            }
        }
    }


    // Replaces text in the combo list, for example: replacing "@hotmail.com" with "@gmail.com"
    void findAndReplaceText(std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        std::string findText, replaceText;
        std::cout << "Enter the text to find: ";
        std::cin >> findText;
        std::cout << "Enter the text to replace it with: ";
        std::cin >> replaceText;

        size_t count = 0;

        for (auto& line : comboList) {
            size_t pos = line.find(findText);
            while (pos != std::string::npos) {
                line.replace(pos, findText.length(), replaceText);
                count++;
                pos = line.find(findText, pos + replaceText.length());
            }
        }

        if (count == 0) {
            std::cout << "No occurrences of \"" << findText << "\" were found in the combo list.\n";
        }
        else {
            std::cout << "Replaced " << count << " occurrences of \"" << findText << "\" with \"" << replaceText << "\".\n";

            // Ask if the user wants to save the updated list
            char saveChoice;
            std::cout << "Do you want to save the updated list? (y/n): ";
            std::cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y') {
                if (saveComboList(comboList)) {
                    std::cout << "Updated list saved successfully.\n";
                }
                else {
                    std::cerr << "Failed to save the updated list.\n";
                }
            }
        }
    }

    // Duplicates each line in the combo list. Pointless feature but could be useful somehow.
    void duplicateEachLine(std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        size_t originalSize = comboList.size();
        std::vector<std::string> duplicatedList;

        // Duplicate each line
        for (const auto& line : comboList) {
            duplicatedList.push_back(line);
            duplicatedList.push_back(line); // Add duplicate
        }

        comboList = duplicatedList; // Replace original list with the duplicated list

        std::cout << "Duplicated each line. Total lines increased from " << originalSize << " to " << comboList.size() << ".\n";

        // Ask if the user wants to save the updated list
        char saveChoice;
        std::cout << "Do you want to save the updated list? (y/n): ";
        std::cin >> saveChoice;

        if (saveChoice == 'y' || saveChoice == 'Y') {
            if (saveComboList(comboList)) {
                std::cout << "Updated list saved successfully.\n";
            }
            else {
                std::cerr << "Failed to save the updated list.\n";
            }
        }
    }


    // Removes invalid entries from the combo list, anything that doesn't include a email:pass, user:pass format
    void removeInvalidEntries(std::vector<std::string>& comboList) {
        if (comboList.empty()) {
            std::cout << "The combo list is empty. Please load a combo list first.\n";
            return;
        }

        size_t originalSize = comboList.size();

        // Remove lines that do not conform to the valid format
        comboList.erase(
            std::remove_if(
                comboList.begin(),
                comboList.end(),
                [](const std::string& line) {
                    size_t colonPos = line.find(':');
                    return colonPos == std::string::npos || // No colon
                        colonPos == 0 || // Empty username/email
                        colonPos == line.length() - 1; // Empty password
                }),
            comboList.end());

        size_t removedCount = originalSize - comboList.size();

        if (removedCount == 0) {
            std::cout << "No invalid entries found. All lines are in the correct format.\n";
        }
        else {
            std::cout << "Removed " << removedCount << " invalid entries. Remaining lines: " << comboList.size() << ".\n";

            // Ask if the user wants to save the updated list
            char saveChoice;
            std::cout << "Do you want to save the updated list? (y/n): ";
            std::cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y') {
                if (saveComboList(comboList)) {
                    std::cout << "Updated list saved successfully.\n";
                }
                else {
                    std::cerr << "Failed to save the updated list.\n";
                }
            }
        }
    }

    // Convert email:password to user:password
    void convertEmailToUser(std::vector<std::string>& comboList) {
        for (auto& line : comboList) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                size_t atPos = line.find('@');
                if (atPos != std::string::npos && atPos < pos) {
                    line.erase(atPos, pos - atPos);
                }
            }
        }
    }

    // Convert user:password to email:password
    void convertUserToEmail(std::vector<std::string>& comboList, const std::string& domain) {
        for (auto& line : comboList) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                size_t atPos = line.find('@');
                if (atPos == std::string::npos) {
                    line.insert(pos, domain);
                }
            }
        }
    }

}

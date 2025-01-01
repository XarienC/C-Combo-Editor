#ifndef FEATURES_H
#define FEATURES_H

#include <vector>
#include <string>

namespace Features {
    bool loadComboList(std::vector<std::string>& comboList, std::string& loadedFileName, int& loadedLineCount);
    bool saveComboList(const std::vector<std::string>& comboList);
    void removeDuplicates(std::vector<std::string>& comboList);
    void sortComboList(std::vector<std::string>& comboList);
    void sortByDomain(std::vector<std::string>& comboList);
    void mergeComboLists(std::vector<std::string>& mainComboList);
    void splitComboList(const std::vector<std::string>& comboList);
    void splitComboByDomain(const std::vector<std::string>& comboList);
    void filterByDomainOrKeyword(const std::vector<std::string>& comboList, std::vector<std::string>& filteredList);
    void shuffleComboList(std::vector<std::string>& comboList);
    void extractUniqueParts(const std::vector<std::string>& comboList, bool extractDomains);
    void removeSpecificLines(std::vector<std::string>& comboList);
    void countLinesByDomain(const std::vector<std::string>& comboList);
    void findAndReplaceText(std::vector<std::string>& comboList);
    void duplicateEachLine(std::vector<std::string>& comboList);
    void removeInvalidEntries(std::vector<std::string>& comboList);
    void convertEmailToUser(std::vector<std::string>& comboList);
    void convertUserToEmail(std::vector<std::string>& comboList, const std::string& domain);
}

#endif

//
// Created by toby.wichmann on 26.07.2024.
//

#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

class Datei
{
public:
    Datei(const std::string& filePath, const std::string& fileName);
    void readAndTransferFile() const;
    std::streampos getFileSize() const;
    void overFileSize(std::streampos maxSize) const;

private:
    std::string filePath;
    std::string fileName;
    std::string getZeitStempel() const;
};

#endif // MAIN_HPP
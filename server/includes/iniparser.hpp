#ifndef INIPARSER_HPP
#define INIPARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

// Configuration variable.
struct VariableConfig {
    std::string description; // Associated comment.
    std::string nom;         // Key, for example host or port.
    std::string valeur;      // Value, for example 127.0.0.1 or 8080.
};

// Configuration section, for example Server or Database.
struct SectionConfig {
    std::string nom;                         // Section name.
    std::vector<VariableConfig> variables;  // Variables in this section.

    // Find a variable by name.
    const VariableConfig* getVariable(const std::string& varName) const {
        for (const auto& var : variables)
            if (var.nom == varName) return &var;
        return nullptr;
    }
};

class IniParser {
public:
    // Remove unnecessary leading and trailing whitespace.
    static std::string trim(const std::string& str);

    // Extract a field value from a JSON object.
    static std::string getField(const std::string &obj, const std::string &key);

    // Parse an INI file.
    static std::vector<SectionConfig> parse(const std::string& filepath);
};

#endif // INIPARSER_HPP
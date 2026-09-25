#include "../includes/iniparser.hpp"

std::string IniParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) 
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, (last - first + 1));
}

std::string IniParser::getField(const std::string &obj, const std::string &key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = obj.find(pattern);

    if (pos == std::string::npos)
        return "";

    size_t colon = obj.find(':', pos + pattern.size());
    if (colon == std::string::npos)
        return "";

    size_t valueStart = obj.find_first_not_of(" \t\r\n", colon + 1);
    if (valueStart == std::string::npos)
        return "";

    // String value.
    if (obj[valueStart] == '"') {
        size_t valueEnd = valueStart + 1;
        while (valueEnd < obj.size()) {
            if (obj[valueEnd] == '\\' && valueEnd + 1 < obj.size()) {
                valueEnd += 2;
                continue;
            }
            if (obj[valueEnd] == '"')
                break;
            
            ++valueEnd;
        }
        return obj.substr(valueStart + 1, valueEnd - valueStart - 1);
    }

    // Number, boolean, or null value.
    size_t valueEnd = valueStart;
    while (valueEnd < obj.size() && obj[valueEnd] != ',' && obj[valueEnd] != '}')
        ++valueEnd;

    return IniParser::trim(obj.substr(valueStart, valueEnd - valueStart));
}

std::vector<SectionConfig> IniParser::parse(const std::string& filepath) {
    std::vector<SectionConfig> sections;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[IniParser] Error: unable to open file: " << filepath << std::endl;
        return sections;
    }

    std::string line;
    std::string accumulatedComments;
    SectionConfig* currentSection = nullptr;

    while (getline(file, line)) {
        std::string trimmedLine = trim(line);

        // Empty line: reset the accumulated description.
        if (trimmedLine.empty()) {
            accumulatedComments.clear();
            continue;
        }

        // Comment line (starts with ';' or '#').
        if (trimmedLine[0] == ';' || trimmedLine[0] == '#') {
            std::string commentContent = trim(trimmedLine.substr(1));
            if (!accumulatedComments.empty())
                accumulatedComments += " "; // Separate multi-line comments.
            
            accumulatedComments += commentContent;
            continue;
        }

        // Section declaration: [SectionName].
        if (trimmedLine.front() == '[' && trimmedLine.back() == ']') {
            std::string sectionName = trim(trimmedLine.substr(1, trimmedLine.size() - 2));
            sections.push_back({sectionName, {}});
            currentSection = &sections.back();
            accumulatedComments.clear(); // Reset comments for the section.
            continue;
        }

        // Variable declaration (key = value).
        size_t delimiterPos = trimmedLine.find('=');
        if (delimiterPos != std::string::npos && currentSection != nullptr) {
            std::string key = trim(trimmedLine.substr(0, delimiterPos));
            std::string value = trim(trimmedLine.substr(delimiterPos + 1));

            VariableConfig var;
            var.nom = key;
            var.valeur = value;
            var.description = accumulatedComments; // Store the collected description.

            currentSection->variables.push_back(var);
            
            // Reset for the next variable.
            accumulatedComments.clear();
        }
    }
    file.close();
    return sections;
}

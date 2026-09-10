#include "iniparser.hpp"

string IniParser::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::string IniParser::getField(const std::string &obj, const std::string &key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = obj.find(pattern);
    if (pos == std::string::npos) {
        return "";
    }

    size_t colon = obj.find(':', pos + pattern.size());
    if (colon == std::string::npos) {
        return "";
    }

    size_t valueStart = obj.find_first_not_of(" \t\r\n", colon + 1);
    if (valueStart == std::string::npos) {
        return "";
    }

    // Cas chaîne de caractères
    if (obj[valueStart] == '"') {
        size_t valueEnd = valueStart + 1;
        while (valueEnd < obj.size()) {
            if (obj[valueEnd] == '\\' && valueEnd + 1 < obj.size()) {
                valueEnd += 2;
                continue;
            }
            if (obj[valueEnd] == '"') {
                break;
            }
            ++valueEnd;
        }
        return obj.substr(valueStart + 1, valueEnd - valueStart - 1);
    }

    // Cas nombre / bool / null
    size_t valueEnd = valueStart;
    while (valueEnd < obj.size() && obj[valueEnd] != ',' && obj[valueEnd] != '}') {
        ++valueEnd;
    }
    return IniParser::trim(obj.substr(valueStart, valueEnd - valueStart));
}

vector<SectionConfig> IniParser::parse(const string& filepath) {
    vector<SectionConfig> sections;
    ifstream file(filepath);

    if (!file.is_open()) {
        qDebug() << "[IniParser] Erreur : Impossible d'ouvrir le fichier : " << filepath;
        return sections;
    }

    string line;
    string accumulatedComments;
    SectionConfig* currentSection = nullptr;

    while (getline(file, line)) {
        string trimmedLine = trim(line);

        // 1. Ligne vide -> On réinitialise la description accumulée
        if (trimmedLine.empty()) {
            accumulatedComments.clear();
            continue;
        }

        // 2. Ligne de commentaire (commence par ';' ou '#')
        if (trimmedLine[0] == ';' || trimmedLine[0] == '#') {
            string commentContent = trim(trimmedLine.substr(1));
            if (!accumulatedComments.empty()) {
                accumulatedComments += " "; // Séparateur si commentaire multi-lignes
            }
            accumulatedComments += commentContent;
            continue;
        }

        // 3. Déclaration d'une section [NomDeLaSection]
        if (trimmedLine.front() == '[' && trimmedLine.back() == ']') {
            string sectionName = trim(trimmedLine.substr(1, trimmedLine.size() - 2));
            sections.push_back({sectionName, {}});
            currentSection = &sections.back();
            accumulatedComments.clear(); // Reset des commentaires pour la section
            continue;
        }

        // 4. Déclaration d'une variable (cle = valeur)
        size_t delimiterPos = trimmedLine.find('=');
        if (delimiterPos != string::npos && currentSection != nullptr) {
            string key = trim(trimmedLine.substr(0, delimiterPos));
            string value = trim(trimmedLine.substr(delimiterPos + 1));

            VariableConfig var;
            var.nom = key;
            var.valeur = value;
            var.description = accumulatedComments; // Assigne la description collectée

            currentSection->variables.push_back(var);

            // Consommé : Réinitialisation pour la prochaine variable
            accumulatedComments.clear();
        }
    }
    file.close();
    return sections;
}

vector<SectionConfig> IniParser::parseFromString(std::string_view content) {
    vector<SectionConfig> sections;

    // On crée un flux de lecture à partir de la vue mémoire
    std::istringstream file{std::string(content)};

    string line;
    string accumulatedComments;
    SectionConfig* currentSection = nullptr;

    while (getline(file, line)) {
        string trimmedLine = trim(line);

        // 1. Ligne vide -> On réinitialise la description accumulée
        if (trimmedLine.empty()) {
            accumulatedComments.clear();
            continue;
        }

        // 2. Ligne de commentaire (commence par ';' ou '#')
        if (trimmedLine[0] == ';' || trimmedLine[0] == '#') {
            string commentContent = trim(trimmedLine.substr(1));
            if (!accumulatedComments.empty()) {
                accumulatedComments += " "; // Séparateur si commentaire multi-lignes
            }
            accumulatedComments += commentContent;
            continue;
        }

        // 3. Déclaration d'une section [NomDeLaSection]
        if (trimmedLine.front() == '[' && trimmedLine.back() == ']') {
            string sectionName = trim(trimmedLine.substr(1, trimmedLine.size() - 2));
            sections.push_back({sectionName, {}});
            currentSection = &sections.back();
            accumulatedComments.clear(); // Reset des commentaires pour la section
            continue;
        }

        // 4. Déclaration d'une variable (cle = valeur)
        size_t delimiterPos = trimmedLine.find('=');
        if (delimiterPos != string::npos && currentSection != nullptr) {
            string key = trim(trimmedLine.substr(0, delimiterPos));
            string value = trim(trimmedLine.substr(delimiterPos + 1));

            VariableConfig var;
            var.nom = key;
            var.valeur = value;
            var.description = accumulatedComments; // Assigne la description collectée

            currentSection->variables.push_back(var);

            // Consommé : Réinitialisation pour la prochaine variable
            accumulatedComments.clear();
        }
    }
    return sections;
}

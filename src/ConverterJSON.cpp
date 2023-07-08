
#include "ConverterJSON.h"

// Читаем файл настроек > nlohmann::json config;
nlohmann::json ConverterJSON::readConfig() const {

    nlohmann::json config;
    try {
        std::ifstream file("config.json");
        if (!file) {
            throw std::runtime_error("Unable to open config.json\n");
        }
        file >> config;
        file.close();
        if (config["config"].is_null()) {
            throw std::runtime_error("Config is_null\n");
        }
    }

    catch (const std::runtime_error& ex) {
        std::cerr << "\n" << SearchServerName << " - " << ex.what();
        exit(-1);
    }
    catch (const nlohmann::json::exception ex) {
            std::cerr << "Wrong file config.json:" << "\n";
            std::cerr << ex.what() << "\n";
            exit(-2);
    }

    return (config);
}


int ConverterJSON::GetResponsesLimit() {
    auto config = readConfig();       // читаем файл настроек
    if (config["config"]["maxResponses"].is_null())
        std::cerr <<"Config maxResponses is_null, use default\n";
    else
        responsesLimit = config["config"]["maxResponses"];   // устанавливаем значение поля класса

    return responsesLimit;
}

// Загрузка всех настроек
void ConverterJSON::GetConfig() {

    GetResponsesLimit();
    auto config = readConfig();       // читаем файл настроек

    if (config["config"]["version"].is_null())
        std::cout << "Config version is_null, current version - " << CurrentVersion << "\n";
    else {
        version = config["config"]["version"];
        if(version != CurrentVersion)
            std::cout << "Config version - " <<  version << ", current version - " << CurrentVersion << "\n";
    }

    if (config["config"]["name"].is_null())
        std::cerr << "Config name is_null, use SearchServer default\n";
    else
        SearchServerName = config["config"]["name"];

    files.clear();
    std::string fp = config["files"][0];  // первая строка config["files"] может быть папкой
    if (fp[fp.size()-1] != '/')
        for (auto& file : config["files"])
            files.push_back(file);
    else {     // read files from folder
        std::ofstream f("files.txt");
        for (auto& p : std::filesystem::recursive_directory_iterator(fp)) {
            if (p.is_regular_file()) {
                files.push_back(p.path().generic_string());
                f << (p.path().generic_string()) << "\n";
            }
        }
        f.close();
    }
}

// Чтение файла requests
std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream file("requests.json");
    if (!file)
        throw std::runtime_error("Unable to open requests.json\n");

    std::vector<std::string> queries;
    nlohmann::json requests;
    // Проверям requests.json на валидность - исключения nlohmann::json
    file >> requests;
    if (requests["requests"].is_null())
        throw std::invalid_argument("Requests is_null\n");
    // собираем запросы
    for (auto& request : requests["requests"])
        queries.push_back(request);

    file.close();
    return queries;
}

// Создание файла answers.json
void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> responses) const {
    // вспомогательные словари
    nlohmann::json fileAnswers;
    nlohmann::json answers;
    nlohmann::json relevance;
    char num[8]{};

    for (int i = 0; i < responses.size(); i++) {  // по запросам
        nlohmann::json answer;  // словать ответа по текущему запросу
        answer["result"] = responses.at(i).size() ? "true" : "false";

        if (responses.at(i).size() == 1) {
            answer["docid"] = responses.at(i).at(0).docId;
            answer["rank"] = (float)responses.at(i).at(0).rank;
        }

        if (responses.at(i).size() > 1) {
            std::vector<nlohmann::json> vectorRels;  // для накопления relevance
            // по документам
            for (unsigned j = 0; j < responses.at(i).size();j++) {
                relevance["docid"] = responses.at(i).at(j).docId ;  //
                sprintf_s(num, "%1.3f", responses.at(i).at(j).rank);   // убираем огрехи float json
                relevance["rank"] = (double)atof(num);

                vectorRels.push_back(relevance);
            }
            nlohmann::json relevances(vectorRels);
            answer["relevance"] = relevances;
        }
        sprintf_s(num, "%03d", i + 1);
        answers["request" + std::string(num)] = answer;
    }

    // to file answers.json
    fileAnswers["answers"] = answers;
    try {
        std::ofstream file("answers.json");   // Попытка создания файла answers.json
        if (!file)
            throw std::runtime_error("Unable to create answers.json\n");
        try {
            file << std::setfill(' ') << std::setw(4)  << fileAnswers;                // Попытка записи в файл answers.json
            file.close();
        }
        catch (const nlohmann::json::exception ex) {
            std::cerr << ex.what() << "\n";
        }
    }

    catch (const std::runtime_error& ex) {
        std::cerr << "\n" << SearchServerName << " - " << ex.what();
    }
}




#pragma once

#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <cmath>
#include <stdexcept>
#include <filesystem>

#define CurrentVersion "2.1"

    /**
    * Структура для хранения относительной релевантности документа
    */
struct RelativeIndex {
    size_t docId;
    float rank;   // Относительная релевантность
    bool operator ==(const RelativeIndex& other) const {
        return (docId == other.docId && rank == other.rank);
    }
};

    /**
    * Класс для работы с JSON-файлами
    */
class ConverterJSON {
    int responsesLimit=5;
    std::string version;

public:
    std::string SearchServerName = "SearchServer";
    std::vector<std::string> files;    // пути к документам

    ConverterJSON() = default;

    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * @return  responsesLimit
    */
    int GetResponsesLimit();
    // get value responsesLimit from memory or read from file
    int GetResponsesLimit(bool read) {
        if(!read)
            return responsesLimit;
        return GetResponsesLimit();
    };
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();
    /**
    * Помещает в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<RelativeIndex>> answers) const;
    /**
    * Получает информацию из файла конфигурации (files, responsesLimit ...)
    */
    void GetConfig();
    [[nodiscard]]
    nlohmann::json readConfig() const; // читаем json из файла конфигурации
    /**
    * Конструктор, инициализирующий все параметры
    */
    explicit ConverterJSON (bool full) {
        GetConfig();
    }
};


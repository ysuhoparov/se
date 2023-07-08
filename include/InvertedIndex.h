#pragma once
#include <iostream>
#include <sstream>
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


    /**  Cтруктура для хранения частоты слов, встречаемых в тексте
    doc_id — идентификатор файла документа;
    count — число, которое обозначает, сколько раз слово встретилось в документе .
    */
struct Entry {
    size_t docId, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
};

    /**
    * Инвертированный индекс это частотый словать -
    * Слово: идентификатор документа, число повторений слова.
    * Служит для быстрого поиска
    */

class InvertedIndex {
    std::vector<std::string> * docs;                 // pointer на список файлов
    std::map<std::string, std::vector<Entry>> freqDictionary; // частотный словарь

    std::mutex fileMutex;
    std::mutex freqMutex;
    std::mutex screenMutex;

    size_t head{ 0 };   // указатедь(относительный) на загружаемые файлы

public:
    InvertedIndex() = default;
    //Если  мьютексы внести в класс, то необходимо:
    InvertedIndex(InvertedIndex& other) {
        this->freqDictionary = other.freqDictionary; // частотный словарь
        //other.freqDictionary.empty();
    };
//

    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> * files);

    /** Обновить или заполнить базу документов для одного файла */
    void UpdateDocument(size_t docId);
    /** создание потоков индексирования текста документов */
    void makeThreads();

    /**
    * Метод определяет количество вхождений слова word в загруженной базе документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    [[nodiscard]]
    std::vector<Entry> GetWordCount(const std::string& word) const;
};

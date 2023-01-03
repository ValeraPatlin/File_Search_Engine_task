#pragma once
#include "Header.h"

struct Entry {
	size_t doc_id, count;
	// ƒанный оператор необходим дл€ проведени€ тестовых сценариев
	bool operator ==(const Entry& other) const 
	{
		return (doc_id == other.doc_id &&
			count == other.count);
	}
};

class InvertedIndex
{
public:
	InvertedIndex();
	/**
	* ќбновить или заполнить базу документов, по которой будем совершать
	поиск
	* @param texts_input содержимое документов
	*/
	void UpdateDocumentBase(std::vector<std::string> input_docs);
	/**
	* ћетод определ€ет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	std::vector<Entry> GetWordCount(const std::string& word);
	size_t get_word_count(const std::string& word, const size_t doc_id) const;

private:
	bool indexing_ongoing = false;
	std::mutex mutex_index;

	std::vector<std::string> docs; // список содержимого документов
	std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь

	void index_file(const std::string& fileContent, size_t docId);
};


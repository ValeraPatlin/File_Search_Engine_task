#pragma once
#include "Header.h"

struct RelativeIndex {
	size_t doc_id;
	float rank;
	size_t absolute_index = 0;

	bool operator ==(const RelativeIndex& other) const {
		return (doc_id == other.doc_id && rank == other.rank);
	}

	bool operator > (const RelativeIndex& other) const
	{
		return (rank > other.rank || (rank == other.rank && doc_id < other.doc_id));
	}

};

class SearchServer {
public:
	/**
	* @param idx � ����������� ������ ��������� ������ �� �����
	InvertedIndex,
	* ����� SearchServer ��� ������ ������� ���� ����������� �
	�������
	*/
	SearchServer(InvertedIndex& idx) : _index(idx) { };
	/**
	* ����� ��������� ��������� ��������
	* @param queries_input ��������� ������� ������ �� �����
	requests.json
	* @return ���������� ��������������� ������ ����������� ������� ���
	�������� ��������
	*/
	std::vector<std::vector<RelativeIndex>> search
	(const std::vector<std::string>& queries_input);
private:
	InvertedIndex& _index;

    const int max_answer = 5;

    std::set<std::string> get_uniqu_words(const std::string& request);  
    std::vector<std::pair<std::string, size_t>> get_words_entries(const std::set<std::string>& words);
    std::vector<size_t> get_documents_words(const std::vector<std::pair<std::string, size_t>>& words); 
    size_t get_relevance_document(size_t docId, std::set<std::string>& unique_words);
};

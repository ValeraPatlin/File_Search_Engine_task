#include "Header.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search
(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<RelativeIndex>> result;

    if (queries_input.empty())
    {
        cout << "Error\n";
        return result;
    }

    for (const auto& query : queries_input)
    {
        std::set<std::string> unique_words = get_uniqu_words(query);

        if (unique_words.empty())
        {
            cout << "\nError! Bad request.\n";
            continue;
        }
        
        auto words_entries = get_words_entries(unique_words);

        std::sort(words_entries.begin(), words_entries.end(), 
            [](auto& left, auto& right)
            {
                return left.second < right.second;
            });

        auto document_ids_vec = get_documents_words(words_entries);

        std::vector<RelativeIndex> relative_indexes;

        size_t max_relevance = 0;

        for (const auto& docId : document_ids_vec)
        {
            size_t relevance_doc = get_relevance_document(docId, unique_words);

            RelativeIndex relative_index;

            relative_index.doc_id = docId;

            relative_index.absolute_index = relevance_doc;

            relative_indexes.push_back(relative_index);

            if (relevance_doc > max_relevance)
            {
                max_relevance = relevance_doc;
            }
        }

        for (auto& relative_index_it : relative_indexes)
        {
            if (max_relevance != 0)
            {
                float rank = static_cast<float>(relative_index_it.absolute_index) /
                    static_cast<float>(max_relevance);
                
                int rounded = static_cast<int>(std::round(rank * 100));

                rank = static_cast<float>(rounded / 100);

                relative_index_it.rank = rank;
            }
            else
            {
                relative_index_it.rank = 0;
            }
        }

        std::sort(relative_indexes.begin(), relative_indexes.end(),
            [&relative_indexes](RelativeIndex& left, RelativeIndex& right)
            {
                return (left.rank > right.rank 
                || (left.rank == right.rank && left.doc_id < right.doc_id));
            });

        if (relative_indexes.size() > max_answer)
        {
            relative_indexes.erase(relative_indexes.begin() + max_answer, relative_indexes.end());
        }

        result.push_back(relative_indexes);
    }
    return result;
}

std::set<std::string> SearchServer::get_uniqu_words(const std::string& request)
{
    std::set<std::string> result;

    std::stringstream ss(request);

    for (std::string word; ss >> word; )
    {
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char ch) 
            { 
                return std::tolower(ch); 
            });

        result.emplace(word);
    }
    return result;
}

std::vector<std::pair<std::string, size_t>> 
SearchServer::get_words_entries(const std::set<std::string>& words)
{
    std::vector<std::pair<std::string, size_t>> result;

    for (const auto& word : words)
    {
        auto word_entries = _index.GetWordCount(word);

        size_t word_sum_entries = 0;

        for (auto word_it : word_entries)
        {
            word_sum_entries += word_it.count;
        }

        std::pair<std::string, size_t> pair_word_entry;
        pair_word_entry.first = word;
        pair_word_entry.second = word_sum_entries;
        result.push_back(pair_word_entry);
    }
    return result;
}

std::vector<size_t> SearchServer::get_documents_words
(const std::vector<std::pair<std::string, size_t>>& words)
{
    std::vector<size_t> result; 

    for (const auto& word_it : words)
    {
        auto entries = _index.GetWordCount(word_it.first);

        for (auto entry : entries)
        {
            result.push_back(entry.doc_id);
        }   
    }

    std::sort(result.begin(), result.end());

    return result;
}

size_t SearchServer::get_relevance_document(size_t docId, std::set<std::string>& unique_words)
{
    size_t result = 0;

    for (const auto& word : unique_words)
    {
        size_t count_word = _index.get_word_count(word, docId);
        result += count_word;
    }
    return result;
}



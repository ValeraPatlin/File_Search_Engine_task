#include "Header.h"

InvertedIndex::InvertedIndex() = default;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    if (input_docs.empty())
    {
        cout << "Error, No Data\n";
        return;
    }
    indexing_ongoing = true;
    size_t docId = 0;

    freq_dictionary.clear();
    
    for (const auto& content : input_docs)
    {
        std::thread index([this, &content, docId]() 
            {
                index_file(content, docId);
            });
        ++docId;
        index.join();
    }
    indexing_ongoing = false;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
    if (indexing_ongoing)
    {
        cout << "\nError\n";
        return {};
    }

    auto it = freq_dictionary.find(word);

    if (it != freq_dictionary.end())
    {
        return it->second;
    }
    else
    {
        return {};
    }
}

size_t InvertedIndex::get_word_count(const std::string& word, const size_t doc_id) const
{
    if (indexing_ongoing) {
        cout << "Error\n";
        return 0;
    }
    size_t count = 0;

    auto it = freq_dictionary.find(word);

    if (it != freq_dictionary.end())
    {
        auto entryVector = it->second;

        for (auto entry : entryVector)
        {
            if (entry.doc_id == doc_id) count += entry.count;
        }
    }
    else {
        cout << "Error!\n Word: \"" << word << "\" not found.\n";
    }
    return count;
}

void InvertedIndex::index_file(const std::string& file_content, size_t id)
{
    std::map<std::string, Entry> dictionary;

    Entry entry{};
    entry.doc_id = id;
    entry.count = 1;

    std::stringstream ss(file_content);

    for (std::string word; ss >> word; )
    {
        std::transform(word.begin(), word.end(),
            word.begin(),
            [](unsigned char c) 
            { 
                return std::tolower(c); 
            });

        std::pair<std::string, Entry> file_word_frequency
        { 
            word, entry 
        };

        if (!dictionary.emplace(file_word_frequency).second)
        {
            dictionary.find(word)->second.count += 1;
        }
    }
    mutex_index.lock();

    for (const auto& word_it : dictionary)
    {
        std::pair<std::string, std::vector<Entry>> word_count;
        word_count.first = word_it.first;

        std::vector<Entry> entry_vec
        { 
            word_it.second
        };

        word_count.second = entry_vec;

        if (!freq_dictionary.emplace(word_count).second)
        {
            freq_dictionary.find(word_count.first)->second.push_back(word_it.second);
        }
    }
    mutex_index.unlock();
}

#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

template <typename Term, typename Doc>
class InvertedIndex {
    private:
        std::unordered_map<Term, std::vector<Doc>> index;

        std::vector<Doc> intersect(const std::vector<Doc>& a, const std::vector<Doc>& b);

        std::vector<Doc> set_union(const std::vector<Doc>& a, const std::vector<Doc>& b);
        
        const std::vector<Doc>& get_postings(const Term& term) const;

        void add_posting(const Term& term, Doc doc_id);
        
    public:
        
        void add_document(Doc& doc_id, const std::vector<Term>& terms);

        void finalise_index();

        std::vector<Doc> search_and(const std::vector<Term>& terms) const;

        std::vector<Doc> search_or(const std::vector<Term>& terms) const;

};


/**
* Given two lists of DocIDs, returns their intersection.
*/
template <typename Term, typename Doc>
std::vector<Doc> InvertedIndex<Term, Doc>::intersect(const std::vector<Doc>& a, const std::vector<Doc>& b) {
    std::vector<Doc> result;
            
    auto itr1 = a.begin();
    auto itr2 = b.begin();

    while (itr1 != a.end() && itr2 != b.end()) {
        if (*itr1 == *itr2) {
            result.push_back(*itr1);
            itr1++;
            itr2++;
        }
        else if (*itr1 < *itr2) {
            itr1++;
        }
        else {
            itr2++;
        }
    }

    return result;
}

/**
 * Given two lists of DocIDs, returns their union.
*/
template <typename Term, typename Doc>
std::vector<Doc> InvertedIndex<Term, Doc>::set_union(const std::vector<Doc>& a, const std::vector<Doc>& b) {
    std::vector<Doc> result;

    auto itr1 = a.begin();
    auto itr2 = b.begin();
            
    while (itr1 != a.end() && itr2 != b.end()) {
        if (*itr1 < *itr2) {
            result.push_back(*itr1);
            itr1++;
        }
        else if (*itr2 < *itr1) {
            result.push_back(*itr2);
            itr2++;
        }
        else {
            result.push_back(*itr1);
            itr1++;
            itr2++;
        }
    }
    
    while (itr1 != a.end()) {
        result.push_back(*itr1);
        itr1++;
    }
    
    while (itr2 != b.end()) {
        result.push_back(*itr2);
        itr2++;
    }

    return result;
}

/**
 * Returns a postings list for a given term.
 */
template <typename Term, typename Doc>
const std::vector<Doc>& InvertedIndex<Term, Doc>::get_postings(const Term& term) const {
    return index.at(term);
}

/**
 * Adds a posting 
 */
template <typename Term, typename Doc>
void InvertedIndex<Term, Doc>::add_posting(const Term& term, Doc doc_id) {
    index[term].push_back(doc_id);
}

/**
 * Adds a document to the index.
 */
template <typename Term, typename Doc>
void InvertedIndex<Term, Doc>::add_document(Doc& doc_id, const std::vector<Term>& terms) {
    for (const Term& term : terms) {
        add_posting(term, doc_id);
    }
}


/**
 * Sorts all postings lists and removes duplicates. Call after all documents are added.
 */
template <typename Term, typename Doc>
void InvertedIndex<Term, Doc>::finalise_index() {
    for (auto& pair : index) {
        std::vector<Doc>& postings = pair.second;
        
        std::sort(postings.begin(), postings.end());
        
        auto last = std::unique(postings.begin(), postings.end());
        postings.erase(last, postings.end());
    }
}


/**
 * Searches for DocIDs that contain all given terms (AND query).
 */
template <typename Term, typename Doc>
std::vector<Doc> InvertedIndex<Term, Doc>::search_and(const std::vector<Term>& terms) const {
    if (terms.empty()) {
        return {};
    }
    std::vector<Doc> result = get_postings(terms[0]);
    for (int i = 1; i < terms.size(); ++i) {
        std::vector<Doc>& postings_list = get_postings(terms[i]);
        
        result = intersect(result, postings_list);
    }
    return result;
}

/**
 * Searches for DocIDs that contain any given term (OR query).
 */
template <typename Term, typename Doc>
std::vector<Doc> InvertedIndex<Term, Doc>::search_or(const std::vector<Term>& terms) const {
    if (terms.empty()) {
        return {};
    }
    std::vector<Doc> result = get_postings(terms[0]);
    for (int i = 1; i < terms.size(); ++i) {
        std::vector<Doc>& postings_list = get_postings(terms[i]);
        result = set_union(result, postings_list);
    }
    return result;
}

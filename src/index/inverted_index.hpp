#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <set>

template <typename Term, typename Doc>
class InvertedIndex {
    private:
        std::unordered_map<Term, std::set<Doc>> index;

        /**
        * Given two lists of DocIDs, returns their intersection.
        */
        static std::set<Doc> intersect(const std::set<Doc>& a, const std::set<Doc>& b);

        /**
         * Given two lists of DocIDs, returns their union.
        */
        static std::set<Doc> set_union(const std::set<Doc>& a, const std::set<Doc>& b);
                
        /**
         * Returns a postings list for a given term.
         */
        const std::set<Doc>& get_postings(const Term& term) const;

        /**
         * Adds a posting 
         */
        void add_posting(const Term& term, Doc doc_id);
        
    public:
        
        /**
         * Adds a document to the index.
         */
        void add_document(Doc& doc_id, const std::vector<Term>& terms);

        /**
         * Searches for DocIDs that contain all given terms (AND query).
         */
        std::set<Doc> search_and(const std::vector<Term>& terms) const;

        /**
         * Searches for DocIDs that contain any given term (OR query).
         */
        std::set<Doc> search_or(const std::vector<Term>& terms) const;

};

template <typename Term, typename Doc>
std::set<Doc> InvertedIndex<Term, Doc>::intersect(const std::set<Doc>& a, const std::set<Doc>& b) {
    std::set<Doc> result;

    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(result, result.begin()));

    return result;
}

template <typename Term, typename Doc>
std::set<Doc> InvertedIndex<Term, Doc>::set_union(const std::set<Doc>& a, const std::set<Doc>& b) {
    std::set<Doc> result;

    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(result, result.begin()));

    return result;
}

template <typename Term, typename Doc>
const std::set<Doc>& InvertedIndex<Term, Doc>::get_postings(const Term& term) const {
    static const std::set<Doc> empty_set;
    
    auto itr = index.find(term);

    if (itr != index.end()) {
        return itr->second;
    }
    else {
        return empty_set;
    }
}

template <typename Term, typename Doc>
void InvertedIndex<Term, Doc>::add_posting(const Term& term, Doc doc_id) {
    index[term].insert(doc_id);
}

template <typename Term, typename Doc>
void InvertedIndex<Term, Doc>::add_document(Doc& doc_id, const std::vector<Term>& terms) {
    for (const Term& term : terms) {
        add_posting(term, doc_id);
    }
}

template <typename Term, typename Doc>
std::set<Doc> InvertedIndex<Term, Doc>::search_and(const std::vector<Term>& terms) const {
    if (terms.empty()) {
        return {};
    }
    
    std::set<Doc> result = get_postings(terms[0]);
        
    for (int i = 1; i < terms.size(); ++i) {
        const std::set<Doc>& postings_list = get_postings(terms[i]);
        result = intersect(result, postings_list);
    }

    return result;
}

template <typename Term, typename Doc>
std::set<Doc> InvertedIndex<Term, Doc>::search_or(const std::vector<Term>& terms) const {
    if (terms.empty()) {
        return {};
    }
    
    std::set<Doc> result = get_postings(terms[0]);

    for (int i = 1; i < terms.size(); ++i) {
        const std::set<Doc>& postings_list = get_postings(terms[i]);
        result = set_union(result, postings_list);
    }
    return result;
}

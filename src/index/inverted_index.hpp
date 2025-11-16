#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <set>
#include <optional>

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
        std::optional<const std::set<Doc>&> get_postings(const Term& term) const;

        /**
         * Adds a posting 
         */
        void add_posting(const Term& term, Doc doc_id);
        
    public:
        
        /**
         * Adds a document to the index.
         */
        template <class Self>
        void add_document(this Self& self, Doc& doc_id, const std::vector<Term>& terms);

        /**
         * Searches for DocIDs that contain all given terms (AND query).
         */
        template <class Self>
        std::set<Doc> search_and(this const Self& self, const std::vector<Term>& terms);

        /**
         * Searches for DocIDs that contain any given term (OR query).
         */
        template <class Self>
        std::set<Doc> search_or(this const Self& self, const std::vector<Term>& terms);

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
std::optional<const std::set<Doc>&> InvertedIndex<Term, Doc>::get_postings(const Term& term) const {
    auto itr = index.find(term);

    if (itr != index.end()) {
        return itr->second;
    }
    else {
        return std::nullopt;
    }
}

template <typename Term, typename Doc>
void InvertedIndex<Term, Doc>::add_posting(const Term& term, Doc doc_id) {
    index[term].insert(doc_id);
}

template <typename Term, typename Doc>
template <class Self>
void InvertedIndex<Term, Doc>::add_document(this Self& self, Doc& doc_id, const std::vector<Term>& terms) {
    for (const Term& term : terms) {
        self.add_posting(term, doc_id);
    }
}

template <typename Term, typename Doc>
template <class Self>
std::set<Doc> InvertedIndex<Term, Doc>::search_and(this const Self& self, const std::vector<Term>& terms) {
    if (terms.empty()) {
        return {};
    }
    
    std::set<Doc> result = self.get_postings(terms[0]).value_or({});
        
    for (int i = 1; i < terms.size(); ++i) {
        std::set<Doc> postings_list = self.get_postings(terms[i]).value_or({});
        result = InvertedIndex<Term, Doc>::intersect(result, postings_list);
    }

    return result;
}

template <typename Term, typename Doc>
template <class Self>
std::set<Doc> InvertedIndex<Term, Doc>::search_or(this const Self& self, const std::vector<Term>& terms) {
    if (terms.empty()) {
        return {};
    }
    
    std::set<Doc> result = self.get_postings(terms[0]).value_or({});
 
    for (int i = 1; i < terms.size(); ++i) {
        std::set<Doc> postings_list = self.get_postings(terms[i]).value_or({});
        result = InvertedIndex<Term, Doc>::set_union(result, postings_list);
    }
    return result;
}

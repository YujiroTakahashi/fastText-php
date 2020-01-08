#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <fasttext/fasttext.h>

namespace croco {

/**
 * CFastText
 *
 * @package     croco-fastText
 * @author      Yujiro Takahashi <yujiro@cro-co.co.jp>
 */
class CFastText : public fasttext::FastText {
    
public:
    std::vector<std::pair<fasttext::real, std::string>> getPredict(int32_t k, std::string word);
    std::vector<std::pair<fasttext::real, std::string>> getAnalogies(int32_t k, std::string word);
    int32_t getK(void);

private:
    std::vector<std::pair<int, std::string>> _parseQuery(std::string query);
}; // class CFastText

/**
 * getPredict
 *
 * @access public
 * @param  int32_t k
 * @param  std::string word
 * @return std::vector<std::pair<fasttext::real, std::string>>
 */
inline std::vector<std::pair<fasttext::real, std::string>> CFastText::getPredict(int32_t k, std::string word)
{
    if ('\n' != word.back()) {
        word.push_back('\n');
    }
    std::stringstream ioss;
    ioss.str(word);

    std::vector<int32_t> words, labels;
    dict_->getLine(ioss, words, labels);

    std::vector<std::pair<fasttext::real, int32_t>> predictions;
    fasttext::real threshold = 0.0;
    predict(k, words, predictions, threshold);

    std::vector<std::pair<fasttext::real, std::string>> result;
    for (const auto& p : predictions) {
        result.push_back(
            std::make_pair(
                std::exp(p.first), 
                dict_->getLabel(p.second)
            )
        );
    } // for (const auto& p : predictions)

    return result;
}

/**
 * getAnalogies
 *
 * @access public
 * @param  int32_t k
 * @param  const std::string word
 * @return std::vector<std::pair<fasttext::real, std::string>>
 */
inline std::vector<std::pair<fasttext::real, std::string>> CFastText::getAnalogies(int32_t k, std::string word)
{
    fasttext::Vector buffer(args_->dim), query(args_->dim);
    query.zero();

    std::vector<std::pair<int, std::string>> queries = _parseQuery(word);
    std::set<std::string> banSet;
    banSet.clear();
    for (auto &node : queries) {
        getWordVector(buffer, node.second);
        query.addVector(buffer, (1.0 * node.first) / (buffer.norm() + 1e-8));
        banSet.insert(node.second);
    }

    lazyComputeWordVectors();
    assert(wordVectors_);

    std::vector<std::pair<fasttext::real, std::string>> result;
    return getNN(*wordVectors_, query, k, banSet);
}

/**
 * getK
 *
 * @access public
 * @return int32_t
 */
inline int32_t CFastText::getK(void)
{
    int64_t total =
      (args_->model == fasttext::model_name::sup) ? dict_->nlabels() : dict_->nwords();

    float x = std::sqrt(total);
    return static_cast<int32_t>(x + 0.5f);
}

/**
 * parse a query format
 *
 * @access private
 * @param  std::string query
 * @return std::vector<std::pair<int, std::string>>
 */
inline std::vector<std::pair<int, std::string>> CFastText::_parseQuery(std::string query)
{
    std::vector<std::pair<int, std::string>> val;
    int sign = 1, left = 0, len=0;
    bool flag = true, step = true;

    val.clear();

    size_t maxlen = query.length();
    for (size_t idx=0; idx < maxlen; idx++) {
        switch (query[idx]) {
            case ' ':
                if (flag) {
                    len = idx - left;
                    val.push_back(
                        std::make_pair(
                            sign,
                            query.substr(left, len)
                        )
                    );
                    flag = false;
                    step = true;
                } // if (flag)
                break;
            case '+':
                sign = 1;
                break;
            case '-':
                sign = -1;
                break;
            default:
                flag = true;
                if (step) {
                    left = idx;
                    step = false;
                }
                break;
        }
    }
    val.push_back(
        std::make_pair(sign, query.substr(left))
    );
    return val;
}

} // namespace croco
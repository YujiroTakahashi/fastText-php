#include <iostream>
#include <queue>
#include <iterator>
#include <sstream>
#include <vector>

#include <string.h>

#include <fasttext/args.h>
#include <fasttext/fasttext.h>

#include "json.hpp"
#include "fasttext_api.h"

using namespace fasttext;

std::vector<std::pair<int, std::string>> _parseQuery(std::string query);

/**
 * get fastText version
 *
 * @access public
 * @return int
 */
int FastTextVersion()
{
    return FASTTEXT_VERSION;
}

/**
 * get fastText size
 *
 * @access public
 * @return int
 */
int FastTextSize()
{
    return sizeof(FastText);
}

/**
 * create a FastText handle
 *
 * @access public
 * @return FastTextHandle
 */
FastTextHandle FastTextCreate()
{
    FastTextHandle handle = new FastText;
    return handle;
}

/**
 * free a FastText handle
 *
 * @access public
 * @param  FastTextHandle handle
 * @return void
 */
void FastTextFree(FastTextHandle handle)
{
    delete static_cast<FastText*>(handle);
}

/**
 * free a FTStr handle
 *
 * @access public
 * @param  FTStr handle
 * @return void
 */
void FastTextStrFree(FTStr handle)
{
    if (nullptr != handle->buff) {
        delete[] handle->buff;
    }

    if (nullptr != handle) {
        delete handle;
    }
}

/**
 * fasttext.cc::loadModel
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* filename
 * @return int
 */
int FastTextLoadModel(FastTextHandle handle, const char *filename)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    try {
        fasttext->loadModel(filename);
    } catch (const std::invalid_argument& e) {
        return FASTTEXT_FALSE;
    }
    return FASTTEXT_TRUE;
}

/**
 * dictionary.cc::nwords
 *
 * @access public
 * @param  FastTextHandle handle
 * @return int32_t
 */
int32_t FastTextWordRows(FastTextHandle handle)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();

    return dict->nwords();
}

/**
 * dictionary.cc::nlabels
 *
 * @access public
 * @param  FastTextHandle handle
 * @return int32_t
 */
int32_t FastTextLabelRows(FastTextHandle handle)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();

    return dict->nlabels();
}

/**
 * fasttext.cc::getWordId
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @return int32_t
 */
int32_t FastTextWordId(FastTextHandle handle, const char* word)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::string text = std::string(word);

    return fasttext->getWordId(text);
}

/**
 * fasttext.cc::getSubwordId
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @return int32_t
 */
int32_t FastTextSubwordId(FastTextHandle handle, const char* word)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::string text = std::string(word);

    return fasttext->getSubwordId(text);
}

/**
 * dictionary.cc::getWord
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  conint32_tst wordId
 * @return FTStr
 */
FTStr FastTextGetWord(FastTextHandle handle, int32_t wordId)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();
    std::string word = dict->getWord(wordId);

    return FTStrVal(word);
}

/**
 * dictionary.cc::getLabel
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  conint32_tst wordId
 * @return FTStr
 */
FTStr FastTextGetLabel(FastTextHandle handle, int32_t labelId)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();
    std::string label = dict->getLabel(labelId);

    return FTStrVal(label);
}

/**
 * main.cc::predict
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @param  const int k
 * @return FTStr
 */
FTStr FastTextPredict(FastTextHandle handle, const char* word, const int k)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();

    std::string text = std::string(word);
    std::stringstream ioss;

    if ('\n' != text.back()) {
        text.push_back('\n');
    }
    ioss.str(text);

    nlohmann::json retj;
    std::vector<int32_t> words, labels;
    std::vector<std::pair<real, int32_t>> predictions;
    std::vector<std::pair<fasttext::real, std::string>> results;
    real threshold = 0.0;

    try {
        dict->getLine(ioss, words, labels);
        fasttext->predict(k, words, predictions, threshold);
        int idx = 0;
        for (auto &p : predictions) {
            retj[idx]["prob"] = std::exp(p.first);
            retj[idx]["label"] = dict->getLabel(p.second);
            idx++;
        }
    } catch (const std::invalid_argument& e) {
        retj["is_error"] = FASTTEXT_TRUE;
        retj["error"] = e.what();
    }

    return FTStrVal(retj.dump());
}

/**
 * main.cc::printWordVectors
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @return FTStr
 */
FTStr FastTextWordVectors(FastTextHandle handle, const char* word)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::string text = std::string(word);

    Vector vec(fasttext->getDimension());
    fasttext->getWordVector(vec, text);

    nlohmann::json retj;
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        retj[idx] = vec[idx];
    }

    return FTStrVal(retj.dump());
}

/**
 * main.cc::printWordVectors
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @return FTStr
 */
FTStr FastTextSubwordVector(FastTextHandle handle, const char* word)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::string text = std::string(word);

    Vector vec(fasttext->getDimension());
    fasttext->getSubwordVector(vec, text);

    nlohmann::json retj;
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        retj[idx] = vec[idx];
    }

    return FTStrVal(retj.dump());
}

/**
 * main.cc::printSentenceVectors
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @return FTStr
 */
FTStr FastTextSentenceVectors(FastTextHandle handle, const char* word)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::string text = std::string(word);
    std::stringstream ioss;
    std::copy(text.begin(), text.end(), std::ostream_iterator<char>(ioss));

    Vector vec(fasttext->getDimension());
    fasttext->getSentenceVector(ioss, vec);

    nlohmann::json retj;
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        retj[idx] = vec[idx];
    }

    return FTStrVal(retj.dump());
}

/**
 * main.cc::nn
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @param  const int k
 * @return FTStr
 */
FTStr FastTextNN(FastTextHandle handle, const char* word, const int k)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();


    std::string query = std::string(word);
    std::vector<std::pair<fasttext::real, std::string>> results;
    nlohmann::json retj;

    try {
        results = fasttext->getNN(query, k);
        int idx = 0;
        for (auto &value : results) {
            retj[idx]["prob"] = value.first;
            retj[idx]["label"] = value.second;
            idx++;
        }
    } catch (const std::invalid_argument& e) {
        retj["is_error"] = FASTTEXT_TRUE;
        retj["error"] = e.what();
    }

    return FTStrVal(retj.dump());
}

/**
 * main.cc::analogies
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @param  const int k
 * @return FTStr
 */
FTStr FastTextAnalogies(FastTextHandle handle, const char* word, const int k)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::shared_ptr<const Dictionary> dict = fasttext->getDictionary();

    Vector buffer(fasttext->getDimension()), query(fasttext->getDimension());
    Matrix wordVectors(dict->nwords(), fasttext->getDimension());

    std::stringbuf buf;
    std::streambuf *prev = std::cerr.rdbuf(&buf);
    fasttext->precomputeWordVectors(wordVectors);
    std::cerr.rdbuf(prev);

    std::set<std::string> banSet;
    banSet.clear();
    query.zero();

    std::vector<std::pair<int, std::string>> queries = _parseQuery(std::string(word));
    for (auto &n : queries) {
        banSet.insert(n.second);
        fasttext->getWordVector(buffer, n.second);
        query.addVector(buffer, 1.0 * n.first);
    }

    nlohmann::json retj;
    std::vector<std::pair<fasttext::real, std::string>> results;
    try {
        fasttext->findNN(wordVectors, query, k, banSet, results);
        int idx = 0;
        for (auto &value : results) {
            retj[idx]["prob"] = value.first;
            retj[idx]["label"] = value.second;
            idx++;
        }
    } catch (const std::invalid_argument& e) {
        retj["is_error"] = FASTTEXT_TRUE;
        retj["error"] = e.what();
    }
    return FTStrVal(retj.dump());
}

/**
 * main.cc::printNgrams
 *
 * @access public
 * @param  FastTextHandle handle
 * @param  const char* word
 * @return FTStr
 */
FTStr FastTextNgramVectors(FastTextHandle handle, const char* word)
{
    FastText *fasttext = static_cast<FastText*>(handle);
    std::vector<std::pair<std::string, Vector>> ngramVectors;
    nlohmann::json retj;

    try {
        ngramVectors = fasttext->getNgramVectors(std::string(word));

        int idx = 0;
        for (const auto& value : ngramVectors) {
            retj[idx]["word"] = value.first;
            for (int64_t _idx = 0; _idx < value.second.size(); _idx++) {
                retj[idx]["vector"][_idx] = value.second[_idx];
            }
            idx++;
        }
    } catch (const std::invalid_argument& e) {
        retj["is_error"] = FASTTEXT_TRUE;
        retj["error"] = e.what();
    }

    return FTStrVal(retj.dump());
}

/**
 * parse a query format
 *
 * @access private
 * @param  std::string query
 * @return std::vector<std::pair<int, std::string>>
 */
std::vector<std::pair<int, std::string>> _parseQuery(std::string query)
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
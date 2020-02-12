#include "ftext.h"

/* {{{ proto void fasttext::__construct()
 */
PHP_METHOD(fasttext, __construct)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *address;
    size_t address_len;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);

    croco::CFastText *fasttext = new croco::CFastText();
    ft_obj->handle = static_cast<FastTextHandle>(fasttext);
}
/* }}} */

/* {{{ proto void fasttext::__destruct()
 */
PHP_METHOD(fasttext, __destruct)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);

    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);
    delete fasttext;
}
/* }}} */

/* {{{ proto string fasttext::getError()
 */
PHP_METHOD(fasttext, getError)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);

    RETURN_ZVAL(&ft_obj->error, 1, 0);
}
/* }}} */

/* {{{ proto long fasttext::load(String filename)
 */
PHP_METHOD(fasttext, load)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *model;
    size_t model_len;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &model, &model_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
         fasttext->loadModel(std::string(model));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto long fasttext::getWordRows()
 */
PHP_METHOD(fasttext, getWordRows)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    zend_long rows;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
        std::shared_ptr<const fasttext::Dictionary> dict = fasttext->getDictionary();
        rows = dict->nwords();
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    RETURN_LONG(rows);
}
/* }}} */

/* {{{ proto long fasttext::getWordRows()
 */
PHP_METHOD(fasttext, getLabelRows)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    zend_long rows;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
        std::shared_ptr<const fasttext::Dictionary> dict = fasttext->getDictionary();
        rows = dict->nlabels();
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    RETURN_LONG(rows);
}
/* }}} */

/* {{{ proto long fasttext::getWordId(String word)
 */
PHP_METHOD(fasttext, getWordId)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;
    zend_long id;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
        id = fasttext->getWordId(std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    RETURN_LONG(id);
}
/* }}} */

/* {{{ proto long fasttext::getSubwordId(String word)
 */
PHP_METHOD(fasttext, getSubwordId)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;
    zend_long id;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
        id = fasttext->getSubwordId(std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    RETURN_LONG(id);
}
/* }}} */

/* {{{ proto mixed fasttext::getWord(int id])
 */
PHP_METHOD(fasttext, getWord)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    zend_long id;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
        std::shared_ptr<const fasttext::Dictionary> dict = fasttext->getDictionary();
        std::string word = dict->getWord(id);
        ZVAL_STRING(return_value, word.c_str());
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getLabel(int id])
 */
PHP_METHOD(fasttext, getLabel)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    zend_long id;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    try {
        std::shared_ptr<const fasttext::Dictionary> dict = fasttext->getDictionary();
        std::string label = dict->getLabel(id);
        ZVAL_STRING(return_value, label.c_str());
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getWordVectors(String word)
 */
PHP_METHOD(fasttext, getWordVectors)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    fasttext::Vector vec(fasttext->getDimension());
    try {
        fasttext->getWordVector(vec, std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        std::stringstream svec("");
        svec << vec[idx];

        zval rowVal;
        ZVAL_DOUBLE(&rowVal, std::stof(svec.str()));
        add_index_zval(return_value, idx, &rowVal);
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getSubwordVector(String word)
 */
PHP_METHOD(fasttext, getSubwordVector)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    fasttext::Vector vec(fasttext->getDimension());
    try {
        fasttext->getSubwordVector(vec, std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        std::stringstream svec("");
        svec << vec[idx];

        zval rowVal;
        ZVAL_DOUBLE(&rowVal, std::stof(svec.str()));
        add_index_zval(return_value, idx, &rowVal);
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getSentenceVectors(String sentence)
 */
PHP_METHOD(fasttext, getSentenceVectors)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *sentence;
    size_t sentence_len;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &sentence, &sentence_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    std::stringbuf strBuf(sentence);
    std::istream istream(&strBuf); 
    fasttext::Vector vec(fasttext->getDimension());

    try {
        fasttext->getSentenceVector(istream, vec);
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    for (int64_t idx = 0; idx < vec.size(); idx++) {
        std::stringstream svec("");
        svec << vec[idx];

        zval rowVal;
        ZVAL_DOUBLE(&rowVal, std::stof(svec.str()));
        add_index_zval(return_value, idx, &rowVal);
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getPredict(String word[, int k])
 */
PHP_METHOD(fasttext, getPredict)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;
    zend_long k = 0;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    std::vector<std::pair<fasttext::real, std::string>> result;
    try {
        if (0 >= k) {
            k = fasttext->getK();
        }
        result = fasttext->getPredict(k, std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : result) {
        zval rowVal, probVal, labelVal;
        array_init(&rowVal);
        ZVAL_DOUBLE(&probVal, std::exp(node.first));
        ZVAL_STRING(&labelVal, node.second.c_str());
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "prob", sizeof("prob")-1, &probVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "label", sizeof("label")-1, &labelVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getNgrams(String word)
 */
PHP_METHOD(fasttext, getNgrams)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    std::vector<std::pair<std::string, fasttext::Vector>> result;
    try {
        result = fasttext->getNgramVectors(std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : result) {
        zval rowVal, vecsVal, vecVal, wordVal;

        array_init(&vecsVal);
        for (int64_t vidx = 0; vidx < node.second.size(); vidx++) {
            zval vecVal;
            ZVAL_DOUBLE(&vecVal, node.second[vidx]);
            add_index_zval(&vecsVal, vidx, &vecVal);
        }
        ZVAL_STRING(&wordVal, node.first.c_str());

        array_init(&rowVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "vectors", sizeof("vectors")-1, &vecsVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "word", sizeof("word")-1, &wordVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */


/* {{{ proto mixed fasttext::getNN(String word[, int k])
 */
PHP_METHOD(fasttext, getNN)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;
    zend_long k = 0;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    std::vector<std::pair<fasttext::real, std::string>> result;
    try {
        if (0 >= k) {
            k = fasttext->getK();
        }
        result = fasttext->getNN(std::string(word), k);
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    zend_ulong idx = 0;
    for (auto &node : result) {
        zval rowVal, scoreVal, labelVal;

        array_init(&rowVal);
        ZVAL_DOUBLE(&scoreVal, node.first);
        ZVAL_STRING(&labelVal, node.second.c_str());
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "score", sizeof("score")-1, &scoreVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "label", sizeof("label")-1, &labelVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

/* {{{ proto mixed fasttext::getAnalogies(String word[, int k])
 */
PHP_METHOD(fasttext, getAnalogies)
{
    php_fasttext_object *ft_obj;
    zval *object = getThis();
    char *word;
    size_t word_len;
    zend_long k = 0;

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
        return;
    }

    ft_obj = Z_FASTTEXT_P(object);
    croco::CFastText *fasttext = static_cast<croco::CFastText*>(ft_obj->handle);

    std::vector<std::pair<fasttext::real, std::string>> result;
    try {
        if (0 >= k) {
            k = fasttext->getK();
        }

        result = fasttext->getAnalogies(k, std::string(word));
    } catch (std::exception& e) {
        ZVAL_STRING(&ft_obj->error, e.what());
        RETURN_FALSE;
    }

    array_init(return_value);
    zend_ulong idx = 0;

    for (auto &node : result) {
        zval rowVal, scoreVal, labelVal;
        array_init(&rowVal);
        ZVAL_DOUBLE(&scoreVal, node.first);
        ZVAL_STRING(&labelVal, node.second.c_str());
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "score", sizeof("score")-1, &scoreVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "label", sizeof("label")-1, &labelVal);

        add_index_zval(return_value, idx, &rowVal);
        idx++;
    }
}
/* }}} */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_fasttext.h"
#include "main/SAPI.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"

ZEND_DECLARE_MODULE_GLOBALS(fasttext)

/* {{{ PHP_INI
*/
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("fasttext.model_dir",  NULL, PHP_INI_SYSTEM, OnUpdateString, model_dir, zend_fasttext_globals, fasttext_globals)
PHP_INI_END()
/* }}} */

/* Handlers */
static zend_object_handlers fasttext_object_handlers;

/* Class entries */
zend_class_entry *php_fasttext_sc_entry;

/* {{{ proto void fasttext::__construct()
 */
PHP_METHOD(fasttext, __construct)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	ft_obj->fasttext = FastTextCreate();
}
/* }}} */

/* {{{ proto long fasttext::load(String filename)
 */
PHP_METHOD(fasttext, load)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *filename;
	size_t filename_len;
	zend_long res;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &filename, &filename_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	res = FastTextLoadModel(ft_obj->fasttext, filename);

	RETURN_LONG(res);
}
/* }}} */

/* {{{ proto long fasttext::getWordRows()
 */
PHP_METHOD(fasttext, getWordRows)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long res;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	res = FastTextWordRows(ft_obj->fasttext);

	RETURN_LONG(res);
}
/* }}} */

/* {{{ proto long fasttext::getWordRows()
 */
PHP_METHOD(fasttext, getLabelRows)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long res;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	res = FastTextLabelRows(ft_obj->fasttext);

	RETURN_LONG(res);
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
	id = (zend_long)FastTextWordId(ft_obj->fasttext, (const char*)word);

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
	id = (zend_long)FastTextSubwordId(ft_obj->fasttext, (const char*)word);

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
	FTStr vals;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	vals = FastTextGetWord(ft_obj->fasttext, (int32_t)id);

	ZVAL_STRING(return_value, vals->buff);
	FastTextStrFree(vals);
}
/* }}} */

/* {{{ proto mixed fasttext::getLabel(int id])
 */
PHP_METHOD(fasttext, getLabel)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	zend_long id;
	FTStr vals;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	vals = FastTextGetLabel(ft_obj->fasttext, (int32_t)id);

	ZVAL_STRING(return_value, vals->buff);
	FastTextStrFree(vals);
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
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextWordVectors(ft_obj->fasttext, (const char*)word);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
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
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextSubwordVector(ft_obj->fasttext, (const char*)word);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
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
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &sentence, &sentence_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextSubwordVector(ft_obj->fasttext, (const char*)sentence);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
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
	zend_long k = 10;
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextPredict(ft_obj->fasttext, (const char*)word, (const int)k);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
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
	zend_long k = 10;
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextNN(ft_obj->fasttext, (const char*)word, (const int)k);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
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
	zend_long k = 10;
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &word, &word_len, &k)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextAnalogies(ft_obj->fasttext, (const char*)word, (const int)k);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
}
/* }}} */

/* {{{ proto mixed fasttext::getNgramVectors(String word)
 */
PHP_METHOD(fasttext, getNgramVectors)
{
	php_fasttext_object *ft_obj;
	zval *object = getThis();
	char *word;
	size_t word_len;
	FTStr json;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &word, &word_len)) {
		return;
	}

	ft_obj = Z_FASTTEXT_P(object);
	json = FastTextNgramVectors(ft_obj->fasttext, (const char*)word);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	FastTextStrFree(json);
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_fasttext_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fasttext_load, 0, 0, 1)
	ZEND_ARG_INFO(0, fileformat)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fasttext_word, 0, 0, 1)
	ZEND_ARG_INFO(0, word)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fasttext_id, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fasttext_wordk, 0, 0, 1)
	ZEND_ARG_INFO(0, word)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()
/* }}} */


/* {{{ php_sfasttext_class_methods */
static zend_function_entry php_fasttext_class_methods[] = {
	PHP_ME(fasttext, __construct,		arginfo_fasttext_void,	ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(fasttext, load, 				arginfo_fasttext_load, 	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWordRows, 		arginfo_fasttext_void, 	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getLabelRows, 		arginfo_fasttext_void, 	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWordId, 		arginfo_fasttext_word, 	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getSubwordId, 		arginfo_fasttext_word,	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWord, 			arginfo_fasttext_id, 	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getLabel, 			arginfo_fasttext_id, 	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWordVectors, 	arginfo_fasttext_word,	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getSubwordVector, 	arginfo_fasttext_word,	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getSentenceVectors,arginfo_fasttext_word,	ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getPredict, 		arginfo_fasttext_wordk, ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getNN,				arginfo_fasttext_wordk, ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getAnalogies, 		arginfo_fasttext_wordk, ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getNgramVectors,	arginfo_fasttext_word,	ZEND_ACC_PUBLIC)

	PHP_FE_END
};
/* }}} */

static void php_fasttext_object_free_storage(zend_object *object) /* {{{ */
{
	php_fasttext_object *intern = php_fasttext_from_obj(object);

	if (!intern) {
		return;
	}

	if (intern->fasttext) {
		FastTextFree(intern->fasttext);
		intern->fasttext = NULL;
	}

	zend_object_std_dtor(&intern->zo);
}
/* }}} */

static zend_object *php_fasttext_object_new(zend_class_entry *class_type) /* {{{ */
{
	php_fasttext_object *intern;

	/* Allocate memory for it */
	int ftxtsize = FastTextSize();
	intern = ecalloc(1, sizeof(php_fasttext_object) + zend_object_properties_size(class_type) + ftxtsize);

	zend_object_std_init(&intern->zo, class_type);
	object_properties_init(&intern->zo, class_type);

	intern->zo.handlers = &fasttext_object_handlers;

	return &intern->zo;
}
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(fasttext)
{
	zend_class_entry ce;

	memcpy(&fasttext_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/* Register fastText Class */
	INIT_CLASS_ENTRY(ce, "fastText", php_fasttext_class_methods);
	ce.create_object = php_fasttext_object_new;
	fasttext_object_handlers.offset = XtOffsetOf(php_fasttext_object, zo);
	fasttext_object_handlers.clone_obj = NULL;
	fasttext_object_handlers.free_obj = php_fasttext_object_free_storage;
	php_fasttext_sc_entry = zend_register_internal_class(&ce);

	REGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(fasttext)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(fasttext)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "fastText support", "enabled");
	php_info_print_table_row(2, "fastText module version", PHP_FASTTEXT_VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ PHP_GINIT_FUNCTION
*/
static PHP_GINIT_FUNCTION(fasttext)
{
	memset(fasttext_globals, 0, sizeof(*fasttext_globals));
}
/* }}} */

/* {{{ fasttext_module_entry
*/
zend_module_entry fasttext_module_entry = {
	STANDARD_MODULE_HEADER,
	"fasttext",
	NULL,
	PHP_MINIT(fasttext),
	PHP_MSHUTDOWN(fasttext),
	NULL,
	NULL,
	PHP_MINFO(fasttext),
	PHP_FASTTEXT_VERSION,
	PHP_MODULE_GLOBALS(fasttext),
	PHP_GINIT(fasttext),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_FASTTEXT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(fasttext)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

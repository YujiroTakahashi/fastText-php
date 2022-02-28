#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "main/SAPI.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"
#include "php_fasttext.h"
#include "classes/ftext.h"

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
	PHP_ME(fasttext, __construct,       arginfo_fasttext_void,  ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(fasttext, __destruct,        arginfo_fasttext_void,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getError,          arginfo_fasttext_void,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, load,              arginfo_fasttext_load,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWordRows,       arginfo_fasttext_void,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getLabelRows,      arginfo_fasttext_void,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWordId,         arginfo_fasttext_word,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getSubwordId,      arginfo_fasttext_word,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWord,           arginfo_fasttext_id,    ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getLabel,          arginfo_fasttext_id,    ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getWordVectors,    arginfo_fasttext_word,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getSubwordVector,  arginfo_fasttext_word,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getSentenceVectors,arginfo_fasttext_word,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getPredict,        arginfo_fasttext_wordk, ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getNgrams,         arginfo_fasttext_word,  ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getNN,             arginfo_fasttext_wordk, ZEND_ACC_PUBLIC)
	PHP_ME(fasttext, getAnalogies,      arginfo_fasttext_wordk, ZEND_ACC_PUBLIC)

	PHP_FE_END
};
/* }}} */



static void php_fasttext_object_free_storage(zend_object *object) /* {{{ */
{
	php_fasttext_object *intern = php_fasttext_from_obj(object);

	if (!intern) {
		return;
	}

	zend_object_std_dtor(&intern->zo);
}
/* }}} */

static zend_object *php_fasttext_object_new(zend_class_entry *class_type) /* {{{ */
{
	php_fasttext_object *intern;

	/* Allocate memory for it */
	intern = ecalloc(1, sizeof(php_fasttext_object) + zend_object_properties_size(class_type));

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

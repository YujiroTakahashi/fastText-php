#ifndef PHP_CLASSES_FTEXT_H
#define PHP_CLASSES_FTEXT_H

#include <string.h>
#include <stdint.h>

#ifdef __cplusplus

#include "cfasttext.h"

extern "C" {

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"
#include "php_fasttext.h"

#endif /* __cplusplus */

typedef void *FastTextHandle;

typedef struct _php_fasttext_object {
    FastTextHandle handle;
    zval error;
    zend_object zo;
} php_fasttext_object;

static inline php_fasttext_object *php_fasttext_from_obj(zend_object *obj) {
    return (php_fasttext_object*)((char*)(obj) - XtOffsetOf(php_fasttext_object, zo));
}

#define Z_FASTTEXT_P(zv) php_fasttext_from_obj(Z_OBJ_P((zv)))

PHP_METHOD(fasttext, __construct);
PHP_METHOD(fasttext, __destruct);
PHP_METHOD(fasttext, getError);
PHP_METHOD(fasttext, load);
PHP_METHOD(fasttext, getWordRows);
PHP_METHOD(fasttext, getLabelRows);
PHP_METHOD(fasttext, getWordId);
PHP_METHOD(fasttext, getSubwordId);
PHP_METHOD(fasttext, getWord);
PHP_METHOD(fasttext, getLabel);
PHP_METHOD(fasttext, getWordVectors);
PHP_METHOD(fasttext, getSubwordVector);
PHP_METHOD(fasttext, getSentenceVectors);
PHP_METHOD(fasttext, getPredict);
PHP_METHOD(fasttext, getNgrams);
PHP_METHOD(fasttext, getNN);
PHP_METHOD(fasttext, getAnalogies);

#ifdef __cplusplus
}   // extern "C"
#endif /* __cplusplus */

#endif /* PHP_CLASSES_FTEXT_H */
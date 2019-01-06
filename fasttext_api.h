#ifndef PHP_FASTTEXT_API_H
#define PHP_FASTTEXT_API_H

#include <time.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus

#include <atomic>
#include <memory>
#include <set>

#include <fasttext/args.h>
#include <fasttext/dictionary.h>
#include <fasttext/matrix.h>
#include <fasttext/qmatrix.h>
#include <fasttext/model.h>
#include <fasttext/real.h>
#include <fasttext/utils.h>
#include <fasttext/vector.h>

extern "C" {

#endif /* __cplusplus */

#ifndef FASTTEXT_API
#   if defined(_WIN32) || defined(_WIN64)
#       define FASTTEXT_API __declspec(dllimport)
#   else
#       define FASTTEXT_API extern
#   endif /* defined(_WIN32) || defined(_WIN64) */
#endif /* FASTTEXT_API */

#ifndef FASTTEXT_VERSION
#    define FASTTEXT_VERSION 12
#endif /* FASTTEXT_VERSION */

#define FASTTEXT_TRUE           (1)
#define FASTTEXT_FALSE          (0)

struct _FTStr {
    size_t len;
    char *buff;
};

typedef float FTReal;
typedef struct _FTStr *FTStr;
typedef void *FastTextHandle;

FASTTEXT_API int FastTextVersion();
FASTTEXT_API int FastTextSize();
FASTTEXT_API FastTextHandle FastTextCreate();
FASTTEXT_API void FastTextFree(FastTextHandle handle);
FASTTEXT_API void FastTextStrFree(FTStr handle);

FASTTEXT_API int FastTextLoadModel(FastTextHandle handle, const char *filename);
FASTTEXT_API int32_t FastTextWordRows(FastTextHandle handle);
FASTTEXT_API int32_t FastTextLabelRows(FastTextHandle handle);
FASTTEXT_API int32_t FastTextWordId(FastTextHandle handle, const char* word);
FASTTEXT_API int32_t FastTextSubwordId(FastTextHandle handle, const char* word);
FASTTEXT_API FTStr FastTextGetWord(FastTextHandle handle, const int32_t wordId);
FASTTEXT_API FTStr FastTextGetLabel(FastTextHandle handle, const int32_t labelId);

FASTTEXT_API FTStr FastTextPredict(FastTextHandle handle, const char* word, const int32_t k);
FASTTEXT_API FTStr FastTextWordVectors(FastTextHandle handle, const char* word);
FASTTEXT_API FTStr FastTextSentenceVectors(FastTextHandle handle, const char* word);
FASTTEXT_API FTStr FastTextNgrams(FastTextHandle handle, const char* word);
FASTTEXT_API FTStr FastTextNN(FastTextHandle handle, const char* word, const int32_t k);
FASTTEXT_API FTStr FastTextAnalogies(FastTextHandle handle, const char* word, const int32_t k);
FASTTEXT_API FTStr FastTextDump(FastTextHandle handle, const char* word, const char* option);

#ifdef __cplusplus
}

inline FTStr FTStrVal(std::string word) {
    FTStr retval = new struct _FTStr;
    retval->len = word.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, word.c_str());
    return retval;
}
#endif /* __cplusplus */

#endif /* PHP_FASTTEXT_API_H */
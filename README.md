# fastText-php

fastText-php is a PHP bindings for fastText.

[fastText](https://fasttext.cc/) is a library for efficient learning of word representations and sentence classification.

## Requirements

PHP 7.x  
fastText shard object  

```
$ git clone https://github.com/facebookresearch/fastText.git
$ cd fastText
$ mkdir build && cd build && cmake ..
$ make
$ sudo make install
```

## Building fastText for PHP

```
$ cd fastText-php
$ phpize
$ ./configure
$ make
$ sudo make install
```

edit your php.ini and add:

```
extension=fasttext.so
```

## Class synopsis

```php
fastText {
    public __construct ( void )
    public int load ( string filename )
    public int getWordRows ( void )
    public int getLabelRows ( void )
    public int getWordId ( string word )
    public string getWord ( int word_id )
    public string getLabel ( int label_id )
    public array getWordVectors ( string word )
    public array getSentenceVectors ( string sentence )
    public mixed getPredict ( streing word [, int k] )
    public mixed getNN ( streing word [, int k] )
    public mixed getAnalogies ( streing word [, int k] )
    public mixed getNgramVectors ( streing word )
    public string lastErrorMsg ( void )
}
```

## Table of Contents

[fastText::__construct](#__construct)  
[fastText::load](#load)  
[fastText::getWordRows](#getwordrows)  
[fastText::getLabelRows](#getlabelrows)  
[fastText::getWordId](#getworded)  
[fastText::getWord](#getword)  
[fastText::getLabel](#getlabel)  
[fastText::getWordVectors](#getwordvectors)  
[fastText::getSentenceVectors](#getsentenceVectors)  
[fastText::getPredict](#getpredict)  
[fastText::getNN](#getnn)  
[fastText::getAnalogies](#getanalogies)  
[fastText::getNgramVectors](#getngramvectors)  
[fastText::lastErrorMsg](#lastErrormsg)  
  
[return value format](#returnvalf)  

-----

### <a name="__construct">fastText::__construct()

Instantiates a fastText object.

```php
$ftext = new fastText();
```

-----

### <a name="load">int fastText::load(string filename)

load a model.

```php
$model = 'result/model.bin';
$ftext->load($model);
```

-----

### <a name="getwordrows">int fastText::getWordRows()

get the number of vocabularies.

```php
$rows = $ftext->getWordRows();
$words = [];
for ($idx = 0; $idx < $rows; $idx++) {
    $words[$idx] = $ftext->getWord($idx);
}
```

-----

### <a name="getlabelrows">int fastText::getLabelRows()

get the number of labels.

```php
$rows = $ftext->getLabelRows();
$labels = [];
for ($idx = 0; $idx < $rows; $idx++) {
    $labels[$idx] = $ftext->getLabel($idx);
}
```

-----

### <a name="getwordid">int fastText::getWordId(string word)

get the word ID within the dictionary.

```php
$word = 'Bern';
$rowId = $ftext->getWordId($word);
```

-----

### <a name="getword">string fastText::getWord(int word_id)

converts a ID into a word.

```php
$rows = $ftext->getWordRows();
$words = [];
for ($idx = 0; $idx < $rows; $idx++) {
    $words[$idx] = $ftext->getWord($idx);
}
```

-----

### <a name="getlabel">string fastText::getLabel(int label_id)

converts a ID into a label.

```php
$rows = $ftext->getLabelRows();
$labels = [];
for ($idx = 0; $idx < $rows; $idx++) {
    $labels[$idx] = $ftext->getLabel($idx);
}
```

-----

### <a name="getwordvectors">array fastText::getWordVectors(string word)

get the vector representation of word.

```php
$vectors = $ftext->getWordVectors('Beijing');
print_r($vectors);
```

-----

### <a name="getsentencevectors">array fastText::getSentenceVectors(string sentence)

get the vector representation of sentence.

```php
$sentence = "It's fine day";

$vectors = $ftext->getSentenceVectors($sentence);
print_r($vectors);
```

-----

### <a name="getpredict">fastText::getPredict
* array fastText::getPredict(string word)
* FALSE fastText::getPredict(string word)

predict most likely labels with probabilities.

```php
$probs = $ftext->getPredict('Berlin');
foreach ($probs as $row) {
    echo $row['label'].'  '.$row['prob'];
}
```

-----

### <a name="getnn">fastText::getNN
* array fastText::getNN(string word)
* FALSE fastText::getNN(string word)

query for nearest neighbors.

```php
$probs = $ftext->getNN('Washington, D.C.');
foreach ($probs as $row) {
    echo $row['label'].'  '.$row['prob'];
}
```

-----

### <a name="getanalogies">fastText::getAnalogies
* array fastText::getAnalogies(string word)
* FALSE fastText::getAnalogies(string word)

query for analogies.

```php
$probs = $ftext->getAnalogies('Paris + France - Spain');
foreach ($probs as $row) {
    echo $row['label'].'  '.$row['prob'];
}
```

-----

### <a name="getngramvectors">fastText::getNgramVectors
* array fastText::getNgramVectors(string word)
* FALSE fastText::getNgramVectors(string word)

get the ngram vectors.

```php
$res = $ftext->getNgramVectors('London');
print_r($res);
```

-----

### <a name="lasterrormsg">string fastText::lastErrorMsg()

get the latest error message.

```php
$probs = $ftext->getNN('Tokyo');
if (FALSE === $probs) {
    echo $ftext->lastErrorMsg();
}
print_r($probs);
```

-----

## <a name="returnvalf">return value format

```php
$probs =
[
    ['label'=> '__label__1', 'prob'=> 0.4234 ],
    ['label'=> '__label__2', 'prob'=> 0.2345 ],
    ['label'=> '__label__3', 'prob'=> 0.1456 ],
                        :
                        :
                        :
]
```

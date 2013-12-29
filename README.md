[![Build Status](https://travis-ci.org/amitaibu/biblio.png?branch=7.x-3.x)](https://travis-ci.org/amitaibu/biblio)

### Render API

```php
$biblio = bilbio_load(1);
$biblio->getText('bibtex');
```

### Import API

```php
// Here you need to specify the biblio style of the data you wish to import.
// In this example, the style is BibTex.
$imported_data_style = 'bibtex';

// The data you wish to import.
// In this example it is a book.
$data = '
@Book{washington+franklin,
  author    = "George {Washington} and Benjamin {Franklin}",
  title     = "Book About the USA",
  publisher = "ABC",
  year      =  1980,
  address   = "Los Angeles",
  edition   = "ninth ABC printing, tenth DEF printing"
}';

// Get the relevant biblio class to handle the information.
ctools_include('plugins');
$plugin = biblio_get_biblio_style($imported_data_style);
$class = ctools_plugin_load_class('biblio', 'biblio_style', $imported_data_style, 'class');

// Create the biblio using the relevant class.
$biblio_style = new $class($plugin);

// Import the biblios from $data.
// This returns an array because multiple biblios can be imported at once.
// The biblios in the array are grouped by the result:
//   'new'       - New biblios, created in the import process.
//   'duplicate' - Existing biblios, when one or more of the biblios in the data are identical to existing biblios.
//   'error'     - Errors that occured during the import process, this means one or more biblios failed to import.
// In this example, the array will contain only one biblio, categorized as 'new'.
$biblios = $biblio_style->import($data);
$new_biblio = $biblios['new'][0];
```



### CiteProc

* Download https://github.com/gbv/citeproc-php and place it under libraries/citeproc-php
* Enable libraries module
* Execute code:

```php

  $biblio = biblio_create('journal');
  $biblio->getText('citeproc', array('style_name' => 'ama'));
  $biblio->getText('citeproc');
```

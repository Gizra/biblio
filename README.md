[![Build Status](https://travis-ci.org/amitaibu/biblio.png?branch=7.x-3.x)](https://travis-ci.org/amitaibu/biblio)

# Biblio

Biblio is used for importing and rendering bibliographies.

### Render Biblio API

```php
// Loading the biblio.
$biblio_bid = 1;
$biblio = bilbio_load($biblio_bid);

// Specify the style in which you wish the output to be.
// In this example, we want it to be in BibTex style.
$biblio_style = 'bibtex';

// Render the biblio.
$biblio->getText($biblio_style);
```

### Import Biblio API

```php
// Here you need to specify the biblio style of the data you wish to import.
// In this example, the style is BibTex.
$data_style = 'bibtex';

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
$plugin = biblio_get_biblio_style($data_style);
$class = ctools_plugin_load_class('biblio', 'biblio_style', $data_style, 'class');

// Create the biblio using the relevant class.
$biblio_style = new $class($plugin);

// Import the biblios from $data.
// This returns an array because multiple biblios can be imported at once.
// The biblios in the array are grouped by the result:
//   - 'new': New biblios, created in the import process.
//   - 'duplicate': Existing biblios, when one or more of the biblios in the data
//      are identical to existing biblios.
//   - 'error': Errors that occured during the import process, this means one or
//     more biblios failed to import.
// In this example, the array will contain only one biblio, categorized as
// 'new'.
$biblios = $biblio_style->import($data);
$new_biblio = $biblios['new'][0];
```

### Adding Contributors

Contributors can be added using the ``Biblio::addContributors`` helper method.

```php
// Biblio Contributors' names.
$biblio = biblio_create('book');

// Add multiple authors.
$biblio->addContributors('John Doe and Ploni Almoni');

// Add an editor.
$biblio->addContributors('John Smith', 'Editor');

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

### Example module

For more useful examples, we recommend enabling the module ``biblio_example``.


Developed by [Gizra](http://gizra.com)

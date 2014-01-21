[![Build Status](https://travis-ci.org/Gizra/biblio.png?branch=7.x-3.x)](https://travis-ci.org/Gizra/biblio)

# Biblio

Biblio is used for importing and rendering bibliographies.

## Installation

* Download https://github.com/gbv/citeproc-php and place it under ``sites/all/libraries/citeproc-php``
* Optional but recommended - Download https://github.com/citation-style-language/styles and place it under ``sites/all/libraries/styles``
* Enable [libraries](https://drupal.org/project/libraries) module
* In ``admin/structure/biblio/attach-fields`` select all the Biblio types and click ``Attach fields to selected types`` 

## Upgrade from 1.x

* Backup your existing DB!
* Download [Migrate](https://drupal.org/node/2029049) (version 2.6-rc1 or higher), and enable Migrate UI
* Download and enable [Migrate extras](https://drupal.org/project/migrate_extras)
* Replace the old ``biblio`` folder with the 3.x version
* Execute ``update.php``
* Follow the steps from the above "Installation" section
* In ``admin/content/migrate/configure`` click on ``Register statically-defined classes``
* In ``admin/content/migrate`` Select ``Biblio 3.x`` and ``Execute``

Congrats, your Biblio installation is now upgraded to 3.x

## Render Biblio API

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

## Import Biblio API

```php
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

// Get the relevant biblio style class to handle the information.
$biblio_style = biblio_get_class_from_style('bibtex')

// Import the Biblios.
$biblios = $biblio_style->import($data);
```

## Adding Contributors

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

Styles can now be rendered using CiteProc, for example:

```php
  $biblio = biblio_create('journal');
  $biblio->getText('citeproc', array('style_name' => 'ama'));
  $biblio->getText('citeproc');
```

## Example module

For more useful examples, we recommend enabling the module ``biblio_example``.


## Credits

The 3.x version is developed by [Gizra](http://gizra.com) and sponsored by [Harvard OpenScholar](http://openscholar.harvard.edu/)

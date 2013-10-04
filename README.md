(Temporary) Make sure to enable Biblio using drush, to allow attaching fields to all the bundles (``drush en biblio_ui``)

To use CiteProc:

* Download https://github.com/gbv/citeproc-php and place it under libraries/citeproc-php
* Enable libraries module
* Execute code:

```php

  $biblio = biblio_create('journal');
  dpm($biblio->getText('citeproc', array('style_name' => 'ama')));
  dpm($biblio->getText('citeproc'));
```

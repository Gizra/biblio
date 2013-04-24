```php

  $biblio = biblio_create('journal');
  dpm($biblio->getText('citeproc', array('style_name' => 'ama')));
  dpm($biblio->getText('citeproc'));
```

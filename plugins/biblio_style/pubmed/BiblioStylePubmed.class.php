<?php

/**
 * @file
 * PubMed style.
 */

class BiblioStylePubmed extends BiblioStyleBase {

  /**
   * Import PubMed entries.
   *
   * @todo: Deal with duplication.
   *
   * @param $data
   * @param string $type
   * @return array
   */
  public function import($data, $type = 'text') {
    $xml = $data;

    $pubmed = new BiblioEntrezPubmedArticle();

    // Array of Biblios.
    $biblios = array();

    foreach ($xml->xpath('//PubmedArticle') as $article) {

      $biblio = biblio_create('article');
      $wrapper = entity_metadata_wrapper('biblio', $biblio);

      $wrapper->title->set($article->Article->ArticleTitle);

      // Attempts to extract the name of the journal from MedlineTA if
      // available.
      if (!empty($article->MedlineJournalInfo->MedlineTA)) {
        $secondary_title = $article->MedlineJournalInfo->MedlineTA;
      }
      elseif (!empty($article->Article->Journal->ISOAbbreviation)) {
        $secondary_title = $article->Article->Journal->ISOAbbreviation;
      }
      else {
        $secondary_title = $article->Article->Journal->Title;
      }
      $wrapper->biblio_secondary_title->set($secondary_title);

      $citekey = variable_get('biblio_auto_citekey', TRUE) ? '' : $article->MedlineCitation->PMID;
      $wrapper->biblio_citekey->set($citekey);

      $wrapper->biblio_volume->set($article->Article->Journal->JournalIssue->Volume);
      $wrapper->biblio_issue->set($article->Article->Journal->JournalIssue->Issue);
      $wrapper->biblio_issn->set($article->Article->Journal->ISSN);
      $wrapper->biblio_pages->set($article->Article->Pagination->MedlinePgn);

      $pub_date = $article->Article->Journal->JournalIssue->PubDate;

      // @todo: Return timestamp?
      $date = isset($pub_date->MedlineDate) ? $pub_date->MedlineDate : implode(' ', (array)$pub_date);

      $wrapper->biblio_issued->set($date);

      // @todo: Use date()?
      // @todo: move this logic to the controller?
      $wrapper->biblio_year->set(substr($date, 0, 4));

      if (isset($article->Article->Abstract)) {
        $abstract = '';
        foreach ($article->Article->Abstract->AbstractText as $text) {
          if (!empty($abstract)) {
            $abstract .= "\n\n";
          }
          $attrs = $text->attributes();
          if (isset($attrs['Label'])) {
            $abstract .= $attrs['Label'] . ': ';
          }
          $abstract .= $text ;
        }
        $abstract;
      }

      $wrapper->biblio_abstract->set($abstract);

      $this->biblio = array(
        'biblio_pubmed_id' => $this->id,
        'biblio_pubmed_md5' => $this->md5,
        'biblio_contributors' => $this->contributors(),
        // MedlineCitations are always articles from journals or books
        'biblio_custom1'  => "http://www.ncbi.nlm.nih.gov/pubmed/{$this->id}?dopt=Abstract",
        'biblio_keywords' => $this->keywords(),
        'biblio_lang'     => $this->lang(),
      );

      $wrapper->save();
      $biblios[] = $biblio;
    }

    return array(
      'new' => $biblios,
    );
  }

  /**
   * Import a generic property.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio.
   * @param $property_name
   *   The propery name (e.g. biblio_year).
   * @param $data
   *   A single PubMed article to be processed.
   */
  public function importEntryGeneric(EntityMetadataWrapper $wrapper, $property_name, $data) {
    $mapping = $this->getMapping();
    $property = $mapping['field'][$property_name];

    // Drill into the object until we have reached our property.
    $sub_data = $data;

    foreach ($property['import_location'] as $location) {
      if (empty($sub_data->{$location})) {
        return;
      }
      $sub_data = $sub_data->{$location};
    }

    $wrapper->{$property_name}->set($sub_data);
  }

  /**
   * Import secondary title.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio.
   * @param $property_name
   *   The propery name (e.g. biblio_year).
   * @param $data
   *   A single PubMed article to be processed.
   */
  public function importSecondaryTitle(EntityMetadataWrapper $wrapper, $property_name, $data) {
    if (!empty($data->MedlineJournalInfo->MedlineTA)) {
      $title = $data->MedlineJournalInfo->MedlineTA;
    }
    elseif (!empty($data->Article->Journal->ISOAbbreviation)) {
      $title = $data->Article->Journal->ISOAbbreviation;
    }
    else {
      $title = $data->Article->Journal->Title;
    }

    $wrapper->{$property_name}->set($title);
  }

  /**
   * @inheritdoc
   */
  public function getMapping() {
    $return = parent::getMapping();

    $return['field'] = array(
      'title' => array(
        'import_location' => array('Article', 'ArticleTitle'),
      ),
      'biblio_citekey' => $citekey,
      'biblio_pubmed_id' => $this->id,
      'biblio_pubmed_md5' => $this->md5,
      'biblio_contributors' => $this->contributors(),

      'biblio_year' => array(
        'import_method' => 'importYear',
      ),
      'biblio_secondary_title' => array(
        'import_method' => 'importSecondaryTitle',
      ),
      'biblio_alternate_title' => array(
        'import_location' => array('Journal', 'ISOAbbreviation'),
      ),
      'biblio_volume' => array(
        'import_location' => array('Journal', 'JournalIssue', 'Volume'),
      ),
      'biblio_issue' => array(
        'import_location' => array('Journal', 'JournalIssue', 'Issue'),
      ),
      'biblio_issn' => array(
        'import_location' => array('Journal', 'ISSN'),
      ),
      'biblio_pages' => array(
        'import_location' => array('Pagination', 'MedlinePgn'),
      ),
      'biblio_abstract' => array(
        'import_method' => 'importAbstract',
      ),
      // @todo: Where should we map this?
      'biblio_custom1' => "http://www.ncbi.nlm.nih.gov/pubmed/{$this->id}?dopt=Abstract",

      'biblio_keywords' => array(
        'import_method' => 'importKeywords',
      ),
      'biblio_language' => array(
        'import_location' => array('Article', 'Language'),
      ),
    );

    // Assign default import method.
    foreach ($return['field'] as $key => $value) {
      $return['field'][$key] += array(
        'import_method' => 'importEntryGeneric',
        'render_method' => 'renderEntryGeneric',
      );
    }

    return $return;
  }
}

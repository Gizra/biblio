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
  public function importData($data, $type = 'text') {
    $xml = new SimpleXMLElement($data);

    $pubmed = new BiblioEntrezPubmedArticle();

    // Array of Biblios.
    $biblios = array();
    $mapping = $this->getMapping();

    foreach ($xml->xpath('//PubmedArticle') as $article) {

      $biblio = biblio_create('journal');
      $wrapper = entity_metadata_wrapper('biblio', $biblio);

      foreach ($mapping['field'] as $property_name => $property) {
        if (empty($wrapper->{$property_name})) {
          // @todo: Is this the right place.
          biblio_create_field($property_name, 'biblio', $wrapper->getBundle());
        }

        $method = $property['import_method'];
        $this->{$method}($wrapper, $property_name, $article->MedlineCitation);
      }

      $biblios['success'][] = $biblio;
    }

    return $biblios;
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

    $wrapper->{$property_name}->set((string)$sub_data);
  }

  /**
   * Import abstract property.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio.
   * @param $property_name
   *   The propery name (e.g. biblio_abstract).
   * @param $data
   *   A single PubMed article to be processed.
   */
  public function importAbstract(EntityMetadataWrapper $wrapper, $property_name, $data) {
    if (!isset($data->Article->Abstract)) {
      return;
    }

    $abstract = array();
    foreach ($data->Article->Abstract->AbstractText as $text) {
      $output = '';
      $attrs = $text->attributes();
      if (isset($attrs['Label'])) {
        $abstract .= $attrs['Label'] . ': ';
        $output = $attrs['Label'] . ': ';
      }
      $output .= (string) $text;
      $abstract[] =  $output;
    }
    $wrapper->{$property_name}->set(implode("\n", $abstract));
  }

  /**
   * Import keywords property.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio.
   * @param $property_name
   *   The propery name (e.g. biblio_keywords).
   * @param $data
   *   A single PubMed article to be processed.
   */
  public function importKeywords(EntityMetadataWrapper $wrapper, $property_name, $data) {
    if (!isset($data->MeshHeadingList->MeshHeading)) {
      return;
    }
    $keywords = array();
    foreach ($data->MeshHeadingList->MeshHeading as $heading) {
      $keywords[] = (string)$heading->DescriptorName;
    }
    parent::importKeywords($wrapper, $property_name, $keywords);
  }


  /**
   * Import year property.
   *
   * @todo: Is this assumption ok?
   * According to PubMed that date might be a string or an XML. We take the
   * first 4 digits as the year,as Biblio year field is an integer field.
   *
   * @link http://www.nlm.nih.gov/bsd/licensee/elements_descriptions.html#pubdate
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio.
   * @param $property_name
   *   The propery name (e.g. biblio_keywords).
   * @param $data
   *   A single PubMed article to be processed.
   */
  public function importYear(EntityMetadataWrapper $wrapper, $property_name, $data) {
    $pub_date = $data->Article->Journal->JournalIssue->PubDate;

    if ($pub_date->Year) {
      $year = (string) $pub_date->Year;
    }
    elseif ($pub_date->MedlineDate) {
      $year = substr((string)$pub_date->MedlineDate, 0, 4);
    }

    $wrapper->{$property_name}->set($year);
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

    $wrapper->{$property_name}->set((string)$title);
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
      // 'biblio_custom1' => "http://www.ncbi.nlm.nih.gov/pubmed/{$this->id}?dopt=Abstract",

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

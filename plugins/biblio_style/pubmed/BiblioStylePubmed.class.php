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
}

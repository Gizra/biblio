<?php
// $Id$
/**
 * @file EntrezPubmedArticle.php
 * Provides a class for handling PubMed articles retrieved with EFetch.
 * Orginally writen by Stefan Freudenberg
 */

class BiblioEntrezPubmedArticle
{
  private $article;

  private $id;

  private $md5;

  private $biblio = array();

  /**
   * Stores the given SimpleXMLElement the PubMed ID and the md5 hash of the
   * serialized XML.
   *
   * @param $pubmedArticle
   *   a PubmedArticle element
   */
  public function __construct(SimpleXMLElement $pubmedArticle)
  {
    $this->article = $pubmedArticle->MedlineCitation->Article;
    $this->id = (int)$pubmedArticle->MedlineCitation->PMID;
    $this->md5 = md5($pubmedArticle->asXML());
  }

  /**
   * Returns the PubMed ID of the article.
   *
   * @return int
   */
  public function getId()
  {
    return $this->id;
  }

  /**
   * Returns the md5 hash of the serialized XML.
   *
   * @return string
   */
  public function getMd5()
  {
    return $this->md5;
  }

  /**
   * Returns article elements as an associative array suitable for import into
   * a biblio node.
   *
   * @return array
   */
  public function getBiblio()
  {
    if (empty($this->biblio)) {
      $this->biblio = array(
        'title' => (string)$this->article->ArticleTitle,
        'biblio_citekey' => $this->id,
        'biblio_pubmed_id' => $this->id,
        'biblio_pubmed_md5' => $this->md5,
        'biblio_contributors' => $this->contributors(),
        // MedlineCitations are always articles from journals or books
        'biblio_type' => 102,
        'biblio_date' => $this->date(),
        'biblio_year' => substr($this->date(), 0, 4),
        'biblio_secondary_title' => (string)$this->article->Journal->Title,
        'biblio_alternate_title' => (string)$this->article->Journal->ISOAbbreviation,
        'biblio_volume' => (string)$this->article->Journal->JournalIssue->Volume,
        'biblio_issue' => (string)$this->article->Journal->JournalIssue->Issue,
        'biblio_issn' => (string)$this->article->Journal->Issn,
        'biblio_pages' => (string)$this->article->Pagination->MedlinePgn,
        'biblio_abst_e' => (string)$this->article->Abstract->AbstractText,
        'biblio_custom1' => "http://www.ncbi.nlm.nih.gov/pubmed/{$this->id}?dopt=Abstract",
      );

      $doi = $this->article->xpath('//ELocationID[@EIdType="doi"]/text()');
      if (!empty($doi)) {
        $this->biblio['biblio_doi'] = (string)$doi[0];
      }
    }

    return $this->biblio;
  }

  /**
   * Returns the list of contributors for import obtained from the given
   * MedlineCitation element.
   *
   * @return array
   *   the contributors of the article
   */
  private function contributors()
  {
    $contributors = array();

    if (isset($this->article->AuthorList->Author)) {
      foreach ($this->article->AuthorList->Author as $author) {
        if (isset($author->CollectiveName)) {
          $category = 5; // corporate author
          $name = (string)$author->CollectiveName;
        } else {
          $category = 1; //primary (human) author
          $lastname = (string)$author->LastName;
          if (isset($author->ForeName)) {
            $name = $lastname . ', ' . (string)$author->ForeName;
          } elseif (isset($author->FirstName)) {
            $name = $lastname . ', ' . (string)$author->FirstName;
          } elseif (isset($author->Initials)) {
            $name = $lastname . ', ' . (string)$author->Initials;
          }
        }
        $contributors[$category][] = array('name' => $name);
      }
    }

    return $contributors;
  }

  /**
   * Returns the publication date obtained from the given MedlineCitation's
   * PubDate element. See the reference documentation for possible values:
   * http://www.nlm.nih.gov/bsd/licensee/elements_descriptions.html#pubdate
   * According to the above source it always begins with a four digit year.
   *
   * @return string
   *   the publication date of the article
   */
  private function date()
  {
    $pubDate = $this->article->Journal->JournalIssue->PubDate;

    if (isset($pubDate->MedlineDate)) {
      $date = (string)$pubDate->MedlineDate;
    } else {
      $date = implode(' ', (array)$pubDate);
    }

    return $date;
  }
}

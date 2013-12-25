<?php

/**
 * Implements hook_biblio_fields_info().
 */
function biblio_biblio_fields_info() {
  // Abstract.
  $items['biblio_abstract'] = array(
    'field' => array(
      'cardinality' => '1',
      'entity_types' => array(),
      'field_name' => 'biblio_abstract',
      'module' => 'text',
      'type' => 'text_long',
    ),
    'instance' => array(
      'label' => 'Publication abstract',
      'description' => 'Holds generic info the biblio.',
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '-1',
      ),
    ),
  );

  // Citekey.
  $items['biblio_citekey'] = array(
    'field' => array(
      'cardinality' => '1',
      'entity_types' => array(),
      'field_name' => 'biblio_citekey',
      'module' => 'text',
      'type' => 'text_long',
    ),
    'instance' => array(
      'label' => 'Publication citation',
      'description' => 'Publication citation.',
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '-1',
      ),
    ),
  );

  // Image.
  $items['biblio_image'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_image',
      'locked' => '0',
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'image',
    ),
    'instance' => array(
      'label' => 'The publication image',
      'description' => 'Image for the biblio',
      'settings' => array(
        'alt_field' => 0,
        'default_image' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => '-2',
      ),
    ),
  );

  // PDF file.
  $items['biblio_pdf'] = array(
    'field' => array(
      'field_name' => 'biblio_pdf',
      'type' => 'file',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'locked' => '0',
      'translatable' => '0',
      'settings' => array(
        'uri_scheme' => 'public',
      ),
    ),
    'instance' => array(
      'label' => 'The publication PDF',
      'description' => 'PDF for the biblio',
      'settings' => array(
        'file_directory' => '',
        'file_extensions' => 'pdf',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'type' => 'file_generic',
        'weight' => '-4',
      ),
    ),
  );

  // Other Numbers.
  $items['biblio_other_number'] = array(
    'field' => array(
      'cardinality' => 1,
      'field_name' => 'biblio_other_number',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'instance' => array(
      'default_value' => NULL,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
        ),
      ),
      'label' => 'Other Numbers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
      ),
    ),
    // @todo: Applies to all?
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'broadcast' => array(),
      'case' => array(),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
      'hearing' => array(),
    ),
  );

  // Publication year.
  $items['biblio_year'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_year',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'instance' => array(
      'default_value' => NULL,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
        ),
      ),
      'field_name' => 'biblio_year',
      'label' => 'Year of Publication',
      // We will enforce required only if the Publication status is set to
      // "published".
      'required' => FALSE,
      'settings' => array(
        'max' => '9999',
        'min' => '-9999',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => 0,
      ),
    ),
  );

  // First letter; can be used to store the character we would like to later
  // group the publications.
  $items['biblio_first_letter'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_first_letter',
      'module' => 'text',
      'settings' => array(
        'max_length' => '1',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Publication first letter',
      'description' => 'Hold the character to later the publications can be grouped by.',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '1',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
  );

  // Section.
  $items['biblio_section'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_section',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Section',
      'required' => FALSE,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'book_chapter' => array(
        'label' => 'Chapter',
      ),
      'bill' => array(
        'label' => 'Code Section',
      ),
      'case' => array(
        'label' => 'Page Cited',
      ),
      'government_report' => array(),
    ),
  );

  // Issue.
  $items['biblio_issue'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_issue',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Issue',
      'required' => FALSE,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'government_report' => array(),
    ),
  );

  // Number of volumes.
  $items['biblio_number_of_volumes'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_number_of_volumes',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Number of Volumes',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'audiovisual' => array(
        'label' => 'Extent of Work',
      ),
      'book_chapter' => array(),
      'book' => array(),
      'case' => array(
        'label' => 'Reporter Abbreviation',
      ),
      'classical' => array(),
      'conference_proceedings' => array(),
      'hearing' => array(),
    ),
  );

  // Number.
  $items['biblio_number'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_number',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Series Volume',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(
        'label' => 'Bill Number',
      ),
      'chart' => array(
        'label' => 'Number',
      ),
      'classical' => array(),
      'film' => array(
        'label' => 'Running Time',
      ),
      'hearing' => array(
        'label' => 'Document Number',
      ),
    ),
  );

  // Pages.
  $items['biblio_pages'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_pages',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Publication pages',
      'description' => 'First & last page numbers of the piece. e.g. 23-25',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'artwork' => array(
        'label' => 'Description',
      ),
      'book' => array(
        'label' => 'Number of Pages',
      ),
      'bill' => array(
        'label' => 'Code Pages',
      ),
      'case' => array(
        'label' => 'First Page',
      ),
      'chart' => array(
        'label' => 'Description',
      ),
      'classical' => array(
        'label' => 'Number of Pages',
      ),
      'conference_proceedings' => array(
        'label' => 'Pagination',
      ),
      'government_report' => array(
        'label' => 'Pagination',
      ),
      'hearing' => array(
        'label' => 'Pagination',
      ),
    ),
  );


  // Reprint edition.
  $items['biblio_reprint_edition'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_reprint_edition',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Reprint Edition',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'book' => array(),
      'book_chapter' => array(),
      'classical' => array(),
    )
  );

  // Call number.
  $items['biblio_call_number'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_call_number',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Call number',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
      // @todo: Applies to all?
      'bundles_info' => array(
        'artwork' => array(),
        'audiovisual' => array(),
        'book' => array(),
        'book_chapter' => array(),
        'bill' => array(),
        'broadcast' => array(),
        'chart' => array(),
        'classical' => array(),
        'conference_paper' => array(),
        'conference_proceedings' => array(),
        'database' => array(),
        'film' => array(),
        'government_report' => array(),
        'hearing' => array(),
        'journal' => array(),
      ),
    ),
  );

  // ISBN number.
  $items['biblio_isbn'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_isbn',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'ISBN Number',
      'description' => 'International Standard Book Number',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    // @todo: Applies to all?
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'broadcast' => array(),
      'case' => array(),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
      'journal' => array(),
    ),
  );

  // Accession number
  $items['biblio_accession_number'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_accession_number',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Accession number',
      'description' => '',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    // @todo: Applies to all?
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'broadcast' => array(),
      'case' => array(),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
      'journal' => array(),
    ),
  );

  // Keywords.
  $items['biblio_keywords'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_keywords',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Keywords',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
  );

  // Notes.
  $items['biblio_notes'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_notes',
      'module' => 'text',
      'type' => 'text_long',
    ),
    'instance' => array(
      'label' => 'Notes',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textarea',
        'weight' => '-3',
      ),
    ),
  );

  // Research notes.
  $items['biblio_research_notes'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_research_notes',
      'module' => 'text',
      'type' => 'text_long',
    ),
    'instance' => array(
      'label' => 'Research Notes',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textarea',
        'weight' => '-3',
      ),
    ),
  );

  // Translated title.
  $items['biblio_translated_title'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_translated_title',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Translated Title',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
  );

  // Summary char; can be used to store the character we would like to later
  // group the publications.
  $items['biblio_summary_char'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_summary_char',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Publication summary character',
      'description' => 'Hold the character to later the publications can be grouped by.',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
  );

  // Contributor fields.
  $items['contributor_field_collection'] = array(
    'field' => array(
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'field_name' => 'contributor_field_collection',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'field_collection',
      'settings' => array(
        'hide_blank_items' => 1,
        'path' => '',
      ),
      'translatable' => '0',
      'type' => 'field_collection',
    ),
    'instance' => array(
      'label' => 'Contributors',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => 0,
      ),
    ),
  );

  // Reference to contributor role.
  $items['biblio_contributor_role'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_contributor_role',
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'sort' => array(
            'type' => 'none',
          ),
          'target_bundles' => array(
            'biblio_roles' => 'biblio_roles',
          ),
        ),
        'target_type' => 'taxonomy_term',
      ),
      'translatable' => '0',
      'type' => 'entityreference',
    ),
    'instance' => array(
      'label' => 'Contributor role',
      'required' => TRUE,
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => '60',
        ),
        'type' => 'options_select',
        'weight' => '2',
      ),
    ),
  );

  // Reference to a single contributor.
  $items['biblio_contributor'] = array(
    'field' => array(
      'active' => '1',
      'cardinality' => 1,
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'biblio_contributor',
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'sort' => array(
            'type' => 'none',
          ),
          'target_bundles' => array(),
        ),
        'target_type' => 'biblio_contributor',
      ),
      'translatable' => '0',
      'type' => 'entityreference',
    ),
    'instance' => array(
      'label' => 'Contributor reference',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'inline_entity_form',
        'settings' => array(
          'type_settings' => array(
            'allow_existing' => TRUE,
            'match_operator' => 'CONTAINS',
          ),
        ),
        'type' => 'inline_entity_form',
        'weight' => '1',
      ),
    ),
  );

  // Short title.
  $items['biblio_short_title'] = array(
    'description' => 'Short title.',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_short_title',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Short title',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'case' => array(
        'label' => 'Abbreviated Case Name',
      ),
      'classical' => array(),
      'conference_proceedings' => array(),
      'film' => array(),
      'hearing' => array(),
    )
  );

  // Short title.
  $items['biblio_alternate_title'] = array(
    'description' => 'Alternate Title',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_alternate_title',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Alternate Title',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(
        'label' => 'Abbreviation',
      ),
      'book_chapter' => array(
        'label' => 'Abbreviation',
      ),
      'classical' => array(),
      'film' => array(),
      'hearing' => array(),
    ),
  );

  // Type of work.
  $items['biblio_type_of_work'] = array(
    'description' => 'Type of work',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_type_of_work',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Type of work',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
      'bundles_info' => array(
        'audiovisual' => array(
          'label' => 'Type',
        ),
        'book' => array(),
      ),
      'chart' => array(
        'label' => 'Type of Image',
      ),
      'classical' => array(
        'label' => 'Type',
      ),
      'conference_proceedings' => array(),
      'film' => array(
        'label' => 'Medium',
      ),
    ),
  );

  // Biblio URL.
  $items['biblio_url'] = array(
    'description' => 'URL',
    'field' => array(
      'cardinality' => 1,
      'field_name' => 'biblio_url',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'URL',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    // @todo: Applies to all?
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'broadcast' => array(),
      'case' => array(),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
      'journal' => array(),
    )
  );

  // Biblio volume.
  $items['biblio_volume'] = array(
    'description' => 'URL',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_volume',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Volume',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(
        'label' => 'Code Volume',
      ),
      'case' => array(
        'label' => 'Reporter Volume',
      ),
      'chart' => array(
        'label' => 'Image Size',
      ),
      'classical' => array(),
      'conference_proceedings' => array(),
      'government_report' => array(),
    ),
  );

  // Biblio volume.
  $items['biblio_doi'] = array(
    'description' => 'DOI',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_doi',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'volume',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    // @todo: Applies to all?
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'broadcast' => array(),
      'case' => array(),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
      'journal' => array(),
    )
  );

  // Biblio place published.
  $items['biblio_place_published'] = array(
    'description' => 'City',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_place_published',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'City',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'audiovisual' => array(),
      'artwork' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'case' => array(
        'label' => 'Place Published',
      ),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'film' => array(
        'label' => 'Country',
      ),
      'hearing' => array(
        'label' => 'City',
      ),
    ),
  );

  // Biblio publisher.
  $items['biblio_publisher'] = array(
    'description' => 'Publisher',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_publisher',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Publisher',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'case' => array(
        'label' => 'Court',
      ),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
    ),
  );

  // Biblio edition.
  $items['biblio_edition'] = array(
    'description' => 'Edition',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_edition',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Edition',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' =>array(
        'label' => 'Session',
      ),
      'chart' => array(
        'label' => 'Version',
      ),
      'classical' => array(),
      'conference_proceedings' => array(),
      'government_report' => array(),
      'hearing' => array(
        'label' => 'Session',
      ),
    ),
  );

  // Biblio secondary title.
  $items['biblio_secondary_title'] = array(
    'description' => 'Secondary title',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_secondary_title',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Secondary title',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'audiovisual' => array(
        'label' => 'Series Title',
      ),
      'book' => array(
        'label' => 'Series Title',
      ),
      'book_chapter' => array(
        'label' => 'Book Title',
      ),
      'bill' => array(
        'label' => 'Code',
      ),
      'case' => array(
        'label' => 'Reporter',
      ),
      'chart' => array(
        'label' => 'Image Source Program',
      ),
      'classical' => array(
        'label' => 'Series Title',
      ),
      'conference_paper' => array(
        'label' => 'Conference Name',
      ),
      'conference_proceedings' => array(
        'label' => 'Conference Name',
      ),
      'database' => array(
        'required' => TRUE,
      ),
      'film' => array(
        'label' => 'Series Title',
      ),
      'magazine_article' => array(
        'label' => 'Magazine',
      ),
      'hearing' => array(
        'label' => 'Committee',
      ),
    ),
  );

  // Biblio tertiary title.
  $items['biblio_tertiary_title'] = array(
    'description' => 'Tertiary title',
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_tertiary_title',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Tertiary title',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'book_chapter' => array(
        'label' => 'Series Title',
      ),
      'bill' => array(
        'label' => 'Legislative Body',
      ),
      'chart' => array(
        'label' => 'Name of File',
      ),
      'conference_proceedings' => array(
        'label' => 'Series Title',
      ),
      'government_report' => array(
        'label' => 'Series Title',
      ),
      'hearing' => array(
        'label' => 'Legislative Body',
      ),
    ),
  );

  // Refereed designation.
  $items['biblio_refereed_designation'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_refereed_designation',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Refereed Designation',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    // @todo: Applies to all?
    'bundles_info' => array(
      'artwork' => array(),
      'audiovisual' => array(),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(),
      'chart' => array(),
      'classical' => array(),
      'conference_paper' => array(),
      'conference_proceedings' => array(),
      'database' => array(),
      'film' => array(),
      'government_report' => array(),
      'hearing' => array(),
    ),
  );

  // Original publication.
  $items['biblio_original_publication'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_original_publication',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Original Publication',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
    'bundles_info' => array(
      'audiovisual' => array(
        'label' => 'Contents',
      ),
      'book' => array(),
      'book_chapter' => array(),
      'bill' => array(
        'label' => 'History',
      ),
      'case' => array(
        'label' => 'History',
      ),
      'classical' => array(),
      'hearing' => array(
        'label' => 'History',
      ),
    ),
  );

  // Publication status.
  $items['biblio_status'] = array(
    'field' => array(
      'field_name' => 'biblio_status',
      'type' => 'list_text',
      'cardinality' => 1,
      'settings' => array(
        'allowed_values' => array(
          'in_press' => 'In press',
          'submitted' => 'Submitted',
          'published' => 'Published',
        ),
      ),
    ),
    'instance' => array(
      'label' => t('Publication status'),
      'description' => t('Determine the publication status (In press, Submitted, Published, etc\').'),
      'display_label' => 1,
      'widget' => array(
        'module' => 'options',
        'type' => 'options_select',
        'weight' => 0,
        'settings' => array(
          'display_label' => TRUE,
        ),
      ),
    ),
  );

  // Language field.
  $items['biblio_language'] = array(
    'field' => array(
      'cardinality' => '1',
      'field_name' => 'biblio_language',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'type' => 'text',
    ),
    'instance' => array(
      'label' => 'Language',
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '-3',
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_biblio_types_info().
 */
function biblio_biblio_types_info() {
  $types['audiovisual'] = array('name' => 'Audiovisual');
  $types['artwork'] = array('name' => 'Artwork');
  $types['bill'] = array('name' => 'Bill');
  $types['book'] = array('name' => 'Book');
  $types['book_chapter'] = array('name' => 'Book Chapter');
  $types['broadcast'] = array('name' => 'Broadcast');
  $types['case'] = array('name' => 'Case');
  $types['chart'] = array('name' => 'Chart');
  $types['classical'] = array('name' => 'Classical');
  $types['conference_paper'] = array('name' => 'Conference Paper');
  $types['conference_proceedings'] = array('name' => 'Conference Proceedings');
  $types['database'] = array('name' => 'Database');
  $types['film'] = array('name' => 'Film');
  $types['government_report'] = array('name' => 'Government Report');
  $types['hearing'] = array('name' => 'Hearing');
  $types['journal'] = array('name' => 'Journal');
  $types['journal_article'] = array('name' => 'Journal Article');
  $types['legal_ruling'] = array('name' => 'Legal Ruling');
  $types['map'] = array('name' => 'Map');
  $types['manuscript'] = array('name' => 'Manuscript');
  $types['magazine_article'] = array('name' => 'Magazine Article');
  $types['miscellaneous'] = array('name' => 'Miscellaneous');
  $types['miscellaneous_section'] = array('name' => 'Miscellaneous Section');
  $types['newspaper_article'] = array('name' => 'Newspaper Article');
  $types['patent'] = array('name' => 'Patent');
  $types['personal'] = array('name' => 'Personal');
  $types['presentation'] = array('name' => 'Presentation');
  $types['report'] = array('name' => 'Report');
  $types['software'] = array('name' => 'Software');
  $types['statute'] = array('name' => 'Statute');
  $types['thesis'] = array('name' => 'Thesis');
  $types['unpublished'] = array('name' => 'Unpublished');
  $types['website'] = array('name' => 'Website');
  $types['web_article'] = array('name' => 'Web Article');
  $types['web_project_page'] = array('name' => 'Web project page');
  $types['web_service'] = array('name' => 'Web service');

  return $types;
}

?>

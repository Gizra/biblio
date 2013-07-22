<?php

/**
 * @file
 * Defines the inline entity form controller for Biblio contributor.
 */

class BiblioContributorInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   */
  public function tableFields($bundles) {
    $fields = array();
    $fields['lastname'] = array(
      'type' => 'property',
      'label' => t('Last name'),
      'weight' => 1,
    );
    $fields['firstname'] = array(
      'type' => 'property',
      'label' => t('First name'),
      'weight' => 2,
    );
    $fields['initials'] = array(
      'type' => 'property',
      'label' => t('Initials'),
      'weight' => 3,
    );

    return $fields;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $biblio_contributor = $entity_form['#entity'];

    $properties = array(
      'prefix',
      'firstname',
      'initials',
      'lastname',
      'suffix',
    );

    foreach ($properties as $property) {
      $biblio_contributor->{$property} = !empty($biblio_contributor->{$property}) ? $biblio_contributor->{$property} : '';
    }

    $entity_form['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => $biblio_contributor->prefix,
      '#maxlength' => 128,
    );

    $entity_form['firstname'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#default_value' => $biblio_contributor->firstname,
      '#maxlength' => 128,
    );

    $entity_form['initials'] = array(
      '#type' => 'textfield',
      '#title' => t('Initials'),
      '#default_value' => $biblio_contributor->initials,
      '#maxlength' => 10,
    );

    $entity_form['lastname'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#default_value' => $biblio_contributor->lastname,
      '#maxlength' => 128,
    );

    $entity_form['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#default_value' => $biblio_contributor->suffix,
      '#maxlength' => 128,
    );



    field_attach_form('biblio_contributor', $biblio_contributor, $entity_form, $form_state);

    // Add all fields to the main fieldset.
    foreach (field_info_instances('biblio_contributor', $biblio_contributor->type) as $instance) {
      $entity_form[$instance['field_name']]['#fieldset'] = 'details';
    }

    return $entity_form;
  }

  /**
   * Returns the settings form for the current entity type.
   *
   * The settings form is embedded into the IEF widget settings form.
   * Settings are later injected into the controller through $this->settings.
   *
   * @param $field
   *   The definition of the reference field used by IEF.
   * @param $instance
   *   The definition of the reference field instance.
   */
  public function settingsForm($field, $instance) {
    $labels = $this->labels();

    $form = array();
    $form['allow_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow users to add existing @label.', array('@label' => $labels['plural'])),
      '#default_value' => $this->settings['allow_existing'],
    );
    $form['match_operator'] = array(
      '#type' => 'select',
      '#title' => t('Autocomplete matching'),
      '#default_value' => $this->settings['match_operator'],
      '#options' => array(
        'STARTS_WITH' => t('Starts with'),
        'CONTAINS' => t('Contains'),
      ),
      '#description' => t('Select the method used to collect autocomplete suggestions. Note that <em>Contains</em> can cause performance issues on sites with thousands of nodes.'),
      '#states' => array(
        'visible' => array(
          ':input[name="instance[widget][settings][type_settings][allow_existing]"]' => array('checked' => TRUE),
        ),
      ),
    );
    // The single widget doesn't offer autocomplete functionality.
    if ($instance['widget']['type'] == 'inline_entity_form_single') {
      $form['allow_existing']['#access'] = FALSE;
      $form['match_operator']['#access'] = FALSE;
    }

    return $form;
  }
}

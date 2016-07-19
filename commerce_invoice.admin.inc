<?php
/**
 * @file
 * Administrative form and page callbacks for the Commerce Invoice module.
 */

/**
 * Form callback for an invoice number pattern.
 *
 * @param array $form
 * @param array &$form_state
 * @param \Drupal\commerce_invoice\Entity\InvoiceNumberPattern $pattern
 * @param string $op
 *
 * @return array
 */
function commerce_invoice_number_pattern_form($form, &$form_state, \Drupal\commerce_invoice\Entity\InvoiceNumberPattern $pattern, $op = 'edit') {
  $form['label'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $pattern->label,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $pattern->name,
    '#disabled' => entity_has_status('commerce_invoice_number_pattern', $pattern, ENTITY_IN_CODE) || $op === 'edit',
    '#machine_name' => array(
      'exists' => 'commerce_invoice_number_pattern_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this pattern. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern'),
    '#description' => t('The pattern for invoice numbers.')
      . '<br />' . t('By default an integer will be appended. Include the special token <code>@sequence_token</code> to place the integer manually.', array(
        '@sequence_token' => \Drupal\commerce_invoice\InvoiceNumber\InvoiceNumber::SEQUENCE_TOKEN,
      ))
      . '<br />' . t('This integer is calculated sequentially based on the pattern (after system tokens have been replaced).'),
    '#default_value' => $pattern->pattern,
    '#required' => TRUE,
  );

  $form['token_help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('date', 'site'),
    '#global_types' => FALSE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form API submit callback for the pattern form.
 */
function commerce_invoice_number_pattern_form_submit(&$form, &$form_state) {
  /** @var \Drupal\commerce_invoice\Entity\InvoiceNumberPattern $pattern */
  $pattern = entity_ui_form_submit_build_entity($form, $form_state);
  $pattern->save();
  $form_state['redirect'] = 'admin/commerce/config/invoice-numbers';
}
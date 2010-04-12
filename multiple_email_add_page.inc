<?php
// $Id: multiple_email_add_page.inc,v 1.1.2.1 2007/12/18 04:42:23 joshbenner Exp $

/**
 * @file
 * Functions for the page to add a new email address to the registry.
 */

 /**
  * Renders the page for adding a new email address
  *
  * @return string
  */
function multiple_email_add_page($account) {
  menu_set_active_item('user/'. $account->uid .'/edit/email-addresses');
  return drupal_get_form('multiple_email_add_form', $account);
}

/**
 * Builds form for adding a new email address
 *
 * @ingroup forms
 * @param object $account User object
 */
function multiple_email_add_form(&$form_state, $account) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#required' => TRUE,
  );
  
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );
  
  if (user_access('administer multiple emails')) {
    $form['confirm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Activate this email.'),
    );
  }
  
   $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );

  return $form;
}

/**
 * Validates multiple_email_add_form
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_add_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('You must enter a valid email address!'));
  }
  elseif (multiple_email_find_address($form_state['values']['email'])) {
    form_set_error('email', t('Entered address is already registered on this site.'));
  }
}

/**
 * Processes form submission of multiple_email_add_form
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_add_form_submit($form, &$form_state) {
  dsm($form_state['values']['confirm']);
  $account = $form_state['values']['account'];
  $eid = multiple_email_register_email($account->uid, $form_state['values']['email']);

  if($eid) {
    if(user_access('administer multiple emails') && $form_state['values']['confirm'] == TRUE) {
      $email = multiple_email_get_address($eid);
      multiple_email_confirm_email($email);
      drupal_set_message("The address {$email->email} has been confirmed!");
    } else {
      multiple_email_send_confirmation($account, multiple_email_get_address($eid));
      drupal_set_message(t("The email address '%email' has been added to your account and is awaiting confirmation.",
      array('%email' => $form_state['values']['email'])));
    }
  }
  else {
    form_set_error('', t("Error attempting to register '%email'", array('%email' => $form_state['values']['email'])));
  }
}

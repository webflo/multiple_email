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
function multiple_email_add_page() {
  global $user;

  $out = '';
  $out .= drupal_get_form('multiple_email_add_form', $user);

  return $out;
}

/**
 * Builds form for adding a new email address
 *
 * @ingroup forms
 * @param object $account User object
 */
function multiple_email_add_form($account) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#required' => true,
  );
  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );
  $url = base_path() . 'my-email-addresses';
  $form[] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#submit' => false,
    '#attributes' => array('onclick'=>"window.location='$url';return false;")
  );

  return $form;
}

/**
 * Validates multiple_email_add_form
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_add_form_validate($form_id, $form_values) {
  if (!valid_email_address($form_values['email'])) {
    form_set_error('email', t('You must enter a valid email address!'));
  } elseif (multiple_email_find_address($form_values['email'])) {
    form_set_error('email', t('Entered address is already registered on this site.'));
  }
}

/**
 * Processes form submission of multiple_email_add_form
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_add_form_submit($form_id, $form_values) {
  global $user;

  if ($eid = multiple_email_register_email($user->uid, $form_values['email'])) {
    multiple_email_send_confirmation($user, multiple_email_get_address($eid));
    drupal_set_message(t("The email address '%email' has been added to your account and is awaiting confirmation.",
    array('%email'=>$form_values['email'])));
  } else {
    drupal_set_message(t("Error attempting to register '%email'", array('%email'=>$form_values['email'])));
  }
  drupal_goto('my-email-addresses');
}
?>
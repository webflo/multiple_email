<?php
// $Id: multiple_email_primary_page.inc,v 1.1.2.1 2007/12/18 04:42:22 joshbenner Exp $

/**
 * @file
 * Functions for making an email address the primary address for a user
 */

 /**
  * Renders page to confirm making an address primary
  *
  * @param integer $eid
  */
function multiple_email_primary_page($account, $eid) {
  $email = multiple_email_get_address($eid);

  menu_set_active_item('user/'. $account->uid .'/edit/email-addresses');
  return drupal_get_form('multiple_email_primary_form', $account, $email);
}

/**
 * Builds form for confirming moving an email to primary status
 *
 * @ingroup forms
 * @see multiple_email_primary_form_validate()
 * @see multiple_email_primary_form_submit()
 *
 * @param object $email
 *   And email object from the database
 *
 * @return array
 */
function multiple_email_primary_form(&$form_state, $account, $email) {
  $form = array();
  $form['#redirect'] = 'user/'. $account->uid .'/edit/email-addresses';

  $form[] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t("Are you sure you wish to make the address '{$email->email}' your primary email address?") . '</p>',
  );

  $form['email'] = array(
    '#type' => 'value',
    '#value' => $email,
  );

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Make Primary'),
  );

  return $form;
}

/**
 * Processes multiple_email_primary_form submission
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_primary_form_submit($form, &$form_state) {
  $email = $form_state['values']['email'];
  $account = $form_state['values']['account'];
  
  if ($email->confirmed == 0) {
     // if admin set address as primary, confirm email
     multiple_email_confirm_email($email);
  }

  multiple_email_make_primary($email);
  drupal_set_message("'{$email->email}' is now your primary email address.");
  $form_state['redirect'] = 'user/'. $account->uid .'/edit/email-addresses';
}

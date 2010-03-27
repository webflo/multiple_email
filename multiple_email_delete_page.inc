<?php
// $Id: multiple_email_delete_page.inc,v 1.1.2.1 2007/12/18 04:42:22 joshbenner Exp $

/**
 * @file
 * Contains functions the interface side of deleting an address
 */

 /**
  * Shows confirmation that you want to delete the specified email address
  *
  * If the second argument is true, then it will just go ahead and delete.
  *
  * @param integer $eid
  *   ID of the email to be deleted
  *
  * @return string
  */
function multiple_email_delete_page($account, $eid) {
  $email = multiple_email_get_address($eid);

  menu_set_active_item('user/'. $account->uid .'/edit/email-addresses');
  return drupal_get_form('multiple_email_delete_form', $account, $email);
}

/**
 * Form displayed to confirm deletion of an email address from the registry
 *
 * @ingroup forms
 * @see multiple_email_delete_form_validate()
 * @see multiple_email_delete_form_submit()
 *
 * @param object $email
 *   Email object from the database
 *
 * @return string
 */
function multiple_email_delete_form(&$form_state, $account, $email) {
  $form = array();
  $form['#redirect'] = 'user/'. $account->uid .'/edit/email-addresses';

  $form[] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t("Are you sure you wish to delete the address '%email' from your user account?", array('%email' => $email->email)) .'</p>'
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
    '#value' => 'Delete',
  );

  return $form;
}

/**
 * Processes mulitple_email_delete_form_submit
 *
 * This is where users-input triggers deletion!
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_delete_form_submit($form, &$form_state) {
  $email = $form_state['values']['email'];
  $account = $form_state['values']['account'];
  
  multiple_email_delete_email($email->eid);
  drupal_set_message("The email address '{$email->email}' has been removed from your account.");
  $form_state['redirect'] = 'user/'. $account->uid .'/edit/email-addresses';
}

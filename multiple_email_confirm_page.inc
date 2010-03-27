<?php
// $Id: multiple_email_confirm_page.inc,v 1.1.2.2 2008/01/07 18:14:57 joshbenner Exp $

/**
 * @file
 * Functions for displaying and processing the confirmation page
 */

 /**
  * Renders the page to confirm an email address
  *
  * If code is passed in on URL, then it will populate that value in the text
  * field.
  *
  * @param integer $eid
  * @param string $code
  * @return string
  */
function multiple_email_confirm_page($account, $eid, $code = NULL) {
  $email = multiple_email_get_address($eid);

  menu_set_active_item('user/'. $account->uid .'/edit/email-addresses');
  return drupal_get_form('multiple_email_confirm_form', $account, $email, $code);
}

/**
 * Builds email confirmation form
 *
 * @ingroup forms
 *
 * @see multiple_email_confirm_form_submit()
 *
 * @param object $email
 * @param string $confirm_code
 * @return array
 */
function multiple_email_confirm_form(&$form_state, $account, $email, $confirm_code) {
  $form = array();

  $form['#redirect'] = 'user/'. $account->uid .'/edit/email-addresses';

  $form[] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t("The email address '%email' is awaiting confirmation. You should have received an email at that address with a confirmation code in it. Enter the code below and click confirm.", array('%email' => $email->email)) . '</p>',
  );

  $form['email'] = array(
    '#type' => 'value',
    '#value' => $email,
  );

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation Code'),
    '#required' => TRUE,
    '#default_value' => $confirm_code,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );

  return $form;
}

function multiple_email_confirm_form_validate($form, &$form_state) {
  $code = trim($form_state['values']['code']);
  
  $email = $form_state['values']['email'];
  $account = $form_state['values']['account'];

  if ($code != '') {
    $attempts = $email->attempts + 1;
    $allowed = (int)variable_get('multiple_email_confirm_attempts', 3);
    db_query("UPDATE {multiple_email} SET attempts = %d WHERE eid = %d", $attempts, $email->eid);

    if ($attempts <= $allowed) {
      if ($code != $email->confirm_code) {
        form_set_error('', 'The confirmation code was incorrect');
      }
    }
    else {
      $email->confirm_code = multiple_email_code(10);
      db_query("UPDATE {multiple_email} SET confirm_code = '%s', time_code_generated = %d, attempts = 0 WHERE eid = %d", $email->confirm_code, time(), $email->eid);
      multiple_email_send_confirmation($account->uid, $email);

      // Redirect & Message
      form_set_error('', 'You have exhausted your allowed attempts at confirming this email address. A new confirmation code has been sent.');
      drupal_goto('user/'. $account->uid .'/edit/email-addresses');
    }
  }
}

/**
 * Process multiple_email_confirm_form submission
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_confirm_form_submit($form, &$form_state) {
  // Confirmation successful!
  $email = $form_state['values']['email'];
  multiple_email_confirm_email($email);
  drupal_set_message("The address '{$email->email}' has been confirmed!");
}

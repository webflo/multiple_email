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
function multiple_email_confirm_page($eid, $code = null) {
  global $user;

  $out = '';

  if ($email = multiple_email_get_address($eid)) {
    if ($email->uid != $user->uid) {
      drupal_set_message(t('Email address not found'));
      watchdog('Multiple Email', 'Unauthorized attempt to access email ' . $eid . ' by ' . $user->name . ' (' . $user->uid . ')');
      drupal_goto('my-email-addresses');
    } elseif ($email->confirmed) {
      drupal_set_message(t("'%email' is already confirmed!", array('%email'=>$email->email)));
      drupal_goto('my-email-addresses');
    } else {
      $out .= drupal_get_form('multiple_email_confirm_form', $email, $code);
    }
  } else {
    drupal_set_message(t('Email address not found'));
    watchdog('Multiple Email', 'Error loading email ' . $eid, WATCHDOG_WARNING);
    drupal_goto('my-email-addresses');
  }

  return $out;
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
function multiple_email_confirm_form($email, $confirm_code) {
  $form['eid'] = array(
    '#type' => 'hidden',
    '#value' => $email->eid,
  );
  $form[] = array(
    '#prefix' => '<p>',
    '#value' => t("The email address '%email' is awaiting confirmation. You should have received an email at that address with a confirmation code in it. Enter the code below and click confirm.",
  array('%email'=>$email->email)),
    '#suffix' => '</p>',
  );
  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation Code'),
    '#required' => true,
    '#default_value' => $confirm_code,
  );
  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
  );
  $url = base_path() . 'my-email-addresses';
  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => 'Cancel',
    '#attributes' => array('onclick'=>"window.location='$url'; return false;"),
  );

  return $form;
}

/**
 * Process multiple_email_confirm_form submission
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_confirm_form_submit($form_id, $form_values) {
  global $user;

  if ($email = multiple_email_get_address($form_values['eid'])) {
    if ($user->uid != $email->uid) {
      watchdog('Multiple Email', "Attempted unauthorized access to email {$email->eid} by user {$user->name} ({$user->uid})");
    } elseif ($email->confirmed) {
      drupal_set_message("'{$email->email}' is already confirmed!");
    } elseif (trim($form_values['code']) != $email->confirm_code) {
      $attempts = $email->attempts + 1;
      $allowed = (int)variable_get('multiple_email_confirm_attempts', 3);
      if ($allowed && $attempts >= $allowed) {
        $email->confirm_code = multiple_email_code(10);
        db_query("
          UPDATE {multiple_email} SET
            confirm_code='%s',
            time_code_generated=%d,
            attempts=0
          WHERE
            eid = %d",
        $email->confirm_code,
        time(),
        $email->eid
        );
        multiple_email_send_confirmation($user, $email);
        drupal_set_message('You have exhausted your allowed attempts at confirming this email address. A new confirmation code has been sent.');
      } else {
        db_query("UPDATE {multiple_email} SET attempts=%d WHERE eid=%d", $attempts, $email->eid);
        drupal_set_message('The confirmation code was incorrect');
      }
    } else {
      // Confirmation successful!
      multiple_email_confirm_email($email);
      drupal_set_message("The address '{$email->email}' has been confirmed!");
      drupal_goto('my-email-addresses');
    }
  } else {
    watchdog('Multiple Email', 'Error loading email ' . $form_values['eid'], WATCHDOG_WARNING);
  }
  drupal_goto("my-email-addresses/confirm/{$form_values['eid']}");
}
?>
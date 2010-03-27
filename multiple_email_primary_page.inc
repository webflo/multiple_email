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
function multiple_email_primary_page($eid) {
  global $user;

  if ($email = multiple_email_get_address($eid)) {
    if ($user->uid == $email->uid) {
      if ($email->primary_address) {
        drupal_set_message("'{$email->email}' is already your primary address!");
      } else {
        return drupal_get_form('multiple_email_primary_form', $email);
      }
    } else {
      watchdog('Multiple Email', 'Attempted unauthorized access to email ' . $eid . ' by user ' . $user->uid);
    }
  } else {
    watchdog('Multiple Email', 'Failed to load address ' . $eid, WATCHDOG_WARNING);
  }

  drupal_goto('my-email-addresses');
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
function multiple_email_primary_form($email) {
  $form['eid'] = array(
    '#type' => 'hidden',
    '#value' => $email->eid,
  );
  $form[]= array(
    '#prefix' => '<p>',
    '#value' => "Are you sure you wish to make the address '{$email->email}' your primary email address?",
    '#suffix' => '</p>',
  );
  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => 'Make Primary',
  );
  $url = base_path() . 'my-email-addresses';
  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => 'Cancel',
    '#attributes' => array('onclick'=>"window.location='$url';return false;"),
  );

  return $form;
}

/**
 * Validates multiple_email_primary_form
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_primary_form_validate($form_id, $form_values) {
  global $user;

  if ($email = multiple_email_get_address($form_values['eid'])) {
    if ($user->uid != $email->uid) {
      watchdog('Multiple Email', 'Attempted unauthorized access to email ' . $form_values['eid'] . ' by user ' . $user->uid);
      drupal_goto('my-email-addresses');
    } elseif ($email->primary_address) {
      drupal_set_message("'{$email->email}' is already your primary address!");
      drupal_goto('my-email-addresses');
    }
  } else {
    watchdog('Multiple Email', 'Invalid eid value: ' . $form_values['eid'], WATCHDOG_WARNING);
    drupal_goto('my-email-addresses');
  }
}

/**
 * Processes multiple_email_primary_form submission
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_primary_form_submit($form_id, $form_values) {
  global $user;

  $email = multiple_email_get_address($form_values['eid']);

  multiple_email_make_primary($user, $email);
  drupal_set_message("'{$email->email}' is now your primary email address.");
  drupal_goto('my-email-addresses');
}
?>
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
function multiple_email_delete_page($eid) {
  global $user;

  if ($email = multiple_email_get_address($eid)) {
    if ($user->uid == $email->uid) {
      if ($email->primary_address) {
        drupal_set_message(t('You can not delete your primary email address!'));
      } else {
        return drupal_get_form('multiple_email_delete_form', $email);
      }
    } else {
      drupal_set_message(t('Email address not found'));
      watchdog('Multiple Email', 'Attempted unauthorized access to email ' . $eid . ' by user ' . $user->uid);
    }
  } else {
    drupal_set_message(t('Email address not found'));
    watchdog('Multiple Email', 'Failed to load address ' . $eid, WATCHDOG_WARNING);
  }

  drupal_goto('my-email-addresses');
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
function multiple_email_delete_form($email) {
  $form['eid'] = array(
    '#type' => 'hidden',
    '#value' => $email->eid,
  );
  $form[]= array(
    '#prefix' => '<p>',
    '#value' => t("Are you sure you wish to delete the address '%email' from your user account?", array('%email'=>$email->email)),
    '#suffix' => '</p>',
  );
  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
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
 * Validates multiple_email_delete_form
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_delete_form_validate($form_id, $form_values) {
  global $user;

  if ($email = multiple_email_get_address($form_values['eid'])) {
    if ($user->uid != $email->uid) {
      watchdog('Multiple Email', 'Attempted unauthorized access to email ' . $form_values['eid'] . ' by user ' . $user->uid);
      drupal_goto('my-email-addresses');
    } elseif ($email->primary_address) {
      drupal_set_message('You cannot delete your primary email address!');
      drupal_goto('my-email-addresses');
    }
  } else {
    watchdog('Multiple Email', 'Invalid eid value: ' . $form_values['eid'], WATCHDOG_WARNING);
    drupal_goto('my-email-addresses');
  }
}

/**
 * Processes mulitple_email_delete_form_submit
 *
 * This is where users-input triggers deletion!
 *
 * @param string $form_id
 * @param array $form_values
 */
function multiple_email_delete_form_submit($form_id, $form_values) {
  $email = multiple_email_get_address($form_values['eid']);
  multiple_email_delete_email($form_values['eid']);
  drupal_set_message("The email address '{$email->email}' has been removed from your account.");
  drupal_goto('my-email-addresses');
}
?>
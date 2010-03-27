<?php
// $Id: multiple_email_addresses_page.inc,v 1.1.2.3 2008/01/23 15:13:16 joshbenner Exp $

/**
 * @file
 * Contains functions for presentation and processing of email address
 * management page
 */

 /**
  * Renders the page to manage user's email addresses
  *
  * @return string
  */
function multiple_email_addresses_page() {  
  global $user;
  
  $out = '';
  
  $out .= drupal_get_form('multiple_email_addresses_form', $user);
  
  return $out;
}

/**
 * Builds form which will output a table of email addresses with management
 * links
 *
 * @ingroup forms
 * @param array $addresses
 * @param array $headers
 */
function multiple_email_addresses_form($account) {
  $addresses = multiple_email_load_addresses($account->uid, $headers);
  unset($addresses[0]);
  
  $headers[] = array(
    'data' => t('Email Address'),
    'field' => 'a.email',
  );
  $headers[] = array(
    'data' => t('Confirmed?'),
    'field' => 'a.confirmed',
  );
  $headers[] = array(
    'data' => t('Primary Address?'),
    'field' => 'primary_address',
    'sort' => 'desc',
  );
  $headers[] = array(
    'data' => t('Remove'),
  );
  
  $form['#headers'] = $headers;
  $form['#columns'] = array('email', 'confirmed', 'primary', 'delete');
  
  $form['add']['#value'] = theme('multiple_email_add_address_link');
  
  foreach ($addresses as $eid=>$a) {
    $aid = "address_$eid";
    $form[$aid]["email_$eid"] = array(
      '#value' => $a->email,
    );
    $form[$aid]["confirmed_$eid"] = array(
      '#value' => $a->confirmed ? 'Yes' : l('Click here to confirm', "my-email-addresses/confirm/$eid"),
    );
    $form[$aid]["primary_$eid"] = array(
      '#value' => $a->primary_address ? 'Primary' : l('Make Primary', "my-email-addresses/make-primary/$eid"),
    );
    $form[$aid]["delete_$eid"] = array(
      '#value' => $a->primary_address ? '' : l('X', "my-email-addresses/delete/$eid"),
    );
  }
  
  return $form;
}

/**
 * Themes multiple_email_addresses_form
 *
 * Basically it builds the table from the data in the form, so you could
 * present the data in a different format by overriding the theme function.
 *
 * @ingroup themable
 * @param array $form
 * @return string
 */
function theme_multiple_email_addresses_form($form) {
  $headers = $form['#headers'];
  $columns = $form['#columns'];
  unset($form['#headers'], $form['#columns']);

  $out = '';
  $rows = array();
  // NOTE: Reference $form instead of $member so that $form values are
  // updated upon being rendered!
  foreach ($form as $aid=>$member) {
    if (preg_match('/address_(\d+)/i', $aid, $matches)) {
      $row = array();
      $eid = $matches[1];
      foreach ($columns as $c) {
        switch ($c) {
          // Should simply this since switch isn't used anymore
          default:
            $row[] = array(
              'data' => drupal_render($form[$aid][$c . '_' . $eid]),
              'class' => "multiple-email-table-$c",
            );
        }
      }
      $rows[] = $row;
    }
  }
  $out .= theme('table', $headers, $rows, array('class'=>'multiple-email-table'));
  $out .= drupal_render($form);
  return $out;
}

/**
 * Themes the link to add a new email address
 * 
 * @ingroup themable
 *
 * @return string
 *  HTML
 */
function theme_multiple_email_add_address_link($text = null) {
  $text = $text ? $text : t('Add Email Address');
  return l($text, 'my-email-addresses/add');
}
?>
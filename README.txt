Module: Multiple Email Addresses
Author: Joshua Benner <joshbenner@gmail.com>
Sponsor: Rock River Star (www.rockriverstar.com) -- My employer

Please note this is a development release. More features and finishing will
come with some time.

The Multiple Email module allows users to register additional emails for their
user accounts. Only one email address is considered to be the "primary" email
address, and will continue to behave as normal. Non-primary accounts are
mostly functionally meaningless, except that during user registration any
email address registered to a user cannot be used to create a new account.

Users may select any confirmed email address to become their primary email
address. This means that the user account edit page's email address field will
not change the user's email address. The default settings for the module will
actually hide the email address field on the user account edit page.

At this time, there is no administrator interface to manage per-user multiple
email address settings. That is to come!

This module was written to fill a small need on the growing number of Drupal-
based social network sites that allow users to "invite" other users based on
their email address. While this module does not implement any invitation code
or integrate directly with any modules that do, it is my hope that these
connections between Multiple Email and other modules will develop to enhance
the usefulness of both.

Once the module is installed, administration settings are available under Site
Configuration -> Multiple Email Settings. The configuration options are rather
straight-forward at this point and are documented in the field descriptions.

The module will create a menu item in the Navigation menu called 'My Email
Addresses' that links to the user's email management page.

Hooks
-----
hook_multiple_email_register($email)
  - $email is the email object that has just been registered
  - Use this hook to perform actions when a user registers an email address
    (but isn't confirmed yet)

hook_multiple_email_confirm($email)
  - $email is the email object that has just been registered
  - Use this hook to perform actions when a user confirms an email address
<?php

/**
 * @file
 * Provides the Google No CAPTCHA administration settings.
 */

/**
 * Form callback; administrative settings for Google No CAPTCHA.
 */
function govi_sdqs_admin_settings() {
  $form['govi_sdqs_general_settings'] = array(
    '#type' => 'fieldset',
    '#description' => t('Actualiza la referencia de los datos con el servidor SDQS.').'</br>',
    '#title' => t('Actualización de datos'),
  );
  $form['govi_sdqs_general_settings']['govi_sdqs_update'] = array(
    '#type' => 'submit',
    '#value' => t('Actualizar Datos SDQS'),
    '#submit' => array('govi_sdqs_admin_update_data'),
  );

  $form['govi_sdqs_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Widget settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('Defines which theme to use for govi_sdqs.'),
    '#options' => array(
      'light' => t('Light (default)'),
      'dark' => t('Dark'),
    ),
    '#default_value' => variable_get('govi_sdqs_theme', 'light'),
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#description' => t('The type of CAPTCHA to serve.'),
    '#options' => array(
      'image' => t('Image (default)'),
      'audio' => t('Audio'),
    ),
    '#default_value' => variable_get('govi_sdqs_type', 'image'),
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_size'] = array(
    '#default_value' => variable_get('govi_sdqs_size', ''),
    '#description' => t('The size of CAPTCHA to serve.'),
    '#options' => array(
      '' => t('Normal (default)'),
      'compact' => t('Compact'),
    ),
    '#title' => t('Size'),
    '#type' => 'select',
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_tabindex'] = array(
    '#type' => 'textfield',
    '#title' => t('Tabindex'),
    '#description' => t('Set the <a href="@tabindex">tabindex</a> of the widget and challenge (Default = 0). If other elements in your page use tabindex, it should be set to make user navigation easier.', array('@tabindex' => 'http://www.w3.org/TR/html4/interact/forms.html#adef-tabindex')),
    '#default_value' => variable_get('govi_sdqs_tabindex', 0),
    '#size' => 4,
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_noscript'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable fallback for browsers with JavaScript disabled'),
    '#default_value' => variable_get('govi_sdqs_noscript', 0),
    '#description' => t('If JavaScript is a requirement for your site, you should <strong>not</strong> enable this feature. With this enabled, a compatibility layer will be added to the captcha to support non-js users.'),
  );

  return system_settings_form($form);
}
function govi_sdqs_admin_update_data() {
  drupal_goto('admin/config/features/sdqs/update');
}
/**
 * Validation function for govi_sdqs_admin_settings().
 *
 * @see govi_sdqs_admin_settings()
 */
function govi_sdqs_admin_settings_validate($form, &$form_state) {
  $tabindex = $form_state['values']['govi_sdqs_tabindex'];
  if (!is_numeric($tabindex)) {
    form_set_error('govi_sdqs_tabindex', t('The tabindex must be an integer.'));
  }
}

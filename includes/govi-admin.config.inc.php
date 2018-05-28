<?php

/**
 * @file
 * Provides the Google No CAPTCHA administration settings.
 */
function govi_sdqs_init(){
// Define static var.
define('DRUPAL_ROOT', getcwd());
// Include bootstrap.
include_once('./includes/bootstrap.inc');
// Initialize stuff.
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// Clear cache.
drupal_flush_all_caches();
}


function govi_sdqs_obtener_info_entidad($form, $form_state) {
  $sdqs =  SdqsClient::getInstance();
  $entity = $form_state['values']['govi_sdqs_entity'];
  $entities_list = $sdqs->getDependencyList($entity);
  reset($entities_list);
  $form['govi_sdqs_widget_settings']['govi_sdqs_dependency']['#options'] = $entities_list;
  return $form['govi_sdqs_widget_settings']['govi_sdqs_dependency'];
}


/**
 * Form callback
 */
function govi_sdqs_admin_settings() {

  $sdqs =  SdqsClient::getInstance();
  if($sdqs->isConnectionConfigured()) {
    $form['govi_sdqs_general_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Actualización de datos'),
    );
    $form['govi_sdqs_general_settings']['govi_sdqs_update'] = array(
      '#type' => 'submit',
      '#value' => t('Actualizar SDQS'),
      '#submit' => array('govi_sdqs_admin_update_data'),
    );
  }


  $form['govi_sdqs_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datos '),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['govi_sdqs_widget_settings']['govi_sdqs_entity'] = array(
    '#type' => 'select',
    '#title' => t('Entidad'),
    '#description' => t('Entidad a la cuál van a ser enviadas las solicitudes'),
    '#options' => variable_get('sdqs_entities'),
    '#ajax' => array(
            'event' => 'change',
            'effect' => 'fade',
            'callback' => 'govi_sdqs_obtener_info_entidad',
            'method' => 'replace',
            'wrapper' => 'wrapper-dependencia'
        ),
    '#default_value' => variable_get('govi_sdqs_entity', 0),
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_dependency'] = array(
    '#type' => 'select',
    '#title' => t('Dependencia'),
    '#description' => t('Entidad a la cuál van a ser enviadas las solicitudes'),
    '#prefix' => '<div id="wrapper-dependencia">',
    '#suffix' => '</div>',
    '#options' => array(),
    '#validated' => TRUE,
    '#default_value' => variable_get('govi_sdqs_dependency', 0),
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
  drupal_goto('admin/config/service-sdqs-update');
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

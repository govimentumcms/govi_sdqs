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


function govi_sdqs_entity_info($form, $form_state) {
  $sdqs =  SdqsClient::getInstance();
  $entity = $form_state['values']['govi_sdqs_entity'];
  reset($entities_list);
  $entities_list = $sdqs->getDependencyList($entity);
  $form['govi_sdqs_widget_settings']['govi_sdqs_dependency']['#options'] = $entities_list;
  return $form['govi_sdqs_widget_settings']['govi_sdqs_dependency'];
}
function govi_sdqs_theme_info($form, $form_state) {
  $sdqs =  SdqsClient::getInstance();
  $entity = $form_state['values']['govi_sdqs_entity'];
  reset($theme_list);
  $theme_list = $sdqs->getThemeList($entity);
  $form['govi_sdqs_widget_settings']['govi_sdqs_theme']['#options'] = $theme_list;
  return $form['govi_sdqs_widget_settings']['govi_sdqs_theme'];
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
  $form['govi_sdqs_widget_settings']['govi_sdqs_sector'] = array(
    '#type' => 'select',
    '#title' => t('Sector'),
    '#description' => t('Sector a la cuál van a ser enviadas las solicitudes'),
    '#options' => variable_get('sdqs_sectors'),
    '#ajax' => array(
      'event' => 'change',
      'effect' => 'fade',
      'callback' => 'govi_sdqs_sector_info',
      'method' => 'replace',
      'wrapper' => 'wrapper-entities'
    ),
    '#default_value' => variable_get('govi_sdqs_sector', 0),
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_entity'] = array(
    '#type' => 'select',
    '#title' => t('Entidad'),
    '#prefix' => '<div id="wrapper-entities">',
    '#suffix' => '</div>',
    '#description' => t('Entidad a la cuál van a ser enviadas las solicitudes'),
    '#options' => variable_get('sdqs_entities'),
    '#ajax' => array(
      'event' => 'change',
      'effect' => 'fade',
      'callback' => 'govi_sdqs_entity_info',
      'method' => 'replace',
      'wrapper' => 'wrapper-dependency'
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('govi_sdqs_entity', 0),
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_dependency'] = array(
    '#type' => 'select',
    '#title' => t('Dependencia'),
    '#description' => t('Dependencia a la cuál van a ser enviadas las solicitudes'),
    '#prefix' => '<div id="wrapper-dependency">',
    '#suffix' => '</div>',
    '#options' => $sdqs->getDependencyList(variable_get('govi_sdqs_entity')),
    '#ajax' => array(
      'event' => 'change',
      'effect' => 'fade',
      'callback' => 'govi_sdqs_theme_info',
      'method' => 'replace',
      'wrapper' => 'wrapper-sdqs-theme'
    ),
    '#validated' => TRUE,
    '#default_value' => variable_get('govi_sdqs_dependency', 0),
  );

  $form['govi_sdqs_widget_settings']['govi_sdqs_theme'] = array(
    '#type' => 'select',
    '#title' => t('Tema'),
    '#description' => t('Tema principal a través del cuál van a ser enviadas las solicitudes'),
    '#prefix' => '<div id="wrapper-sdqs-theme">',
    '#suffix' => '</div>',
    '#options' => $sdqs->getThemeList(variable_get('govi_sdqs_entity')),
    '#validated' => TRUE,
    '#default_value' => variable_get('govi_sdqs_theme', 0),
  );

  return system_settings_form($form);
}
function govi_sdqs_admin_update_data() {
  drupal_goto('admin/config/service-sdqs-update');
}

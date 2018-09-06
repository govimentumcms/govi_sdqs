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

function govi_sdqs_sector_info($form, $form_state) {
  $sdqs =  SdqsClient::getInstance();
  $sector = $form_state['values']['govi_sdqs_sector'];
  $entities_list = variable_get('sdqs_entities');
  $form['govi_sdqs_widget_settings']['govi_sdqs_entity']['#options'] = $entities_list[$sector];
  return $form['govi_sdqs_widget_settings']['govi_sdqs_entity'];
}

function govi_sdqs_entity_info($form, $form_state) {
  $sdqs =  SdqsClient::getInstance();
  $entity = $form_state['values']['govi_sdqs_entity'];
  $entities_list = $sdqs->getDependencyList($entity);
  $form['govi_sdqs_widget_settings']['govi_sdqs_dependency']['#options'] = $entities_list;
  return $form['govi_sdqs_widget_settings']['govi_sdqs_dependency'];
}
function govi_sdqs_theme_info($form, $form_state) {
  $sdqs =  SdqsClient::getInstance();
  $entity = $form_state['values']['govi_sdqs_entity'];
  $theme_list = $sdqs->getThemeList($entity);
  $form['govi_sdqs_widget_settings']['govi_sdqs_theme']['#options'] = $theme_list;
  return $form['govi_sdqs_widget_settings']['govi_sdqs_theme'];
}



/**
 * Form callback
 */
function govi_sdqs_admin_settings() {
  $sdqs =  SdqsClient::getInstance();
  $user = variable_get('govi_sdqs_site_user', FALSE);
  $password = variable_get('govi_sdqs_secret_password', FALSE);
  $enviroment = variable_get('govi_sdqs_site_env', FALSE);
  if(isset($user) && isset($password) && isset($enviroment) && !$sdqs->checkConnection($user, $password, $enviroment)){
    drupal_set_message(t('No se puede establecer comunicación con el servicio web'), 'error');
    drupal_goto('admin/config/services/sdqs');
  }
  if($sdqs->isConnectionConfigured()) {

    $form['govi_sdqs_general_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Actualización de datos'),
      '#description' => t('</p>Utilice este botón para sincronizar los parámetros de las operaciones del servicio. Se recomienda realizar una sincronización periódicamente (por lo menos una vez por semana), ya que algunos parámetros son actualizados con frecuencia en el servicio web.</p>')
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
    //'#required' => TRUE,
    '#default_value' => variable_get('govi_sdqs_sector', 0),
  );
  $form['govi_sdqs_widget_settings']['govi_sdqs_entity'] = array(
    '#type' => 'select',
    '#title' => t('Entidad'),
    '#prefix' => '<div id="wrapper-entities">',
    '#suffix' => '</div>',
    '#description' => t('Entidad a la cuál van a ser enviadas las solicitudes'),
    // define parametro default de la primera entidad de la lista para evitar el
    // aviso de argumento inválido en la iteración de las opciones del selector de
    // entidad
    '#options' => variable_get('sdqs_entities')[variable_get('govi_sdqs_sector',10)],
    '#ajax' => array(
      'event' => 'change',
      'effect' => 'fade',
      'callback' => 'govi_sdqs_entity_info',
      'method' => 'replace',
      'wrapper' => 'wrapper-dependency'
    ),
    '#required' => TRUE,
    '#validated' => TRUE,
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
  $form['govi_sdqs_widget_settings']['sdqs_debug'] = array(
      '#type' => 'checkbox',
      '#title' => 'Modo debug',
      '#description' => 'Al enviar una solicitud muestra los parámetros enviados al servicio web',
      '#return_value' => 1,
      '#default_value' => variable_get('sdqs_debug', 0),
  );
  $form['#validate'][] = 'govi_sdqs_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Validation function for govi_sdqs_admin_settings().
 *
 * @see govi_sdqs_admin_settings()
 */
function govi_sdqs_admin_settings_validate($form, &$form_state) {
  $sdqs =  SdqsClient::getInstance();
  $sector = $form_state['values']['govi_sdqs_sector'];
  $entity = $form_state['values']['govi_sdqs_entity'];
  $dependency = $form_state['values']['govi_sdqs_dependency'];
  $theme = $form_state['values']['govi_sdqs_theme'];
  if(empty($form['govi_sdqs_general_settings']['govi_sdqs_update'])) {
    if($entity==0 ) {
        form_set_error('govi_sdqs_entity','Debe seleccionar una entidad.');
    }
    if($entity!=0 && !in_array($entity, array_keys($sdqs->getEntityList(variable_get('govi_sdqs_sector')))) ) {
        form_set_error('govi_sdqs_entity','La entidad seleccionada no se encuentra en ese sector.');
    }
    if($dependency!=0 && !in_array($entity, array_keys($sdqs->getDependencyList(variable_get('govi_sdqs_entity')))) ) {
        form_set_error('govi_sdqs_entity','La dependencia seleccionada no es válida para la entidad.');
    }
  }

}
function govi_sdqs_admin_update_data() {
  drupal_goto('admin/config/service-sdqs-update');
}

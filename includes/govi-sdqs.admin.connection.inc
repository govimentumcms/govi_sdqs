<?php

/**
 * @file
 * Provides the form for updating conection settings.
 */

 /**
 * Implements hook_form_alter().
 */

$path = drupal_get_path('module', 'govi_sdqs');
require_once DRUPAL_ROOT . "/$path/handlers/sdqs_handler.inc";


function govi_sdqs_form_alter(&$form, &$form_state, $form_id) {
  $sdqs =  SdqsClient::getInstance();

  if($form_id == 'govi_sdqs_admin_connection') {
    if(!$sdqs->isConnectionConfigured()){
      drupal_set_message(t('Aún no se ha configurado la conexión. Ingrese las credenciales.'), 'warning');
    }
  }
}

/**
* Form callback; administrative connection settings
*/
function govi_sdqs_admin_connection() {
  $form['govi_sdqs_general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datos de conexión'),
  );
  $form['govi_sdqs_general_settings']['govi_sdqs_site_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Usuario'),
    '#default_value' => variable_get('govi_sdqs_site_user', ''),
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['govi_sdqs_general_settings']['govi_sdqs_secret_password'] = array(
    '#type' => 'password',
    '#title' => t('Contraseña'),
    '#default_value' => variable_get('govi_sdqs_secret_password', ''),
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['govi_sdqs_general_settings']['govi_sdqs_site_env'] = array(
      '#type' => 'radios',
      '#title' => 'Seleccione el ambiente del servicio web a usar',
      '#description' => 'Utilize el ambiente de pruebas para efectos de depuración y mantenimiento del cliente',
      '#options' => array(
          'prod' => 'Producción',
          'test' => 'Pruebas',
      ),
      '#required' => TRUE,
      '#default_value' => variable_get('govi_sdqs_site_env', 'test'),
  );

  $form['govi_sdqs_general_settings']['help'] = array(
    '#markup' => t('Las credenciales de acceso deben ser solicitadas a través del <a href="mailto:@email">canal de soporte</a> de la Dirección Distrital de Calidad del Servicio.', array('@email' => 'sdqssoporte@alcaldiabogota.gov.co')),
  );
  global $base_url;
  global $base_path;
  $form['govi_sdqs_general_settings']['sdqs_form'] = array(
    '#markup' => t('<p><a href="@url">Formulario SDQS</a>.</p>', array('@url' => $base_url . $base_path . 'govi-sdqs/crear')),
  );
  $form = system_settings_form($form);
  $form['#validate'][] = 'govi_sdqs_admin_connection_validate';
  // $form['#submit'][] = 'govi_sdqs_admin_connection_submit';
  return $form;
}

/**
 * FIXME: variables are not sent to this stage from previous form submit
 * It needs to check a way to intercept submission to add variables,
 * so this method should be disabled as the user must go to the
 * configuration page to update webservice operations
 */
//function govi_sdqs_admin_connection_submit($form, &$form_state){
//  drupal_goto('/admin/config/service-sdqs-update');
//}

/**
 * Validation function for govi_sdqs_admin_settings().
 *
 * @see govi_sdqs_admin_settings()
 */
function govi_sdqs_admin_connection_validate($form, &$form_state) {
  $sdqs =  SdqsClient::getInstance();
  $user = $form_state['values']['govi_sdqs_site_user'];
  $password = $form_state['values']['govi_sdqs_secret_password'];
  $enviroment = $form_state['values']['govi_sdqs_site_env'];

  if (!empty($enviroment)) {
    variable_set('govi_sdqs_site_env', $enviroment);
    $sdqs->enviroment = $enviroment;
  }

  if(empty($password) || ! $sdqs->checkConnection($user, $password, $enviroment)){
    form_set_error('govi_sdqs_general_settings','Error estableciendo conexión con el servicio web SDQS. Revise sus credenciales.');
  }
}

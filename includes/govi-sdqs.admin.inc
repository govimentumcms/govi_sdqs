<?php

function govi_sdqs_admin_config_main_form($form, &$form_state) {

    $form = array();

    $form['wsdl_config'] = array(
        '#type' => 'fieldset',
        '#title' => 'Configuracíón Básica',
    );

    $form['wsdl_config']['intro'] = array(
        '#markup' => '<p>El módulo Govi SDQS (govi_sdqs) es una implementación del cliente Soap del Sistema Distrital de Quejas y Soluciones SDQS que permite generar peticiones básicas por parte de la ciudadanía</p><p>Esta sección administrativa le permitirá actualizar y almacenar las codificaciones e identificadores del de las tipologías definidas en el We Service del SDQS. Por favor tenga en cuenta las siguientes pautas si es la primera vez que instala este módulo:
<ul><li>Recuerde ingresar el nombre de usuario y contraseña asignado a su Entidad apra acceder a través del sistema de autenticación básico del Web Service.</li><li>Selecionar los códigos e identificadores que desea crear o actualizar</li><li>Seleccione enviar para autenticar y obtener los datos</li></p>',
    );

    $form['wsdl_config']['auth_user'] = array(
        '#type' => 'textfield',
        '#title' => 'Usuario',
        '#description' => 'Nombre del usuario SDQS asignado para la autenticación básica en el ambiente de producción del web service',
        '#required' => TRUE,
        '#default_value' => variable_get('sdqs_user', ''),
    );

    // TODO: Encontrar el modo de usar el campo tipo password y validar si se ha definido
    // la contraseña del webservice, por el momento se maneja un campo de texto

    $form['wsdl_config']['auth_password'] = array(
        '#type' => 'textfield',
        '#title' => 'Contraseña',
        '#description' => 'Contraseña del usuario SDQS asignado para la autenticación básica en el ambiente de producción del web service',
        '#required' => TRUE,
        '#default_value' => variable_get('sdqs_password'),
    );

    /**
    // TODO: Encontrar el modo de usar el campo tipo password y validar si se ha definido
    // la contraseña del webservice

    $form['wsdl_config']['auth_password_button'] = array(
        '#type' => 'button',
        '#prefix' => '<div id="change-password-btn">',
        '#suffix' => '</div>',
        '#value' => 'Cambiar contraseña',
        '#ajax' => array(
            'callback' => 'password_button_ajax',
            'event' => 'click',
            'method' => 'replace',
            'effect' => 'fade',
            'wrapper' => 'change-password-btn'
        ),
    );
    **/

    $form['wsdl_config']['sdqs_env'] = array(
        '#type' => 'radios',
        '#title' => 'Seleccione el ambiente del webservice que desea usar',
        '#description' => 'Utilize el ambiente de pruebas para efectos de depuración y mantenimiento del cliente',
        '#options' => array(
            'prod' => 'Producción',
            'test' => 'Pruebas',
        ),
        '#required' => TRUE,
        '#default_value' => variable_get('sdqs_env', 'test'),
    );

    $form['wsdl_config']['get_codes'] = array(
        '#type' => 'checkboxes',
        '#options' => drupal_map_assoc(array(
            'getListaEntidadesDistritales',
            'getListaTipoPeticion',
            'getListaTema',
            'getListaTipoIdentificacion'
        )),
        '#title' => 'Seleccione las operaciones en los cuales desea actualizar la codificación del Web Service (Estos valores serán almacendados como variables enel sistema de Drupal)',
    );

    $entidades = variable_get('govi_sdqs_lista_entidades');
    $opts_entidades = array();

    foreach ($entidades as $entidad) {
        $opts_entidades[$entidad['id']] = $entidad['nombre'];
    }

    $form['wsdl_config']['codigo_entidad'] = array(
        '#type' => 'select',
        '#title' => 'Por favor seleccione la entidad a la cual pertenece este sitio',
        '#description' => 'Este valor será usado en la petición realizada por el ciudadano',
        '#options' => !empty($opts_entidades) ? $opts_entidades : array(),
        '#required' => TRUE,
        '#default_value' => variable_get('sdqs_entidad'),
    );

    $form['wsdl_config']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Guardar',
    );


    return $form;
}

function govi_sdqs_admin_config_main_form_submit($form, &$form_state) {

    $user = $form_state['values']['auth_user'];
    $passwd = $form_state['values']['auth_password'];
    $env = $form_state['values']['sdqs_env'];
    $opts = $form_state['values']['get_codes'];
    $entidad = $form_state['values']['codigo_entidad'];

    variable_set('sdqs_user', $user);
    variable_set('sdqs_password', $passwd);
    variable_set('sdqs_env', $env);
    variable_set('sdqs_entidad', $entidad);

    $services = _create_services_descriptions($user, $passwd, $env);

    // TODO: Services debería ser una variable con la lista de 
    // servicios disponibles

    if (!empty($opts)) {
        _get_operations_data($opts, $services);
    }
}

/**
 // TODO: Encontrar el modo de usar el ampo tipo password y validar si se ha definido
 // la contraseña del webservice, por el momento se maneja un campo de texto

 function password_button_ajax ($form, &$form_state) {
 
    $form['wsdl_config']['auth_password']['#access'] = TRUE;

    return $form['wsdl_config']['auth_password'];
}
*/

function _create_services_descriptions($user, $passwd, $env) {

    $services = array();

    /** 
     * Define WS que suministra el método requerido para
     * realizar una petición al Sistema Distrital de Quejas
     * y Soluciones -SDQS-
     **/
    $service = new WSClientServiceDescription();

    $service->name = 'service_registro_peticion';
    $service->label = 'Descripción del Webservice para realizar una petición al SDQS';
    if ($env == 'prod') {
        $service->url = 'http://sdqs.bogota.gov.co/sdqs/servicios/WSRegistrarPeticionService?wsdl';
    }
    elseif($env == 'test') {
        $service->url = 'http://www.alcaldiabogota.gov.co/sdqs/servicios/WSRegistrarPeticionService?wsdl';
    }

    $services[$service->name] = $service;

    /** 
     * Define WS que suministra los métodos requeridos para
     * obtener los códigos correspondientes a la definición y 
     * tipología del SDQS, los cuales son requeridos por otros
     * métodos del sistema.
     **/

    $service = new WSClientServiceDescription();

    $service->name = 'service_consulta';
    $service->label = 'Descripción del Webservice SDQS con los bindings para las consultas';
    if ($env == 'prod') {
        $service->url = 'http://sdqs.bogota.gov.co/sdqs/servicios/WSConsultasService?wsdl';
    } 
    elseif ($env == 'test') {
        $service->url = 'http://www.alcaldiabogota.gov.co/sdqs/servicios/WSConsultasService?wsdl';
    }

    $services[$service->name] = $service;

    /** 
     * Define WS que suministra los métodos que permiten
     * la radicación peticiones a traves de clientes asociados
     * a sistemas de gestión documental u otros sistemas
     **/

    $service = new WSClientServiceDescription();

    $service->name = 'service_radicacion';
    $service->label = 'Descripción del Webservice SDQS para efectuar las operaciones de radicación';

    if ($env == 'prod') {
        $service->url = 'http://sdqs.bogota.gov.co/sdqs/servicios/RadicacionService?wsdl';
    } 
    elseif ($env == 'test') {
        $service->url = 'http://www.alcaldiabogota.gov.co/sdqs/servicios/RadicacionService?wsdl';
    }

    $services[$service->name] = $service;

    foreach ($services as $service) {

        // Verifica que no exista un webservice con el mismo nombre para evitar
        // el romper la integridad de la base de datos.

        $ws_check = db_select('wsclient_service', 'n')
                  ->fields('n')
                  ->condition('name', $service->name, '=')
                  ->execute()
                  ->fetchAssoc();

        if ($ws_check['name'] !== $service->name) {

            $service->type = 'soap';
            $service->settings['options']['login'] = $user;
            $service->settings['options']['password'] = $passwd;
            
            try {
                $service->endpoint()->initializeMetadata();
                $service->save();
            }
            catch (WSClientException $e) {
                watchdog('wsclient', $e->__toString());
            }
        } else {
            // Notificar validación de existencia y redireccionar a la 
            // administración del WS en el modulo wsclient
            drupal_set_message('Existe una descripción de '. $service->name .' en la base de datos, diríjase a la <b><a href = "'. WSCLIENT_UI_PATH .'/manage/'. $service->name .'">administración del webservice</a></b> para mayor información.', 'warning');

            // TODO: Actualizar la descripción del WS con los datos del 
            // formulario y notificar
        }
    }

    return $services;
}

function _get_operations_data($opts, $services) {

    $consultaWS = wsclient_service_load($services['service_consulta']->name);
    //dsm($consultaWS);

    foreach ($opts as $op) {
        if (!empty($op)) {

            try {
                $results = $consultaWS->invoke($op, array());
                _save_results_as_drupal($results, $op);
            } catch (WSClientexception $e) {
                watchdog('wsclient', $e->__toString());
                drupal_set_message('Ha ocurrido un error, verifique el log de errores para obtener mayor detalle', 'error');
            }
        }
    }
}

function _save_results_as_drupal($data, $op) {

    $data = object_to_array($data);

    if ($op === 'getListaEntidadesDistritales') $op = 'lista_entidades';
    if ($op === 'getListaTipoPeticion') $op = 'lista_tipo_peticion';
    if ($op === 'getListaTema') $op = 'lista_tema';
    if ($op === 'getListaTipoIdentificacion') $op = 'lista_tipos_id';

    variable_set('govi_sdqs_'.$op, $data['return']);

}

/**
 * Función que transforma objetos multidimensionales
 * en arrays
 *
 * @ver http://www.if-not-true-then-false.com/2009/php-tip-convert-stdclass-object-to-multidimensional-array-and-convert-multidimensional-array-to-stdclass-object/
 */
function object_to_array ($o) {
    if(is_object($o)) {

        $o = get_object_vars($o);
    }

    if(is_array($o)){

        return array_map(__FUNCTION__, $o);
    } else {

        return $o;
    }
}
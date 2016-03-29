<?php

function govi_sdqs_admin_config_main_form($form, &$form_state) {

    $form = array();

    $temas = variable_get('govi_sdqs_lista_tema');
    $entidades = variable_get('govi_sdqs_lista_entidades');
    $tipos_peticion = variable_get('govi_sdqs_lista_tipo_peticion');
    $tipos_id = variable_get('govi_sdqs_lista_tipos_id');
    
    if (empty($temas)
        || empty($entidades)
        || empty($tipos_peticion)
        || empty($tipos_id)) {
        
        $msg = '<p>Uno o varios códigos del SDQS no se han cargado en el sistema. <br/>';
        $msg .= 'Seleccione las operaciones que desea cargar/actualizar ';
        $msg .= 'y guarde la configuración para que los cambios tomen ';
        $msg .= 'efecto.<br/>';
        $msg .= 'Los códigos serán utilizados en los selectores de los ';
        $msg .= 'formularios <em>Crear Petición y Configuración Básica.';

        drupal_set_message($msg, 'warning');
    }

    if (variable_get('sdqs_env') === 'test') {
        $msg = '';
        $msg .= 'Govi SDQS está funcionando en <b>modo de pruebas</b>';

        drupal_set_message($msg, 'warning');
    }

    $form['#attached']['css'] = array(
        drupal_get_path('module', 'govi_sdqs').'/assets/css/govi-sdqs.admin.css',
    );

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
            'getListaEntidad',
            'getListaTipoPeticion',
            'getListaTema',
            'getListaTipoIdentificacion'
        )),
        '#title' => 'Seleccione las operaciones en los cuales desea actualizar la codificación del Web Service (Estos valores serán almacendados como variables enel sistema de Drupal)',
    );

    $entidades = variable_get('govi_sdqs_lista_entidades');
    $opts_entidades = array();

    if (!empty($entidades)) {
        foreach ($entidades as $entidad) {
            $opts_entidades[$entidad['id']] = $entidad['nombre'];
        }
    }

    $form['wsdl_config']['codigo_entidad'] = array(
        '#type' => 'select',
        '#title' => 'Por favor seleccione la entidad a la cual pertenece este sitio',
        '#description' => 'Este valor será usado en la petición realizada por el ciudadano',
        '#options' => !empty($opts_entidades) ? $opts_entidades : array(),
        '#required' => TRUE,
        '#default_value' => variable_get('sdqs_entidad'),
        '#ajax' => array(
            'event' => 'change',
            'effect' => 'fade',
            'callback' => 'govi_sdqs_obtener_info_entidad',
            'method' => 'replace',
            'wrapper' => 'wrapper-dependencia'
        ),
        '#required' => TRUE,
        '#validated' => TRUE

    );

    $opts_dependencias = variable_get('govi_sdqs_lista_dependencias');

    $form['wsdl_config']['codigo_dependencia'] = array(
        '#prefix' => '<div id="wrapper-dependencia">',
        '#suffix' => '</div>',
        '#type' => 'select',
        '#title' => 'Por favor seleccione la dependencia',
        '#default_value' => variable_get('sdqs_dependencia'),
        '#options' => !empty($opts_dependencias) ? $opts_dependencias : array(),
        '#validated' => TRUE
    );

    $form['apariencia'] = array(
        '#type' => 'fieldset',
        '#title' => 'Con esta opción puede cambiar la apariencia del módulo SDQS',
    );

    $form['apariencia']['tema'] = array(
        '#type' => 'select',
        '#title' => 'Seleccione el tema que desea usar para la visualización del módulo',
        '#options' => array(
            'light' => 'light',
            'dark' => 'dark'
        ),
        '#ajax' => array(
            'event' => 'change',
            'effect' => 'fade',
            'wrapper' => 'govi-sdqs-tema-thumb',
            'method' => 'replace',
            'callback' => 'govi_sdqs_tema_cambiar_apariencia',
        ),
        '#default_value' => variable_get('govi_sdqs_tema', 'dark'),
        '#required' => TRUE

    );

    $form['apariencia']['previsualizacion'] = array(
        '#markup' => '<div id="govi-sdqs-tema-thumb" class="'.variable_get('govi_sdqs_tema').'"></div>',
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Guardar',
    );


    return $form;
}

function govi_sdqs_obtener_info_entidad($form, $form_state) {

    $entidad = $form_state['values']['codigo_entidad'];
    $entidades = variable_get('govi_sdqs_lista_entidades');
    $service = wsclient_service_load('service_consulta');

    $data = array(
        'parameters' => array(
            'idEntidad' => $entidad
        ),
    );

    try {

        $opts_dependencias = array();
        $response = $service->invoke('getListaDependencia' , $data);

        if (!empty($response)) {

            foreach ($response->return as $dep) {

                $opts_dependencias[$dep->id] = $dep->nombre;
            }
        }

        reset($opts_dependencias);
        variable_set('govi_sdqs_lista_dependencias', $opts_dependencias);
        variable_set('sdqs_dependencia', key($opts_dependencias));
        variable_set('sdqs_entidad', $entidad);

        $form['wsdl_config']['codigo_dependencia']['#options'] = $opts_dependencias;

        drupal_set_message('Debe guardar el formulario para que los cambios tengan efecto', 'warning');

        // Obtiene el sector al que la entidad pertenece
        foreach ($entidades as $item) {
            if ($item['id'] === (int)$entidad) {
                variable_set('sdqs_sector', $item['idSector']);
                break;
            }
        }

        return $form['wsdl_config']['codigo_dependencia'];
    } catch (WSClientException $exception) {
        $output .= '<h3>Ha ocurrido un error inesperado</h3>';
        $output .= '<p>Consulte con el administador para verificar el log de errores</p>';

        watchdog('govi_sdqs', $exception->__toString());
    }
}

function govi_sdqs_tema_cambiar_apariencia($form, $form_state) {

    $tema = $form_state['values']['tema'];
    $output = '';
    if ($tema === 'light') {
        $output .= '<div id="govi-sdqs-tema-thumb" class="light"></div>';
    } elseif ($tema === 'dark') {
        $output .= '<div id="govi-sdqs-tema-thumb" class="dark"></div>';
    }
    return $output;
}

function govi_sdqs_admin_config_main_form_submit($form, &$form_state) {

    $user = $form_state['values']['auth_user'];
    $passwd = $form_state['values']['auth_password'];
    $env = $form_state['values']['sdqs_env'];
    $opts = $form_state['values']['get_codes'];
    $entidad = $form_state['values']['codigo_entidad'];
    $tema = $form_state['values']['tema'];
    $dependencia =  $form_state['values']['codigo_dependencia'];


    variable_set('sdqs_user', $user);
    variable_set('sdqs_password', $passwd);
    variable_set('sdqs_env', $env);
    variable_set('sdqs_entidad', $entidad);
    variable_set('sdqs_dependencia', $dependencia);
    variable_set('govi_sdqs_tema', $tema);


    $services = _create_services_descriptions($user, $passwd, $env);

    // TODO: Services debería ser una variable con la lista de 
    // servicios disponibles

    if (!empty($opts)) {
        _get_operations_data($opts, $services);
    }

    drupal_set_message('La configuración ha sido almacenada', 'status');

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

    if ($op === 'getListaEntidad') $op = 'lista_entidades';
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
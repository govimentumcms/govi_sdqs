<?php

function govi_sdqs_admin_config_main_form($form, &$form_state) {

    $form = array();

    $temas = variable_get('govi_sdqs_lista_tema');
    $entidades = variable_get('govi_sdqs_lista_entidades');
    $tipos_peticion = variable_get('govi_sdqs_lista_tipo_peticion');
    $tipos_id = variable_get('govi_sdqs_lista_tipos_id');
    $path_modulo = drupal_get_path('module', 'govi_sdqs');
    
    $actual_argument =arg();

    if (variable_get('sdqs_env') === 'test' && end(($actual_argument)) != 'ajax') {
        $msg = '';
        $msg .= 'Govi SDQS está funcionando en <b>modo de pruebas</b>';

        drupal_set_message($msg, 'warning');
    }
    
    $form['#attached']['css'] = array(
        $path_modulo.'/assets/css/govi-sdqs.admin.css',
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
    $form['wsdl_config']['validar_conexion'] = array(
        '#type' => 'button',
        '#prefix' => '<div id="test-connection-btn">',
        '#suffix' => '</br>Se validará la conexión con la contraseña previamente almacenada. Si desea probar con una nueva contraseña guarde los cambios primero y luego si ejecute</div>',
        '#value' => 'Probar Conexión',
        '#description' => 'Se validará la conexión con la contraseña previamente almacenada. Si desea probar con una nueva contraseña guarde los cambios primero.',

        '#ajax' => array(
            'callback' => 'test_connection',
            'event' => 'click',
            'method' => 'replace',
            'effect' => 'fade',
            'wrapper' => 'test-connection-btn'
        ),
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
    $lista_operaciones = array(

            'getListaEntidad',
            'getListaTipoPeticion',
            'getListaTema',
            'getListaTipoIdentificacion',
            'getListaPais',
            'getListaDepatamento',
            'getListaCiudad',

        );

    $form['wsdl_config']['get_codes'] = array(
        '#type' => 'checkboxes',
        '#options' => drupal_map_assoc($lista_operaciones),
        '#title' => 'Seleccione la lista de valores a actualizar. Esta opción consulta y trae los siguientes contenidos del servicio web y luego los almacena  localmente .(Estos valores serán almacendados como variables en el sistema de Drupal)',
    );

    $entidades = variable_get('govi_sdqs_lista_entidades');
    $opts_entidades = array();

    if (!empty($entidades)) {
        foreach ($entidades as $entidad) {
            $opts_entidades[$entidad['id']] = $entidad['nombre'];
        }
    }


    if(!empty(variable_get('sdqs_entidad'))) {
        $form['wsdl_config']['entidad_seleccionada'] = array(
            '#type' => 'item',
            '#title'=> t('Entidad seleccionada'),
            '#markup' => (!empty(variable_get('sdqs_entidad')) ? $opts_entidades[variable_get('sdqs_entidad')]: 'No ha seleccionado una entidad.').' '.'<a class="change-item" href="#" data-change="form-item-codigo-entidad"  >cambiar</a>',
        );    
    }


    $form['wsdl_config']['codigo_entidad'] = array(
        '#type' => 'select',
        '#title' => 'Por favor seleccione la entidad a la cual pertenece este sitio',
        '#description' => 'Este valor será usado en la petición realizada por el ciudadano',
        '#options' => !empty($opts_entidades) ? $opts_entidades : array(),
        '#required' => TRUE,
        '#default_value' => variable_get('sdqs_entidad'),
        '#attributes' => array('class' => ( Array(!empty(variable_get('sdqs_entidad')) ? 'hide' : 'show')) ),
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


    if(!empty(variable_get('sdqs_dependencia'))) {
        $form['wsdl_config']['dependencia_seleccionada'] = array(
            '#type' => 'item',
            '#title'=> t('Dependencia seleccionada'),
            '#markup' => (!empty(variable_get('sdqs_dependencia')) ? $opts_dependencias[variable_get('sdqs_dependencia')]: 'No ha seleccionado una dependencia.').' '.'<a class="change-item" data-change="form-item-codigo-dependencia" href="#"  >cambiar</a>',
        );    
    }

    $form['wsdl_config']['codigo_dependencia'] = array(
        '#prefix' => '<div id="wrapper-dependencia">',
        '#suffix' => '</div>',
        '#type' => 'select',
        '#title' => 'Por favor seleccione la dependencia',
        '#default_value' => variable_get('sdqs_dependencia'),
        '#options' => !empty($opts_dependencias) ? $opts_dependencias : array(),
        '#attributes' => array('class' => (Array(!empty(variable_get('sdqs_dependencia')) ? 'hide' : 'show'))),
        '#validated' => TRUE
    );

    if (!empty($temas)) {
        foreach ($temas as $tema) {
            $opts_temas[$tema['id']] = $tema['nombre'];
        }

    }
    if(!empty(variable_get('sdqs_tema'))) {
        $form['wsdl_config']['tema_seleccionado'] = array(
            '#type' => 'item',
            '#title'=> t('Dependencia seleccionada'),
            '#markup' => (!empty(variable_get('sdqs_tema')) ? $opts_temas[variable_get('sdqs_tema')]: 'No ha seleccionado una tema.').' '.'<a class="change-item" data-change="form-item-codigo-tema" href="#"  >cambiar</a>',
        );    
    }

    $form['wsdl_config']['codigo_tema'] = array(
        '#prefix' => '<div id="wrapper-tema">',
        '#suffix' => '</div>',
        '#type' => 'select',
        '#title' => 'Por favor seleccione el tema principal de su dependencia',
        '#default_value' => variable_get('sdqs_tema'),
        '#options' => !empty($opts_temas) ? $opts_temas : array(),
        '#attributes' => array('class' => ( Array(!empty(variable_get('sdqs_tema')) ? 'hide' : 'show')) ),

        '#validated' => TRUE
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Guardar',
    );
    
    $path = drupal_get_path('module', 'govi_sdqs');
    $js = $path . '/assets/js/govi-sdqs-form-admin.js';
    drupal_add_js($js);
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


        $opts_dependencias = array();
        $response = $service->invoke('getListaDependencia' , $data);
        $opts_dependencias[0] = 'NO SELECCIONAR DEPENDENCIA';

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


        // Obtiene el sector al que la entidad pertenece
        foreach ($entidades as $item) {
            if ($item['id'] === (int)$entidad) {
                variable_set('sdqs_sector', $item['idSector']);
                break;
            }
        }

        return $form['wsdl_config']['codigo_dependencia'];

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
    if (in_array("tema", $form_state['values'])) {
      $tema = $form_state['values']['tema'];
    }

    $dependencia =  $form_state['values']['codigo_dependencia'];
    $tema_principal = $form_state['values']['codigo_tema'];
    $actual_env = variable_get('sdqs_env');


    //Si hay cambio de entorno actualiza los registros de las listas
    if($actual_env != $env) {
        variable_del('govi_sdqs_lista_tema');
        variable_del('govi_sdqs_lista_entidades');
        variable_del('govi_sdqs_lista_tipo_peticion');
        variable_del('govi_sdqs_lista_tipos_id');
        variable_del('govi_sdqs_lista_paises');
        variable_del('govi_sdqs_lista_departamentos');
        variable_del('govi_sdqs_lista_dependencias');
        variable_del('govi_sdqs_lista_ciudades');
        variable_del('govi_sdqs_getListaDependencia');
        variable_del('sdqs_entidad');
        variable_del('sdqs_dependencia');
        variable_del('govi_sdqs_tema');
        variable_del('sdqs_sector');
        $opts = Array(
          'getListaEntidad'=>'getListaEntidad',
          'getListaTipoPeticion'=>'getListaTipoPeticion',
          'getListaTema'=>'getListaTema',
          'getListaTipoIdentificacion'=>'getListaTipoIdentificacion',
          'getListaPais'=>'getListaPais',
          'getListaDepatamento'=>'getListaDepatamento',
          'getListaCiudad'=>'getListaCiudad',
        );
    }
    
    variable_set('sdqs_user', $user);
    variable_set('sdqs_password', $passwd);
    variable_set('sdqs_env', $env);
    variable_set('sdqs_entidad', $entidad);
    variable_set('sdqs_dependencia', $dependencia);
    if (!empty($tema)) {
      variable_set('govi_sdqs_tema', $tema);
    }
    variable_set('sdqs_tema', $tema_principal);

    $services = _create_services_descriptions($user, $passwd, $env);

    // TODO: Services debería ser una variable con la lista de 
    // servicios disponibles
    if (!empty($opts)) {
        _get_operations_data($opts, $services);
    } else {
        print 'empty';
        exit();
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

function test_connection ($form, &$form_state) {
 
    $consultaWS = wsclient_service_load('service_consulta');
    try {
      $results = $consultaWS->invoke('getListaRedSocial', Array());

      if(!empty($results->return)){
        return ' <div class="messages status">
        <h2 class="element-invisible">Status message</h2>
            La conexión con el servicio web SDQS está funcionando correctamente para el usuario '.$form_state['values']['auth_user'].'.
        </div>
        ';
      }

    } catch (WSClientException $e) {
      watchdog('wsclient', $e->__toString());
      return '<div class="messages error">
              <h2 class="element-invisible">Error message</h2>
               No se puede conectar con el servicio web SDQS. Valide sus credenciales.
              </div>';
    }


}

function _create_services_descriptions($user, $passwd, $env) {

    $services = array();

    /** 
     * Define WS que suministra los métodos que permiten
     * la radicación peticiones a traves de clientes asociados
     * tipología del SDQS, los cuales son requeridos por otros
     * métodos del sistema.
     **/
    $name='service_consulta';
    $service = wsclient_service_load($name);
    if(empty($service->settings)) {
      $service = new WSClientServiceDescription();
    }
    $service->name = $name;
    $service->label = 'service_consulta_sdqs';

    if ($env == 'prod') {
        $service->url = 'http://sdqs.bogota.gov.co/sdqs/servicios/WSConsultasService?wsdl';
    } 
    elseif ($env == 'test') {
        $service->url = 'http://www.alcaldiabogota.gov.co/sdqs/servicios/WSConsultasService?wsdl';
    }
    $service->type="soap";
    $service->settings['options']['login'] = $user;
    $service->settings['options']['password'] = $passwd;

    $services[$service->name] = $service;

    /** 
     * Define WS dedicado para los clientes SDQS de la Distribución
     * Distrital CMS que suministra los métodos que permiten
     * consulta códigos y la radicación de peticiones, quejas,
     * reclamos y felicitaciones.
     * Además proporciona el método para la selección del canal a
     * través del cual se realiza la petición. En ese caso el canal
     * se define como Web.
     **/

    $name='service_radicacion_canal';
    $service = wsclient_service_load($name);
    if(empty($service->settings)) {
      $service = new WSClientServiceDescription();
    }
    $service->name = $name;
    $service->label = 'service_radicacion_canal_sdqs';

    if ($env == 'prod') {
        $service->url = 'http://sdqs.bogota.gov.co/sdqs/servicios/RadicacionPorCanalService?wsdl';
    } 
    elseif ($env == 'test') {
        $service->url = 'http://www.alcaldiabogota.gov.co/sdqs/servicios/RadicacionPorCanalService?wsdl';
    }
    $service->type="soap";
    
    $service->settings['options']['login'] = $user;
    $service->settings['options']['password'] = $passwd;

 
    $services[$service->name] = $service;

    foreach ($services as $service) {
      try {
        $service->endpoint()->initializeMetaData();
        $service->save();
      }
      catch (WSClientException $e) {
        watchdog('wsclient', $e->__toString());
      }
    }



    return $services;
}
function _getOperationgetListaCiudad($services){
    $consultaWS = wsclient_service_load($services['service_consulta']->name);
    $dptos = _getOperationgetListaDepatamento($services);
    $departamentos = array_map(create_function('$o', 'return $o->id;'), $dptos->return);
    $ciudades = array();
    foreach($departamentos as $departamento) {
        $arguments = array(
            'parameters' => array(
              'idDepartamento' => $departamento
            ), 
        );
        $result = $consultaWS->invoke('getListaCiudad', $arguments);
        if(!is_array($result->return)){
            $result->return = array( 0 => $result->return);
        }

        $ciudades = array_merge((array)$result->return, $ciudades);
        
    }
    return Array('return' => $ciudades);
}
function _getOperationgetListaDepatamento($services){
    $consultaWS = wsclient_service_load($services['service_consulta']->name);
    $arguments = array(
        'parameters' => array(
          'idPais' => 42
        ),
    );
    $results = $consultaWS->invoke('getListaDepatamento', $arguments);
    return $results;
}
function _get_operations_data($opts, $services) {

    $consultaWS = wsclient_service_load($services['service_consulta']->name);

    foreach ($opts as $op => $value) {
        if (!empty($op)) {
            $results = NULL;
            $arguments = array();
            try {
                switch ($op) {
                    case 'getListaDepatamento':
                        $results = _getOperationgetListaDepatamento($services);
                        break;
                    case 'getListaCiudad':
                        $results = _getOperationgetListaCiudad($services);
                        break;
                    default:

                        $results = $consultaWS->invoke($op, $arguments);
                        break;
                }

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
    if ($op === 'getListaPais') $op = 'lista_paises';
    if ($op === 'getListaDepatamento') $op = 'lista_departamentos';
    if ($op === 'getListaCiudad') $op = 'lista_ciudades';
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
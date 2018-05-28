<?php

/**
 * @file
 * SDQS Client handler.

 */
 /**
  * Represents the required functions for a Singleton.
  */
 interface SingletonInterface {

   /**
    * Returns instance, if instance does not exist then creates new one
    * and returns it.
    *
    * @return $this
    */
   public static function getInstance();

 }

 /**
  * Singleton trait.
  */
 trait SingletonTrait {

   /**
    * @var self[]
    */
   private static $instances = [];

   /**
    * Returns instance, if instance does not exist then creates new
    * one and returns it.
    *
    * @return $this
    */
   public static function getInstance() {
     $self = get_called_class();
     if (!isset(self::$instances[$self])) {
       self::$instances[$self] = new $self();
     }
     return self::$instances[$self];
   }

   /**
    * @return bool true if has instance, otherwise false.
    */
   protected static function hasInstance() {
     $self = get_called_class();
     return isset(self::$instances[$self]);
   }

 }

/**
 * Base class for sdqs_handlers.
 * Implementation of Singleton design pattern.
 */

class SdqsClient implements SingletonInterface {

  use SingletonTrait;

  private $service;
  private $enviroment;

  function __construct() {
    $this->enviroment = variable_get('govi_sdqs_site_env');
    if(isset($this->enviroment)) {
      $this->service = $this->loadWebservice();
    }
  }

  public function isConnectionConfigured() {
    $user = variable_get('govi_sdqs_site_user');
    $password = variable_get('govi_sdqs_secret_password');
    $enviroment = variable_get('govi_sdqs_site_env');
    $s = $this->loadWebservice();
    if(!empty($user) && !empty($password) && !empty($enviroment) && !empty($s)) {
      return true;
    }
    return false;
  }
  public function updateDataOperation($operation, $variable, $arguments =  Array()) {
    $response = $this->service->invoke($operation, $arguments);

    return $response->return->list;
  }

  public function updateEntities() {
    $entities = $this->updateDataOperation('consultarEntidadesDistritales');
    $entitiy_list = Array(0 => 'NO SELECCIONADA');
    foreach ($entities as $entity) {
      $entitiy_list[$entity->id] = $entity->nombre;
    }
    variable_set('sdqs_entities', $entitiy_list);
  }
  public function updateRequestTypes() {
    $response = $this->updateDataOperation('consultarTipoPeticion');
    variable_set('sdqs_requests', $response);

  }
  public function updateCountries() {
    $response = $this->updateDataOperation('consultarPaises');
    variable_set('sdqs_countries', $response);

  }
  public function updateIdentificationTypes() {
    $response = $this->updateDataOperation('consultarTiposIdentificacion');
    variable_set('sdqs_identification_types', $response);


  }
  public function getDependencyList($ent) {
    $arguments = array(
      'parameters' => array(
        'codigoEntidad' => Array(
          'codigoEntidad' => $ent,
        )
      ),
    );
    
    $dependencies = $this->service->invoke('consultarDependencia', $arguments)->return->list;
    $dependency_list = Array(0 => 'NO SELECCIONADA');
    foreach ($dependencies as $dependency) {
      $dependency_list[$dependency->id] = $dependency->nombre;
    }
    return $dependency_list;


  }

  public function loadWebservice(){
    $name = 'sdqs_'.$this->enviroment;
    $ws = wsclient_service_load($name);
    if(!empty($ws)) {
      return $ws;
    }
    return NULL;
  }
  public function getService() {
    return $this->service;
  }

  public function getEndpoint() {
    if($this->enviroment == 'prod'){
      return 'http://sdqs.bogota.gov.co/sdqs/servicios/RadicacionPorCanalService?wsdl';
    }
    return 'http://200.75.50.41:8080/sdqs/servicios/RadicacionPorCanalService?wsdl';
  }

  /**
   * Check Connection with SDQS Server
   */

  public function checkConnection($user, $pasword, $enviroment) {
    $this->enviroment = $enviroment;


    $s = new WSClientServiceDescription();
    $s->name = 'sdqs_'.$this->enviroment;
    $s->label = 'sdqs_'.$this->enviroment;
    $s->url = $this->getEndpoint();
    $s->type="soap";
    $s->settings['options']['login'] = $user;
    $s->settings['options']['password'] = $pasword;
    try {
      $s->endpoint()->initializeMetaData();
      $result = $s->invoke('consultarCodigosError', Array());
      $s->save();
      $this->service =  $s;
      return true;
    }
    catch (WSClientException $e) {f-
      watchdog('wsclient', $e->__toString());
      return false;
    }
  }
}

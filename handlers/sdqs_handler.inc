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
    if(isset($user) && isset($password) && isset($enviroment) && isset($s)) {
      return true;
    }
    return false;
  }
  public function updateDataOperation($operation, $variable, $arguments =  Array()) {
    $response = $this->service->invoke($operation, $arguments);
    variable_set($variable, $response->return->list);
    return $response->return->list;
  }

  public function updateEntities() {
    return $this->updateDataOperation('consultarEntidadesDistritales','sdqs_entities');
  }
  public function updateCountries() {
    return $this->updateDataOperation('consultarPaises','sdqs_countries');
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
    return 'http://www.alcaldiabogota.gov.co/sdqs/servicios/RadicacionPorCanalService?wsdl';
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
    catch (WSClientException $e) {
      watchdog('wsclient', $e->__toString());
      return false;
    }
  }
}

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
  public $enviroment;

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
public function updateDataOperation($operation, $variable = '', $arguments =  Array()) {
    $response = $this->service->invoke($operation, $arguments);

    return $response->return->list;
  }
  public function register($data){
    if(isset($data['anonimo']) && $data['anonimo']){
      unset($data['anonimo']);
      return $this->service->invoke('registrarPeticion', $data);
    } else {
      return $this->service->invoke('registrarRequerimiento', $data);
    }

  }

  public function updateEntities() {
    $entities = $this->updateDataOperation('consultarEntidadesDistritales');
    $entitiy_list = Array(0 => 'NO SELECCIONADA');
    foreach ($entities as $entity) {
      $entitiy_list[$entity->id] = $entity->nombre;
    }
    variable_set('sdqs_entities', $entitiy_list);
  }
  public function updateSectors() {
    $sectors = $this->updateDataOperation('consultarSectores');
    $sector_list = Array(0 => 'NO SELECCIONADO');
    $entity_list = Array();
    foreach ($sectors as $sector) {
      $sector_list[$sector->codigoSector] = $sector->nombre;
      $entity_list[$sector->codigoSector] = $this->getEntityList($sector->codigoSector);
    }
    variable_set('sdqs_sectors', $sector_list);
    variable_set('sdqs_entities', $entity_list);

  }
  public function updatefileTypes() {
    $fileTypes = $this->updateDataOperation('consultarCategoriaArchivos');
    $files = Array();
    $file_extensions =  Array();
    $file_contentypes =  Array();

    foreach ($fileTypes as $type) {
      $files[$type->id] = Array('name'=>$type->nombre, 'extensions'=> Array());
      $arguments = array(
        'parameters' => array(
          'codigoCategoria' => Array(
            'codigoCategoria' => $type->id,
          )
        ),
      );
      $extensions = $this->service->invoke('consultarTipoArchivo', $arguments)->return->list;
      $files[$type->id]['extensions'] = (array) $extensions;
      foreach($files[$type->id]['extensions']  as $extension) {
        $file_contentypes[$extension->contentype] = $extension->id;
        $file_extensions[$extension->extension] = $extension->id;
      }

    }
    variable_set('sdqs_file_extensions', $file_extensions);
    variable_set('sdqs_file_contentypes', $file_contentypes);

    variable_set('sdqs_files', $files);
  }

  public function updateGenres() {
    $genres = $this->updateDataOperation('consultarGeneros');
    $genres_list = Array();
    foreach ($genres as $genre) {
      $genres_list[$genre->id] = $genre->nombre;
    }
    variable_set('sdqs_genres', $genres_list);
  }
  public function updateRequestTypes() {
    $response = $this->updateDataOperation('consultarTipoPeticion');
    variable_set('sdqs_requests', $response);

  }
  public function updateCountries() {
    $response = $this->updateDataOperation('consultarPaises');
    variable_set('sdqs_countries', $response);
  }
  public function updateCities(){
    $arguments = array(
      'parameters' => array(
        'codigoPais' => Array(
          'codigoPais' => 169,
        )
      ),
    );
    $cities_list =  Array();
    $departments = $this->service->invoke('consultarDepartamentosPais', $arguments)->return->list;
    foreach ($departments as $department) {
      $cities_list[$department->codigoDepartamento] = Array();
      $arguments = array(
        'parameters' => array(
          'codigoDepartamento' => Array(
            'codigoDepartamento' => $department->codigoDepartamento,
          )
        ),
      );
      $cities = $this->service->invoke('consultarCiudadesDepartamento', $arguments)->return->list;
      if($cities->codigoCiudad) {
        $cities_list[$cities->codigoCiudad] = $cities->nombre;
      } else {
        foreach ($cities as $city) {
          $cities_list[$department->codigoDepartamento][$city->codigoCiudad] = $city->nombre;
        }
      }
    }
    variable_set('sdqs_cities', $cities_list);
    return $cities_list;
  }

  public function updateDepartments() {
    $arguments = array(
      'parameters' => array(
        'codigoPais' => Array(
          'codigoPais' => 169,
        )
      ),
    );
    $departments = $this->service->invoke('consultarDepartamentosPais', $arguments)->return->list;
    $deparments_list =  Array();
    foreach ($departments as $department) {
      $deparments_list[$department->codigoDepartamento] = $department->nombreDepartamento;
    }
    variable_set('sdqs_departments', $deparments_list);

  }
  public function updateIdentificationTypes() {
    $response = $this->updateDataOperation('consultarTiposIdentificacion');
    $id_types =  Array();
    foreach ($response as $type) {
      $id_types[$type->id] = $type->nombre;
    }
    variable_set('sdqs_identification_types', $id_types);
  }
  public function updateComplaintType() {
    $response = $this->updateDataOperation('consultarTipoPeticion');
    $complaintTypes =  Array();
    foreach ($response as $type) {
      $complaintTypes[$type->id] = $type->nombre;
    }
    variable_set('sdqs_complaint_types', $complaintTypes);
  }
  public function getEntityList($ent) {
    $arguments = array(
      'parameters' => array(
        'consultaEntidades' => Array(
          'codigoSector' => $ent,
          'codigoEntidad' => '',
        )
      ),
    );

    $dependencies = $this->service->invoke('consultarEntidades', $arguments)->return->list;
    $dependency_list = Array(0 => 'NO SELECCIONADA');
    if(empty($ent)) {
      return $dependency_list;
    }

    if(!is_array($dependencies)) {
      $dependency_list[$dependencies->codigoEntidad] = $dependencies->nombre;
    } else {
      foreach ($dependencies as $dependency) {
        $dependency_list[$dependency->codigoEntidad] = $dependency->nombre;
      }      
    }
    return $dependency_list;

  }

  public function getDependencyList($ent) {
    $arguments = array(
      'parameters' => array(
        'codigoEntidad' => Array(
          'codigoEntidad' => $ent,
        )
      ),
    );
    $dependencies = (!empty($ent)) ? $this->service->invoke('consultarDependencia', $arguments)->return->list : NULL;
    $dependency_list = Array(0 => 'NO SELECCIONADA');
    if(empty($ent)) {
      return $dependency_list;
    }
    foreach ($dependencies as $dependency) {
      $dependency_list[$dependency->id] = $dependency->nombre;
    }
    return $dependency_list;


  }
  public function getThemeList($ent) {
    $arguments = array(
      'parameters' => array(
        'codigoEntidad' => Array(
          'codigoEntidad' => '',
        )
      ),
    );

    $themes = $this->service->invoke('consultarTemas', $arguments)->return->list;
    $theme_list = Array(0 => 'NO SELECCIONADO');

    foreach ($themes as $theme) {
      $theme_list[$theme->id] = $theme->nombre;
    }
    return $theme_list;

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
    } elseif ($this->enviroment == 'test') {
        return 'http://200.75.50.41:8080/sdqs/servicios/RadicacionPorCanalService?wsdl';
    }
  }

  /**
   * Check Connection with SDQS Server
   */

  public function checkConnection($user, $pasword, $enviroment) {
    $name = 'sdqs_'.$this->enviroment;

   $service = wsclient_service_load($name);
    if(empty($service->settings)) {
      $s = new WSClientServiceDescription();
    } else {
      $s = $service;
    }

    $s->name = $name;
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

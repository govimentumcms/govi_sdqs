<?php //dsm(get_defined_vars()); 

$tema = variable_get('govi_sdqs_tema') === 'light' ? 'class="light"' : ''; 
?>

<?php
if (variable_get('sdqs_env') === '') {
	$msg = 'Bienvenido al Sistema Distrital de Quejas y Soluciones -SDQS- <br/>';
	$msg .= '<hr/>';
	$msg .= 'Este módulo está temporalmente en <b>modo de pruebas</b>, ';
	$msg .= 'las peticiones enviadas a través de este formulario ';
	$msg .= '<b>no</b> serán atendidas por la Dirección Distrital de ';
	$msg .= 'Servicio al Ciudadano o entidad competente. ';
	$msg .= 'Si tiene alguna inquietud por favor escriba al correo electrónico: ';
	$msg .= '<a href="mailto:'.variable_get('site_mail').'" title="escribir">';
	$msg .= variable_get('site_mail').'</a> para obtener mayor información.<br>';
	$msg .= 'Si desea enviar una petición formal puede acceder al Sistema ';
	$msg .= 'Distrital de Quejas y Soluciones SDQS haciendo click ';
	$msg .= '<a href="http://www.bogota.gov.co/sdqs" title="Ir al Sistema Distrital de Quejas y Soluciones" target="_blank">';
	$msg .= 'aquí</a>.';

	drupal_set_message($msg, 'warning');

   $path = drupal_get_path('module', 'govi_sdqs');
   $css = $path . '/assets/css/govi-sdqs-form.css';
   $js = $path . '/assets/js/govi-sdqs-form.js';
   drupal_add_js($js);
   drupal_add_css($css);
}

?>
<div class="formsdqsn">
<p>Los campos marcados con * son obligatorios</p>
<fieldset>
   <legend>Datos de la solicitud</legend>
<div class="pure-g formsdqstic">

<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
   <?php print render($form['pqr_crear']['tipo_peticion']); ?>
</div>
<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
      <?php print render($form['datos_personales']['tipo_solicitante']); ?>
   </div>
</div>

</fieldset>

<fieldset class="id-data">
   <legend>Datos de identificación</legend>

   <div class="pure-g formsdqstic">

      <div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['nombre']); ?>
      </div>
      <div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['apellido']); ?>
      </div>
      <div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['tipo_identificacion']); ?>
      </div>
      <div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['numero_identificacion']); ?>
      </div>
      <div class="pure-u-1 pure-u-sm-1 pure-u-md-1-1 pure-u-lg-1-1 pure-u-xl-1-1">
          <?php print render($form['datos_personales']['razon_social']); ?>
      </div>
   </div>

</fieldset>

<fieldset class="contact">
   <legend>Ubicación y contacto</legend>
      <div class="pure-g formsdqstic">

      <div class="no-anon pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['pais']); ?>
      </div>
      <div class="no-anon pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['departamento']); ?>
      </div>
      <div class="no-anon pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['ciudad']); ?>
      </div>
      <div class="no-anon pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['direccion']); ?>
      </div>
      <div class="email pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
          <?php print render($form['datos_personales']['correo_electronico']); ?>
      </div>
      <div class="no-anon pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['telefono_fijo']); ?>
      </div>
      <div class="no-anon pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['datos_personales']['telefono_movil']); ?>
      </div>
   </div>
</fieldset>
<fieldset>
   <legend>Solicitud</legend>
   <div class="pure-g formsdqstic">
       <div class="pure-1-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['pqr_crear']['tema']); ?>
      </div>
      <div class="pure-1-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['pqr_crear']['entidad']); ?>
      </div>
      <div class="pure-1-1 pure-u-sm-1 pure-u-md-23-24 pure-u-lg-23-24 pure-u-xl-23-24">
         <?php print render($form['pqr_crear']['asunto']); ?>
      </div>
      <div class="pure-1-1 pure-u-sm-1 pure-u-md-23-24 pure-u-lg-23-24 pure-u-xl-23-24">
         <?php print render($form['pqr_crear']['archivo']); ?>
      </div>
      <div class="no-anon pure-1-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
         <?php print render($form['pqr_crear']['respuesta']); ?>
      </div>

   </div>

   </fieldset>
   <fieldset>
   <legend>Información sobre posibles costos asociados a la respuesta</legend>

      <div class="pure-1-1 pure-u-sm-1 pure-u-md-1-1 pure-u-lg-1-1 pure-u-xl-1-1">
         <p>La solicitud de información puede generar costos de reproducción de la información pública los cuales puede consultar en la sección de transparencia, instrumentos de gestión de la información, costos de reproducción.</p>
      </div>
   </fieldset>
<div class="pure-g formsdqstic">

   <div class="no-anon pure-1-1 pure-u-sm-1 pure-u-md-23-24 pure-u-lg-23-24 pure-u-xl-23-24">
     <?php print render($form['pqr_crear']['terminos_habeas_data']); ?>
   </div>
   <div class="no-anon pure-1-1 pure-u-sm-1 pure-u-md-23-24 pure-u-lg-23-24 pure-u-xl-23-24">
     <?php print render($form['pqr_crear']['terminos_correo']); ?>
   </div>
   <div class="pure-u-1 pure-sm-1 pure-u-md-1-1 pure-u-lg-1-1 pure-u-xl-1-1">
      <?php print render($form['captcha']); ?>
   </div>

   <div class="pure-u-1 pure-sm-1 pure-u-md-1-1 pure-u-lg-1-1 pure-u-xl-1-1">
      <?php print render($form['submit']); ?>
   </div>
</div>

</div>
<?php print render($form['form_build_id']); ?>
<?php print render($form['form_token']); ?>
<?php print render($form['form_id']); ?>

<?php drupal_render_children($form); ?>
